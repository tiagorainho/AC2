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
    int freq = 2;
    LATB = LATB & 0x80FF;
    TRISB = TRISB & 0x80FF;
    LATD = (LATD & 0xFF9F) | 0x0060;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    int i;
    float t;
    unsigned int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    while (1)
    {
        t = (1 / (float)freq) * 1000;
        for (i = 0; i < 16; i++)
        {
            LATB = display7Scodes[i] << 8 | (LATB & 0x80FF);
            delay(t);
        }
    }
    return 0;
}
