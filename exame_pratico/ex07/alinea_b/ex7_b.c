#include <detpic32.h>

volatile int v;

void delay(int ms)
{
    while (ms > 0)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
        {
        }
        ms -= 1;
    }
}

void printRBs(void)
{
    printStr("RB3: ");
    printInt((PORTB & 0x0008) >> 3, 2);

    printStr(", ");

    printStr("RB2: ");
    printInt((PORTB & 0x0004) >> 2, 2);

    printStr(", ");

    printStr("RB1: ");
    printInt((PORTB & 0x0002) >> 1, 2);

    printStr(", ");

    printStr("RB0: ");
    printInt(PORTB & 0x0001, 2);

    printStr("   potenciometro: ");
    printInt(v, 10);

    printStr("\r");
}

void _int_(27) is_isr(void)
{
    v = ADC1BUF0;
    AD1CON1bits.ASAM = 1; // start conversion
    IFS1bits.AD1IF = 0;   // flag
}

int main(void)
{
    EnableInterrupts();
    // configure switches
    TRISB = TRISB | 0x000F;

    // configure AD
    TRISB = TRISB | 0x0010;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 1 - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
    AD1CON1bits.ASAM = 1; // start conversion

    // configure interruptions
    IPC6bits.AD1IP = 2; // configurar prioridade
    IFS1bits.AD1IF = 0; // flag
    IEC1bits.AD1IE = 1; // enable

    while (1)
    {
        printRBs();
        delay(1000 / ((1 + v / 255) * 10));
    }

    return 1;
}
