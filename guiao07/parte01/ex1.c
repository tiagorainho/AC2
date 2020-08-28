#include <detpic32.h>

int main(void)
{
    // Configure Timer T3 (2 Hz with interrupts disabled)
    T3CONbits.TCKPS = 7; // 1:256 prescaler (i.e Fout_presc = 78125 KHz)
    PR3 = 39060;         // Fout = 20MHz / (256 * (39060 + 1)) = 2 Hz
    TMR3 = 0;            // Reset timer T3 count register
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the timer configuration sequence)

    while (1)
    {
        // wait until T3IF = 1
        while (IFS0bits.T3IF == 0)
            ;

        // Reset T3IF
        IFS0bits.T3IF = 0;

        // Print
        putChar('.');
    }
    return 0;
}

/*
    kPrescaler = Math.ceil(20*10^6 / ((65535+1)*2)) = Math.ceil(305.17) = 152.58 -> 256 
    FoutPrescaler = PBCLK / Kprescaler = 20*10^6/256 = 78125
    PR2 = (FoutPrescaler / Fout) -1 = (78125 / 2)-1 = 39060.5
*/
