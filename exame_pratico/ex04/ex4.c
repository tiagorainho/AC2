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

unsigned char toBCD(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

void sendToDisplay(unsigned char value)
{
    // configurar Display 7 seg
    TRISB = TRISB & 0x00FF;
    LATB = LATB & 0x00FF;

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;

    static char flip = 0;
    static unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
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
    // configurar LEDS
    TRISE = TRISE | 0xFFF0;
    int num = 0;
    int cond = 1;
    while (1)
    {
        int key = inkey();
        if (key == '0')
        {
            LATE = (LATE & 0xFFF0) | 0x1;
            num = 0;
            cond = 1;
        }
        else if (key == '1')
        {
            LATE = (LATE & 0xFFF0) | 0x2;
            num = 1;
            cond = 1;
        }
        else if (key == '2')
        {
            LATE = (LATE & 0xFFF0) | 0x4;
            num = 2;
            cond = 1;
        }
        else if (key == '3')
        {
            LATE = (LATE & 0xFFF0) | 0x8;
            num = 3;
            cond = 1;
        }
        else if (key != 0)
        {
            LATE = (LATE & 0xFFF0) | 0xF;
            int time = 0;
            // delay de 1 seg
            while (time < 100)
            {
                time++;
                sendToDisplay(0xFF);
                delay(10);
            }
            cond = 0;
            LATDbits.LATD5 = 0;
            LATDbits.LATD6 = 0;
        }
        if (cond == 1)
        {
            sendToDisplay(num);
        }
        delay(10);
    }
    return 1;
}