#include <detpic32.h>
#include <math.h>

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

void configureTimer3(int fout)
{
    T3CONbits.TON = 0; // disable timer
    // get prescaler
    int prescalerValues[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    int Kprescaler = ceil(FREQ / 2) / ((65535 + 1) * fout);
    int i = 0;
    for (i = 0; i < 9; i++)
    {
        if (Kprescaler < prescalerValues[i])
        {
            Kprescaler = prescalerValues[i];
            break;
        }
    }
    T3CONbits.TCKPS = Kprescaler; // prescaler

    // get the others
    PR3 = ((FREQ / 2) / Kprescaler) / fout;
    TMR3 = 0;
    T3CONbits.TON = 1; // enable timer
}

void _int_(12) is_isr(void)
{
    static int num = 0;
    int key = inkey();
    if (key <= 0x34 && key >= 0x30)
    {
        configureTimer3(10 * (1 + key % 0x30));
    }
    printInt(num, 10);
    printStr("\r");
    num += 1;
    if (num >= 100)
    {
        num = 0;
    }
    IFS0bits.T3IF = 0; // interrupt flag
}

int main(void)
{
    configureTimer3(10);
    // configurar interrupcoes
    IPC3bits.T3IP = 2; // interrupt priority
    IEC0bits.T3IE = 1; // interrupt enable
    IFS0bits.T3IF = 0; // interrupt flag
    EnableInterrupts();

    while (1)
    {
    }
    return 1;
}
