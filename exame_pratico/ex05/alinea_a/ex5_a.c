#include <detpic32.h>

void delay(int ms)
{
    while (ms != 0)
    {
        resetCoreTimer();
        while (readCoreTimer() < 2000)
        {
        }
        ms = ms - 1;
    }
}

void sendToDisplay(int value)
{
    TRISB = TRISB & 0x00FF;
    LATB = LATB & 0x00FF;

    TRISD = TRISD & 0xFF9F;

    static char flag = 0;

    static unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    char low = display7Scodes[value & 0x0F] << 8;
    char high = display7Scodes[value >> 4] << 8;
    if (flag)
    {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = (LATB & 0xFFF0) | low;
    }
    else
    {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = (LATB & 0xFF0F) | high;
    }

    flag = !flag;
}

int main(void)
{
    // freq contagem = 10hz
    int num = 0;
    while (1)
    {
        num = 0;
        int freqCounter = 0;
        while (num < 100)
        {
            freqCounter = 0;
            while (freqCounter++ < 100)
            {
                sendToDisplay(num);
                delay(10);
            }
            printInt(num, 10);
            printStr("\r");
            num += 1;
        }
    }
    return 1;
}