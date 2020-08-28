#include <detpic32.h>

volatile int dutyCycle = 0;

void configureTimer3()
{
    T3CONbits.TCKPS = 2; // 1:4 prescaler
    PR3 = 49999;         // Fout = 20MHz / (4 * (49999 + 1)) = 100 Hz
    TMR3 = 0;            // Reset timer T3 count register
    IPC3bits.T3IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1;   // Enable timer T3 interrupts
    IFS0bits.T3IF = 0;   // Reset timer T3 interrupt flag
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the timer configuration sequence)
}

void configureTimer1()
{
    T3CONbits.TCKPS = 3; // 1:256 prescaler
    PR3 = 39063;         // Fout = 20MHz / (256 * (39063 + 1)) = 2 Hz
    TMR3 = 0;            // Reset timer T1 count register
    IPC1bits.T1IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;   // Enable timer T1 interrupts
    IFS0bits.T1IF = 0;   // Reset timer T1 interrupt flag
    T1CONbits.TON = 1;   // Enable timer T1 (must be the last command of the timer configuration sequence)
}

void setPWM(unsigned int dutyCycle)
{
    OC1RS = (int)((49999 + 1) * dutyCycle) / 100;
}

void _int_(4) isr_T1(void)
{
    if (dutyCycle >= 100)
    {
        dutyCycle = 0;
    }
    else
    {
        dutyCycle++;
    }
    printf("%d\n", dutyCycle);
    setPWM(dutyCycle);

    // Reset T1IF
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
    // Reset T3IF
    IFS0bits.T3IF = 0;
}

int main(void)
{
    configureTimer1();
    configureTimer3();
    EnableInterrupts();

    OC1CONbits.OCM = 6;    // PWN no OCx
    OC1CONbits.OCTSEL = 1; // Usar o timer T3 -> 1, o T2 -> 0
    setPWM(0);
    OC1CONbits.ON = 1;
    while (1)
        ;
    return 0;
}
