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

void send2Displays(unsigned char value)
{

    unsigned int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    // right display
    LATD = 0xFFBF;
    LATB = LATB | display7Scodes[value & 0x0F] << 8;
    delay(100);

    // left display
    LATD = 0xFFDF;
    LATB = LATB & 0x00FF;
    LATB = LATB | display7Scodes[value >> 4] << 8;

    delay(100);
}

int main(void)
{
    int freq = 5;
    float t = (1 / (float)freq) * 1000;
    configureDisplay();
    unsigned int value = 0;
    while (1)
    {
        send2Displays(value);
        delay(t);
        if (value == 0xFF)
        {
            value = 0;
        }
        value = value + 1;
    }
    return 0;
}
