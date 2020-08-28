#include <detpic32.h>
#include <p32mx795f512h.h>

void delay(unsigned int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
            ;
    }
}

// Interrupt Handler
// Replace VECTOR by the A/D vector  ###  number - see "PIC32 family data  ###  sheet" (pages 74-76)
void _int_(27) isr_adc(void)
{
    // Print ADC1BUF0 value
    int *p = (int *)(&ADC1BUF0);
    printInt(*p, 16 | 3 << 16);
    putChar(' ');

    // Start A/D conversion
    AD1CON1bits.ASAM = 1;

    // Reset AD1IF flag
    IFS1bits.AD1IF = 0;
}

int main(void)
{
    // Configure all (digital I/O, analog input, A/D module)
    TRISEbits.TRISE0 = 0;
    TRISBbits.TRISB4 = 1;     // Porta Desligar volatile int aux; int aux;
    AD1PCFGbits.PCFG4 = 0;    // Porta Ligar Ler Analogico
    AD1CHSbits.CH0SA = 4;     // Porta a ir buscar para o multiplexer analogico
    AD1CON1bits.CLRASAM = 1;  // Precisamos de dar ordem de inicio
    AD1CON1bits.SSRC = 7;     // Trigger para inicio da conversão
    AD1CON3bits.SAMC = 16;    // Duração do tempo de amostragem
    AD1CON2bits.SMPI = 1 - 1; // Numero de BUFx que vai usar (nº conversoes)

    // Configure interrupt system
    IPC6bits.AD1IP = 2; // Prioridade da interrupção A/D
    IEC1bits.AD1IE = 1; // enable A/D interrupts
    IFS1bits.AD1IF = 0; // clear A/D interrupt flag
    AD1CON1bits.ON = 1; // Ligar a conversor A/D

    // Global Interrupt Enable
    EnableInterrupts();

    // Start A/D conversion
    AD1CON1bits.ASAM = 1;

    while (1)
    {
    }
    // all activity is done by the ISR
    return 0;
}
