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
    TRISBbits.TRISB4 = 1;     // Porta Desligar Digital
    AD1PCFGbits.PCFG4 = 0;    // Porta Ligar Ler Analogico
    AD1CHSbits.CH0SA = 4;     // Porta a ir buscar para o multiplexer analogico
    AD1CON1bits.CLRASAM = 1;  // Precisamos de dar ordem de inicio
    AD1CON1bits.SSRC = 7;     // Trigger para inicio da conversão
    AD1CON3bits.SAMC = 16;    // Duração do tempo de amostragem
    AD1CON2bits.SMPI = 1 - 1; // Numero de BUFx que vai usar (nº conversoes)
    AD1CON1bits.ON = 1;       // Ligar a cena
    int i;
    while (1)
    {
        delay(500);
        AD1CON1bits.ASAM = 1; // start
        while (IFS1bits.AD1IF == 0)
            ;

        int *p = (int *)(&ADC1BUF0);
        for (i = 0; i < 16; i++)
        {
            printInt(p[i * 4], 16 | 4 << 16);
            putChar(' ');
        }
        IFS1bits.AD1IF = 0; // restart values
        putChar(',');
        printStr("\n");
    }
    return 0;
}