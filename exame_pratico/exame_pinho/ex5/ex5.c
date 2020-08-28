#include <detpic32.h>

volatile int ad;

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

void _int_(27) isr_adc(void)
{
    while (IFS1bits.AD1IF == 0)
    {
    }

    ad = ADC1BUF0;
    ad = (ad * 0xF) / 1023;
    AD1CON1bits.ASAM = 1;
    IFS1bits.AD1IF = 0;
}

int main(void)
{
    EnableInterrupts();
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

    IPC6bits.AD1IP = 2;
    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;

    while (1)
    {

        printInt(ad, 10);
        printStr("\n");
        delay(50);
    }

    return 1;
}