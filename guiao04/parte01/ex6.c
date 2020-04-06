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
    int freq = 10;
    LATB = LATB & 0x80FF;
    TRISB = TRISB & 0x80FF;
    LATD = (LATD & 0xFF9F) | 0x0040;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    int segment, i;
    float t;
    while (1)
    {
        t = (1 / (float)freq) * 1000;
        LATD = LATD ^ 0x0060;
        segment = 1;
        for (i = 0; i < 7; i++)
        {
            LATB = segment << 8 | (LATB & 0x80FF);
            delay(t);
            segment = segment << 1;
        }
    }
    return 0;
}
