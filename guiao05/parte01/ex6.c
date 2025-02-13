#include <detpic32.h>
#include <p32mx795f512h.h>

void delay(unsigned int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
            ;
    }
}

int main(void)
{
    volatile int aux;
    TRISEbits.TRISE0 = 0;
    TRISBbits.TRISB4 = 1;     // Porta Desligar Digital
    AD1PCFGbits.PCFG4 = 0;    // Porta Ligar Ler Analogico
    AD1CHSbits.CH0SA = 4;     // Porta a ir buscar para o multiplexer analogico
    AD1CON1bits.CLRASAM = 1;  // Precisamos de dar ordem de inicio
    AD1CON1bits.SSRC = 7;     // Trigger para inicio da conversão
    AD1CON3bits.SAMC = 32;    // Duração do tempo de amostragem
    AD1CON2bits.SMPI = 1 - 1; // Numero de BUFx que vai usar (nº conversoes)
    AD1CON1bits.ON = 1;       // Ligar a cena
    int *p = (int *)(&ADC1BUF0);
    while (1)
    {
        LATEbits.LATE0 = 1;   // E0 = 1
        AD1CON1bits.ASAM = 1; // start conversion
        while (IFS1bits.AD1IF == 0)
            ;
        LATEbits.LATE0 = 0; // E0 = 0
        aux = *p;           // ler do buffer de resultados
        IFS1bits.AD1IF = 0; // reset value
    }
    return 0;
}