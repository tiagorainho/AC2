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

void sendToDisplay(int value)
{
    static unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static int flip = 0;

    TRISB = TRISB & 0x00FF;
    LATB = LATB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    unsigned int low = display7Scodes[value & 0x0F] << 8;
    unsigned int high = display7Scodes[value >> 4] << 8;

    if (flip == 0)
    {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = LATB | high;
    }
    else
    {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = LATB | low;
    }

    flip = !flip;
}

void _int_(8) isr_timer2(void)
{
    sendToDisplay(ad);
    IFS0bits.T2IF = 0;
}

int getADCvalue(void)
{
    while (IFS1bits.AD1IF == 0)
    {
    }
    return ADC1BUF0;
}

int main(void)
{
    EnableInterrupts();
    // configurar adc
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 1 - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
    AD1CON1bits.ASAM = 1;

    // configurar timer2
    T2CONbits.TCKPS = 2;
    PR2 = 41665;
    TMR2 = 0;
    T2CONbits.TON = 1;

    // interrupcoes do timer2
    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;

    while (1)
    {
        while (IFS1bits.AD1IF == 0)
        {
        }
        ad = getADCvalue();
        delay(100);
        AD1CON1bits.ASAM = 1;
    }

    return 1;
}