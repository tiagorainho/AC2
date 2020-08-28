#include <detpic32.h>

// Global variable
volatile int voltage = 0;

void configureAD()
{
    // Configure all (digital I/O, analog input, A/D module)
    TRISB = (TRISB & 0xFFFC) | 0x0003;
    TRISEbits.TRISE0 = 0;
    TRISBbits.TRISB4 = 1;     // Porta Desligar volatile int aux; int aux;
    AD1PCFGbits.PCFG4 = 0;    // Porta Ligar Ler Analogico
    AD1CHSbits.CH0SA = 4;     // Porta a ir buscar para o multiplexer analogico
    AD1CON1bits.CLRASAM = 1;  // Precisamos de dar ordem de inicio
    AD1CON1bits.SSRC = 7;     // Trigger para inicio da conversão
    AD1CON3bits.SAMC = 16;    // Duração do tempo de amostragem
    AD1CON2bits.SMPI = 8 - 1; // Numero de BUFx que vai usar (nº conversoes)

    // Configure interrupt system
    IPC6bits.AD1IP = 2; // Prioridade da interrupção A/D
    IEC1bits.AD1IE = 1; // enable A/D interrupts
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    AD1CON1bits.ON = 1; // Ligar a conversor A/D
}

void send2Displays(unsigned char value)
{
    static char displayFlag = 0;
    static unsigned int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    if (displayFlag == 0)
    {
        // configure right display
        LATD = 0xFFBF;

        // set value on right display
        LATB = (LATB & 0x00FF) | (display7Scodes[value & 0x0F] << 8);
    }
    else
    {
        // configure left display
        LATD = 0xFFDF;

        // set value on left display
        LATB = (LATB & 0x00FF) | (display7Scodes[value >> 4] << 8);
    }
    displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

void configureDisplay()
{
    // configure RB8 to RB14 as outputs
    TRISB = TRISB & 0x00FF;

    // configure RD5 and RD6 as outputs
    TRISD = TRISD & 0xFF9F;

    // define first states RB8 to RB14 (default: 0)
    LATB = LATB & 0x00FF;
}

void configureTimer1()
{
    T1CONbits.TCKPS = 3; // 1:256 prescaler
    PR1 = 78125;         // Fout = 20MHz / (256 * (78125 + 1)) = 4 Hz
    TMR1 = 0;            // Reset timer T1 count register
    IPC1bits.T1IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;   // Enable timer T1 interrupts
    IFS0bits.T1IF = 0;   // Reset timer T1 interrupt flag
    T1CONbits.TON = 1;   // Enable timer T1 (must be the last command of the timer configuration sequence)
}

void configureTimer3()
{
    T3CONbits.TCKPS = 3; // 1:8 prescaler
    PR3 = 25000;         // Fout = 20MHz / (8 * (25000 + 1)) = 100 Hz
    TMR3 = 0;            // Reset timer T3 count register
    IPC3bits.T3IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T3IE = 1;   // Enable timer T3 interrupts
    IFS0bits.T3IF = 0;   // Reset timer T3 interrupt flag
    T3CONbits.TON = 1;   // Enable timer T3 (must be the last command of the timer configuration sequence)
}

void _int_(12) isr_T3(void)
{
    // Print voltage
    send2Displays(voltage);
}

void _int_(4) isr_T1(void)
{

    if (PORTBbits.RB0 == 1 && PORTBbits.RB1 == 0) // (PORTB & 0x000C) == 0x0001
    {
        // get ADC1BUF0 value
        int *p = (int *)(&ADC1BUF0);

        int i;
        int val = 0;
        for (i = 0; i < 8; i++)
        {
            val += p[i * 4];
        }
        val = val / 8;
        val = (val * 33 + 511) / 1023;

        // convert voltage amplitude to decimal and assign it to voltage
        voltage = toBcd(val);
    }

    // Start A/D conversion
    AD1CON1bits.ASAM = 1;

    // Reset T1IF
    IFS0bits.T1IF = 0;
}

int main(void)
{
    configureDisplay();
    configureAD();

    configureTimer1();
    configureTimer3();
    EnableInterrupts();

    while (1)
    {
    };
    return 0;
}
