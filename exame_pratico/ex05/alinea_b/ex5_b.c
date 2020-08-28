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
    int num = 0;
    int freq = 10;
    int stickyValue = -1;
    while (1)
    {
        num = 0;
        while (num < 100)
        {
            int key = inkey();
            if (key <= 0x34 && key >= 0x30)
            {
                freq = 10 * (1 + (key - 0x30));
            }
            else if (key == '\n')
            {
                stickyValue = num;
            }
            printInt(num, 10);
            if (stickyValue >= 0)
            {
                printStr(" >> ");
                printInt(stickyValue, 10);
            }
            printStr("\r");
            num += 1;
            int period = 1000 / freq;
            delay(period);
        }
    }
    return 1;
}
