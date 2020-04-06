#include <detpic32.h>

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
    LATB = LATB & 0x80FF;
    TRISB = TRISB & 0x80FF;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    int segment;
    int i;
    while (1)
    {
        LATDbits.LATD6 = !LATDbits.LATD6;
        LATDbits.LATD5 = !LATDbits.LATD5;
        segment = 1;
        for (i = 0; i < 7; i++)
        {
            LATB = segment << 8 | (LATB & 0x80FF);
            delay(500);
            segment = segment << 1;
        }
    }
    return 0;
}
