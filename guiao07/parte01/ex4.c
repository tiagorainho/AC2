#include <detpic32.h>

void _int_(12) isr_T3(void)
{
    // Reset T3IF
    IFS0bits.T3IF = 0;

    putChar('3');
}

void _int_(4) isr_T1(void)
{
    // Reset T1IF
    IFS0bits.T1IF = 0;

    putChar('1');
}

void configureTimer1()
{
    T1CONbits.TCKPS = 3; // 1:256 prescaler
    PR1 = 39063;         // Fout = 20MHz / (256 * (39063 + 1)) = 2 Hz
    TMR1 = 0;            // Reset timer T1 count register
    IPC1bits.T1IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;   // Enable timer T1 interrupts
    IFS0bits.T1IF = 0;   // Reset timer T1 interrupt flag
    T1CONbits.TON = 1;   // Enable timer T1 (must be the last command of the timer configuration sequence)
}

void configureTimer3()
{
    T3CONbits.TCKPS = 5; // 1:32 prescaler
    PR3 = 62500;         // Fout = 20MHz / (32 * (62500 + 1)) = 10 Hz
    TMR3 = 0;            // Reset timer T3 count register
    IPC3bits.T3IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1;   // Enable timer T3 interrupts
    IFS0bits.T3IF = 0;   // Reset timer T3 interrupt flag
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the timer configuration sequence)
}

int main(void)
{
    configureTimer1();
    configureTimer3();
    EnableInterrupts();

    while (1)
        ;
    return 0;
}
