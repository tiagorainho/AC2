#include <detpic32.h>

void sendToDisplay(int value)
{
    static unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static int flip = 0;

    // configurar displays
    TRISB = TRISB & 0x00FF;
    LATB = LATB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    unsigned int low = display7Scodes[value & 0x0F] << 8;
    unsigned int high = display7Scodes[value >> 4] << 8;

    if (flip == 0)
    {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = LATB | low;
    }
    else
    {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = LATB | high;
    }
    flip = !flip;
}

int main(void)
{
    // configurar switch
    TRISB = TRISB | 0x000F;

    // configurar switch
    TRISE = TRISE & 0xFFF0;

    while (1)
    {
        LATE = (LATE & 0xFFF0) | (PORTB & 0x000F);
        sendToDisplay(PORTB & 0x000F);
    }
    return 1;
}