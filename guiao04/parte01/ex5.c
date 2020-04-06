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
    LATD = (LATD & 0xFF9F) | 0x0040;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    int segment;
    int i;
    while (1)
    {
        LATD = LATD ^ 0x0060;
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
