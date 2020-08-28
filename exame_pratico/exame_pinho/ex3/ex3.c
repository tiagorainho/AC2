#include <detpic32.h>

void delay(int ms)
{
    while (ms > 0)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
        {
        }
        ms -= 1;
    }
}

int main(void)
{
    // configure ADC
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 1 - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
    AD1CON1bits.ASAM = 1;

    int v;
    while (1)
    {

        while (IFS1bits.AD1IF == 0)
        {
        }

        v = ADC1BUF0;
        v = (v * 0xF) / 1023;
        AD1CON1bits.ASAM = 1;
        printInt(v, 10);
        printStr("\n");
        delay(50);
    }

    return 1;
}