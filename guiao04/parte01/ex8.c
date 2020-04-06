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

void sendToDisplay(unsigned int value)
{
}

int main(void)
{
    unsigned int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    int freq = 5;

    // configure RB8 to RB14 and RD5 to RD6 as outputs
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;

    // define first states RB8 to RB14 and RD5 to RD6
    LATB = LATB & 0x80FF;
    LATD = (LATD & 0xFF9F) | 0x0060;

    unsigned int value;
    float t = (1 / (float)freq) * 1000;

    while (1)
    {
        // ler Port do B0 a B3
        value = PORTB & 0x000F;

        LATB = display7Scodes[value] << 8; // | (LATB & 0x80FF)
        //delay(t);
    }
    return 0;
}
