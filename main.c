/* Trabalho Final de Laboratório de Sistemas Microprocessados
 * Grupo:
 *       Bruno Couto Mariño - 190011106
 *       Clara Maia -
 */

#include <msp430.h> 
#include <stdint.h>

volatile uint16_t adcResult[2] = {0,0};
volatile uint8_t flag=0;

//Timer.h
typedef enum {us, ms, sec, min} timeunit_t;
uint16_t count;
uint8_t  timerCount = 0;



//Creio que precisaremos de 2 funções adcConfig(). Uma para cada joystick.
void adcConfig();

// Precisamos ver se precisa adaptar para usar o OLED.
void i2cConfig();
void i2cWriteByte(uint8_t addr, uint8_t byte);
uint8_t i2cWrite(uint8_t addr,uint8_t * data, uint8_t nBytes);

void wait(uint16_t time, timeunit_t unit);



/*------------------- Rotina Principal -------------------------*/

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	/*------------------- Configuração -------------------*/

	// Configurar Timer A0 - Controla a captura do adc
	    TA0CTL = (TASSEL__SMCLK | MC__UP);
	    TA0CCR0 = 65536 - 1;                                // ~16Hz
	    TA0CCTL0 = CCIE;                                    // Habilita interrupção do timer

	// Configurar o ADC
	adcConfig();

	// Configurar o i2c
	i2cConfig();

	// Habilitar interrupções
    __enable_interrupt();


    while(1){
    /*------------------- Loop Principal -------------------*/
        while(flag){

            //Atualiza o OLED
            flag=0;
        }
    }
}




/*------------------- Interrupções -------------------------*/

// Interrupção que gera o flanco de subida para a captura do joystick
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ADC_TRIGGER(){

    ADC12CTL0 &= ~ADC12SC;                  // Gera um flanco de...
    ADC12CTL0 |=  ADC12SC;                  // ...subida no bit SC
}

// Interrupção que calcula as médias
#pragma vector = ADC12_VECTOR
__interrupt void ADC_RESULT(){

    //atualiza Old Results
    adcResult[0] = (ADC12MEM0  +            // Coleta o resultado da Entrada no P6.6
                    ADC12MEM2  +
                    ADC12MEM4  +
                    ADC12MEM6  +
                    ADC12MEM8  +
                    ADC12MEM10 +
                    ADC12MEM12 +
                    ADC12MEM14 );

    adcResult[1] = (ADC12MEM1  +            // Coleta o resultado da Entrada no P6.5
                    ADC12MEM3  +
                    ADC12MEM5  +
                    ADC12MEM7  +
                    ADC12MEM9  +
                    ADC12MEM11 +
                    ADC12MEM13 +
                    ADC12MEM15 );

    ADC12CTL0 |= ADC12ENC;                  // Habilita o trigger
    flag = 1;
}



/*-------------------- Funções Utilizadas -----------------------*/


// Captura 8 vezes os valores de cada um dos 2 canais
void adcConfig(){

    ADC12CTL0 = 0;                  // Zera o bit de ENC
    ADC12CTL0 = ADC12SHT0_0 |       // SHT = 4 batidas de clock
                ADC12ON;            // Liga o Conversor AD

    ADC12CTL1 = ADC12CSTARTADD_0 |  // Guarda o resultado no MEM0
                ADC12SHS_0       |  // Usa o bit SC como trigger para iniciar a conversa
                ADC12SHP         |  // Usa o timer interno ADC12
                ADC12SSEL_3      |  // Usa o SMCLK como clock do ADC
                ADC12CONSEQ_1;      // Modo Sequencial

    ADC12CTL2 = ADC12TCOFF       |  // Desliga o sensor de temp (eco. energ)
                ADC12RES_2       |  // Usa resolução de 12 bits
                ADC12SR;            // Fs até 50ksps (remova para chegar a 200k samples)

    //----------------------------
    ADC12MCTL0 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_0;       // Entrada no P6.0

    ADC12MCTL1 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_1;       // Entrada no P6.1

    ADC12MCTL2 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_0;       // Entrada no P6.0

    ADC12MCTL3 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_1;       // Entrada no P6.1

    ADC12MCTL4 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_0;       // Entrada no P6.0

    ADC12MCTL5 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_1;       // Entrada no P6.1

    ADC12MCTL6 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_0;       // Entrada no P6.0

    ADC12MCTL7 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_1;       // Entrada no P6.1

    ADC12MCTL8 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_0;       // Entrada no P6.0

    ADC12MCTL9 = ADC12SREF_0     |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_1;       // Entrada no P6.1

    ADC12MCTL10 = ADC12SREF_0    |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_0;       // Entrada no P6.0

    ADC12MCTL11 = ADC12SREF_0    |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_1;       // Entrada no P6.1

    ADC12MCTL12 = ADC12SREF_0    |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_0;       // Entrada no P6.0

    ADC12MCTL13 = ADC12SREF_0    |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_1;       // Entrada no P6.1

    ADC12MCTL14 = ADC12SREF_0    |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_0;       // Entrada no P6.0

    ADC12MCTL15 = ADC12SREF_0    |  // Usa referência padrão AVSS e AVCC
                 ADC12INCH_1     |  // Entrada no P6.1
                 ADC12EOS;          // Coloca o fim da sequência
    //----------------------------

    ADC12IE    = ADC12IE15;         // Usa a interrupção do MEM15, último
    ADC12CTL0 |= ADC12ENC;          // Habilita o trigger/enable
}




void i2cConfig(){

    UCB0CTL1  = UCSWRST;                    // Reseta a Interface
    UCB0CTL0  = UCMST | UCMODE_3  | UCSYNC; // Interface é mestre, modo I2C, síncrono
    UCB0CTL1 |= UCSSEL__SMCLK;              // Seleciona o SMCLK ~1MHz
    UCB0BRW   = 11;                         // SCL ~100kHz

    //Configurar os Pinos
    P3SEL |=  (BIT0 | BIT1);                // P3.0 e P3.1 serão usado pela interface
    P3DIR &= ~(BIT0 | BIT1);                // Usar os resistores
    P3REN |=  BIT0 | BIT1;                  // Ativa resistor de pull up
    P3OUT |=  BIT0 | BIT1;

    UCB0CTL1 &= ~UCSWRST;                    // Zera os bits de RST para a interface funcionar
    return;
}

void i2cWriteByte(uint8_t addr, uint8_t byte) {
    i2cWrite(addr, &byte, 1);
}


uint8_t i2cWrite(uint8_t addr,uint8_t * data, uint8_t nBytes){

    UCB0IFG = 0;                            // Boa Prática

    UCB0I2CSA = addr;                       // Configura o endereço do escravo
    UCB0CTL1 |= UCTXSTT | UCTR;             // Requisita o início da comunicação como TX

    while(!(UCB0IFG & UCTXIFG));            // Espera o Buffer ficar livre
    UCB0TXBUF = *data++;                    // Escreve no Buffer de transmissão
    nBytes--;                               // para destravar o ciclo de ACK

    while(UCB0CTL1 & UCTXSTT);              // Espera o ciclo de ACK acontecer
    if(UCB0IFG & UCNACKIFG){                // Se o escravo não estiver na linha, vou receber um NACK
        UCB0CTL1 |= UCTXSTP;                // Preciso transitir um STOP
        while(UCB0CTL1 & UCTXSTP);          // Espero ele ser transmitido
        return 1;                           // Retorno um código de erro
    }

    while(nBytes--){
        while(!(UCB0IFG & UCTXIFG));        // Espera o Buffer ficar livre
            UCB0TXBUF = *data++;            // Escreve no Buffer de transmissão (TX)
                                            // para destravar o ciclo de ACK
    }

    while(!(UCB0IFG & UCTXIFG));            // Espera o último byte ser carregado
    UCB0CTL1 |= UCTXSTP;                    // Peço um STOP
    while(UCB0CTL1 & UCTXSTP);              // Espero ele ser transmitido

    return 0;                               // Retorna Sucesso
}


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





















