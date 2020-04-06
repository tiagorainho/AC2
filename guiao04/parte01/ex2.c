#include <detpic32.h>

void delay(int ms);

int main(void)
{
    LATEbits.LATE0 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE3 = 0;
    // The initial value should be set
    // before configuring the port as output
    TRISEbits.TRISE0 = 0; // RD0 configured as output
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;

    while (1)
    {
        delay(500);
        LATEbits.LATE0 = !LATEbits.LATE0;
        LATEbits.LATE1 = !LATEbits.LATE1;
        LATEbits.LATE2 = !LATEbits.LATE2;
        LATEbits.LATE3 = !LATEbits.LATE3;
    }
    return 0;
}

void delay(int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
            ;
    }
}