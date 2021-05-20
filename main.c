/* Trabalho Final de Laboratório de Sistemas Microprocessados
 * Grupo:
 *       Bruno Couto Mariño - 190011106
 *       Clara Resende Maia - 180030931
 */
/*Portas Usadas:
 *
 *  - Joystick (jogador 1)
 *    - GND -> GND
 *    - +5V -> 3V3
 *    - VRX -> P6.0
 *
 *  -Joystick (jogador 2)
 *    - GND -> GND
 *    - +5V -> 3V3
 *    - VRX -> P6.1
 */

#include <msp430.h> 
#include <stdint.h>
#include "libs/adc.h"
#include "libs/i2c.h"
#include "libs/oled.h"
#include "libs/game.h"
#include "libs/clock.h"

volatile uint16_t adcResult[2] = {0,0};
volatile uint8_t flag=0;

//Timer.h
typedef enum {us, ms, sec, min} timeunit_t;
uint16_t count;
uint8_t  timerCount = 0;

void wait(uint16_t time, timeunit_t unit);
void checkJoystick(Player * player, uint8_t i);
/* Devaneios de Bruno
   1. Definir Structs(jogador, bola) e tals (Clara)
   2. Move Barra
   3. Move Bola
   4. Bate bola na Barra ou parede
   5. Bola fez gol?
   6. Desenha no quadrinho (Bruno)
   7. Estado Inicial
   8.
   9.
  10.
 */
/*------------------- Rotina Principal -------------------------*/

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    /*------------------- Configuração -------------------*/

    // Configurar Timer A0 - Controla a captura do adc
        TA0CTL = (TASSEL__SMCLK | MC__UP);
        TA0CCR0 = 65536 - 1;                                // ~16Hz
        TA0CCTL0 = CCIE;                                    // Habilita interrupção do timer

    // Configurar o Clock
    clockConfig();

    // Configurar o ADC
    adcConfig();

    // Configurar o i2c
    i2cConfig();

  // Configurar o oled
    oledConfig();

    // Habilitar interrupções
    __enable_interrupt();

    oledClearDisplay();

    //Testes
    Player P1,P2;
    Ball ball;

    playersInit(&P1, &P2, &ball);

    drawVerticalBar(P1.bar.x, P1.bar.y, P1.bar.length, P1.bar.thickness);
    drawVerticalBar(P2.bar.x, P2.bar.y, P2.bar.length, P2.bar.thickness);
    drawVerticalBar(ball.x, ball.y, ball.size, ball.size);

    while(!(gameOver(&P1, &P2))){
        clear();

        checkJoystick(&P1,1);
        checkJoystick(&P2,2);
        moveBall(&ball);
        hitBar(&P1, &P2, &ball);

        changeScore(-1, P1.score);
        changeScore(1, P2.score);
        drawVerticalBar(P1.bar.x, P1.bar.y, P1.bar.length, P1.bar.thickness);
        drawVerticalBar(P2.bar.x, P2.bar.y, P2.bar.length, P2.bar.thickness);
        drawVerticalBar(ball.x, ball.y, ball.size, ball.size);
        drawMiddleLine();
        printBoard();

    }

    oledClearDisplay();
    if(P1.score == 12) {
        oledSendString(5, 3, "O da esquerda venceu!");
        oledSendString(5, 4, "UHUUUULL");
    }
    else {
        oledSendString(5, 3, "O da direita venceu!");
        oledSendString(5, 4, "UHUUUULL");
    }
}

void checkJoystick(Player * player, uint8_t i){
    if(adcResult[i-1]<10000){
        movePlayerUp(player);
        drawVerticalBar(player->bar.x, player->bar.y, player->bar.length, player->bar.thickness);

    }
    if(adcResult[i-1]>25000){
        movePlayerDown(player);
        drawVerticalBar(player->bar.x, player->bar.y, player->bar.length, player->bar.thickness);
    }
    return;

}








/*------------------- Interrupções -------------------------*/

// Interrupção que gera o flanco de subida para o ADC a uma taxa de 16Hz
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ADC_TRIGGER(){

    ADC12CTL0 &= ~ADC12SC;                  // Gera um flanco de...
    ADC12CTL0 |=  ADC12SC;                  // ...subida no bit SC
}

// Interrupção que calcula as médias dos valores de X dos joysticks
#pragma vector = ADC12_VECTOR
__interrupt void ADC_RESULT(){

    // Calcula o resultado do joystick 1
    adcResult[0] = (ADC12MEM0  +            // Coleta o resultado da Entrada no P6.6
                    ADC12MEM2  +
                    ADC12MEM4  +
                    ADC12MEM6  +
                    ADC12MEM8  +
                    ADC12MEM10 +
                    ADC12MEM12 +
                    ADC12MEM14 );
    // Calcula o resultado do joystick 2
    adcResult[1] = (ADC12MEM1  +            // Coleta o resultado da Entrada no P6.5
                    ADC12MEM3  +
                    ADC12MEM5  +
                    ADC12MEM7  +
                    ADC12MEM9  +
                    ADC12MEM11 +
                    ADC12MEM13 +
                    ADC12MEM15 );

    ADC12CTL0 |= ADC12ENC;                  // Habilita o trigger
    flag=1;
}


/*-------------------- Funções Utilizadas -----------------------*/





void wait(uint16_t time, timeunit_t unit) {
  if (unit == us) {
    // Use o SMCLK
    TA2CTL = TASSEL__SMCLK | MC__UP | TACLR;
    TA2CCR0 = time;

    while (!(TA2CCTL0 & CCIFG));
    TA2CCTL0 &= ~CCIFG;
  } else {
    // Use o ACLK
    TA2CTL = TASSEL__ACLK  | MC__UP | TACLR;

    if (unit == ms) {
      TA2CCR0 = (time << 5) - 1;

      while (!(TA2CCTL0 & CCIFG));

      TA2CCTL0 &= ~CCIFG;
    } else if (unit == sec) {
      count = time;

      TA2CCR0 = 0x8000 - 1;

      TA2CCTL0 = CCIE;
      while (count);
    } else if (unit == min) {
      count = time * 60;

      TA2CCR0 = 0x8000 - 1;

      TA2CCTL0 = CCIE;
      while (count);
    }
  }
  TA2CTL = MC__STOP;
}

//Interrupção do timer usado para wait()
#pragma vector = TIMER2_A0_VECTOR
__interrupt void TA2_CCR0_ISR() {
  count--;
}
