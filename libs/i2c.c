#include <msp430.h>
#include <stdint.h>
#include "i2c.h"


void i2cConfig(){

    //Configurar os Pinos
    P4SEL |=  (BIT1 | BIT2);                // P4.1 e P4.2 serão usado pela interface
    P4DIR &= ~(BIT1 | BIT2);                // Usar os resistores
    P4REN |=  BIT1 | BIT2;                  // Ativa resistor de pull up
    P4OUT |=  BIT1 | BIT2;

    UCB1CTL1  = UCSWRST;                    // Reseta a Interface
    UCB1CTL0  = UCMST | UCMODE_3  | UCSYNC; // Interface é mestre, modo I2C, síncrono
    UCB1CTL1 |= UCSSEL__SMCLK;              // Seleciona o SMCLK ~1MHz
    UCB1BR0 = 64;                           // fSCL = SMCLK/64 = ~400kHz
    UCB1BR1 = 0;                            // UCBRx = (UCxxBR0 + UCxxBR1 * 256) -> fSCL = SMCLK/USBRx
    UCB1CTL1 &= ~UCSWRST;                   // Zera os bits de RST para a interface funcionar
    return;
}

void i2cWriteByte(uint8_t addr, uint8_t byte) {
    i2cWrite(addr, &byte, 1);
}


uint8_t i2cWrite(uint8_t addr,uint8_t * data, uint8_t nBytes){

    UCB1IFG = 0;                            // Boa Prática

    UCB1I2CSA = addr;                       // Configura o endereço do escravo
    UCB1CTL1 |= UCTXSTT | UCTR;             // Requisita o início da comunicação como TX

    while(!(UCB1IFG & UCTXIFG));            // Espera o Buffer ficar livre
    UCB1TXBUF = *data++;                    // Escreve no Buffer de transmissão
    nBytes--;                               // para destravar o ciclo de ACK

    while(UCB1CTL1 & UCTXSTT);              // Espera o ciclo de ACK acontecer
    if(UCB1IFG & UCNACKIFG){                // Se o escravo não estiver na linha, vou receber um NACK
        UCB1CTL1 |= UCTXSTP;                // Preciso transitir um STOP
        while(UCB1CTL1 & UCTXSTP);          // Espero ele ser transmitido
        return 1;                           // Retorno um código de erro
    }

    while(nBytes--){
        while(!(UCB1IFG & UCTXIFG));        // Espera o Buffer ficar livre
            UCB1TXBUF = *data++;            // Escreve no Buffer de transmissão (TX)
                                            // para destravar o ciclo de ACK
    }

    while(!(UCB1IFG & UCTXIFG));            // Espera o último byte ser carregado
    UCB1CTL1 |= UCTXSTP;                    // Peço um STOP
    while(UCB1CTL1 & UCTXSTP);              // Espero ele ser transmitido

    return 0;                               // Retorna Sucesso
}
