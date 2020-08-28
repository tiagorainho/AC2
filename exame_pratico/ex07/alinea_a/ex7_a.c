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
    TRISB = TRISB | 0x000F;
    LATB = LATB & 0xFFF0;
    while (1)
    {
        printStr("RB3: ");
        printInt((PORTB & 0x0008) >> 3, 2);

        printStr(", ");

        printStr("RB2: ");
        printInt((PORTB & 0x0004) >> 2, 2);

        printStr(", ");

        printStr("RB1: ");
        printInt((PORTB & 0x0002) >> 1, 2);

        printStr(", ");

        printStr("RB0: ");
        printInt(PORTB & 0x0001, 2);

        printStr("\r");
        delay(10);
    }

    return 1;
}
