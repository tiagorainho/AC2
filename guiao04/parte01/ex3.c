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
    LATB = LATB & 0x10FF;
    TRISB = TRISB & 0x10FF;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 1;

    TRISDbits.TRISD5 = 1;
    TRISDbits.TRISD6 = 0;

    char input;

    while (1)
    {
        input = getChar();
        if (input >= 0x0061 && input <= 0x0068)
        {
            LATB = input << 8 | (LATB & 0x10FF);
        }
        delay(1000);
    }
    return 0;
}
