#include <detpic32.h>

int main(void)
{
    TRISB = TRISB | 0x000F;
    TRISE = TRISE & 0xFFF0;
    while (1)
    {
        int value = (PORTB & 0x000F);
        int v = 0;
        if ((value & 0x1) > 0)
        {
            v = v + 8;
        }
        if ((value & 0x2) > 0)
        {
            v = v + 4;
        }
        if ((value & 0x4) > 0)
        {
            v = v + 2;
        }
        if ((value & 0x8) > 0)
        {
            v = v + 1;
        }
        LATE = (LATE & 0xFFF0) | v;
    }

    return 1;
}
