#include <detpic32.h>

void _int_(12) isr_T3(void)
{
    // static variable so that its values doesnt get lost between calls
    static int counter = 0;

    // Reset T3IF
    IFS0bits.T3IF = 0;

    if (counter >= 1)
    {
        putChar('.');
        counter = 0;
    }
    else
    {
        counter++;
    }
}

int main(void)
{
    // Configure Timer T3 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 7; // 1:256 prescaler (i.e Fout_presc = 78125 KHz)
    PR3 = 39060;         // Fout = 20MHz / (256 * (39060 + 1)) = 2 Hz
    TMR3 = 0;            // Reset timer T3 count register
    IPC3bits.T3IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1;   // Enable timer T2 interrupts
    IFS0bits.T3IF = 0;   // Reset timer T2 interrupt flag
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the timer configuration sequence)

    EnableInterrupts();

    while (1)
        ;
    return 0;
}
