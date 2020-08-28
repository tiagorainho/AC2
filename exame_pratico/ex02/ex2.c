#include <detpic32.h>

void delay(int ms)
{
    while (ms != 0)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
        {
        }
        ms = ms - 1;
    }
}

int main(void)
{
    TRISE = TRISE & 0xFFF0;
    while (1)
    {
        int keyPressed = getChar();
        if (keyPressed == 0x30)
        {
            LATE = (LATE & 0xFFF0) | 0x0001;
        }
        else if (keyPressed == 0x31)
        {
            LATE = (LATE & 0xFFF0) | 0x0002;
        }
        else if (keyPressed == 0x32)
        {
            LATE = (LATE & 0xFFF0) | 0x0004;
        }
        else if (keyPressed == 0x33)
        {
            LATE = (LATE & 0xFFF0) | 0x0008;
        }
        else if (keyPressed != 0)
        {
            LATE = (LATE & 0xFFF0) | 0xF;
            delay(1000);
            LATE = LATE & 0xFFF0;
        }
    }
    return 1;
}
