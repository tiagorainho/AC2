#include <detpic32.h>
#include <p32mx795f512h.h>

void delay(unsigned int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
            ;
    }
}

void configureDisplay()
{
    // configure RB8 to RB14 as outputs
    TRISB = TRISB & 0x80FF;

    // configure RD5 and RD6 as outputs
    TRISD = TRISD & 0xFF9F;

    // define first states RB8 to RB14 (default: 0)
    LATB = LATB & 0x80FF;
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

void send2Displays(unsigned char value)
{
    static char displayFlag = 0;
    static unsigned int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    if (displayFlag == 0)
    {
        // configure right display
        LATD = 0xFFBF;

        // set value on right display
        LATB = (LATB & 0x00FF) | (display7Scodes[value & 0x0F] << 8);
    }
    else
    {
        // configure left display
        LATD = 0xFFDF;

        // set value on left display
        LATB = (LATB & 0x00FF) | (display7Scodes[value >> 4] << 8);
    }
    displayFlag = !displayFlag;
}

int main(void)
{
    configureDisplay();
    int value = 0;
    int i = 0;
    while (1)
    {
        i++;
        send2Displays(toBcd(value));
        delay(10);
        if (value == 0x3C)
        {
            value = 0;
        }
        if (i % 20 == 0)
        {
            value++;
        }
    }
    return 0;
}
