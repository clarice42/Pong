#include <msp430.h>
#include <stdint.h>

void adcConfig(){

    ADC12CTL0 = 0;                  // Zera o bit de ENC

    // Habilita o P6.0 e o P6.1 como "from module"
    P6SEL |= (BIT0 | BIT1);

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
