#include <detpic32.h>

// Global variable
volatile int voltage = 0;

void prepareInterrupts(void)
{
    U1STAbits.URXISEL = 0;
    U1STAbits.UTXSEL = 0;
    IEC0bits.U1RXIE = 1;
    IEC0bits.U1TXIE = 0;
    IPC6bits.U1IP = 5;
    IFS0bits.U1RXIF = 0;
}

void configureUART(int baudrate, char parity, unsigned int stopBits)
{
    prepareInterrupts();

    // 2 – Configure number of data bits, parity and number of stop bits
    if (parity == 'N')
    {
        U1MODEbits.PDSEL = 0; // without parity
    }
    else if (parity == 'E')
    {
        U1MODEbits.PDSEL = 1; // even parity
    }
    else if (parity == 'O')
    {
        U1MODEbits.PDSEL = 2; // odd parity
    }
    U1MODEbits.STSEL = stopBits - 1; // stop bit

    // 1 - Configure BaudRate Generator
    U1MODEbits.BRGH = 0; // divide for 16 (baudrate)
    U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;

    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U1STAbits.UTXEN = 1; // Turn on Tx (envio)
    U1STAbits.URXEN = 1; // Turn on Rx (receçao)

    // 4 – Enable UART1 (see register U1MODE)
    U1MODEbits.ON = 1;
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
    PR1 = 19532 - 1;     // Fout = 20MHz / (256 * (78125 + 1)) = 4 Hz
    TMR1 = 0;            // Reset timer T1 count register
    IPC1bits.T1IP = 2;   // Interrupt priority (must be in range [1..6])
    IEC0bits.T1IE = 1;   // Enable timer T1 interrupts
    IFS0bits.T1IF = 0;   // Reset timer T1 interrupt flag
    T1CONbits.TON = 1;   // Enable timer T1 (must be the last command of the timer configuration sequence)
}

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

void configureAD()
{
    // Configure all (digital I/O, analog input, A/D module)
    TRISB = (TRISB & 0xFFFC) | 0x0003;

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

void setPWM(unsigned int dutyCycle)
{
    OC1RS = (int)((49999 + 1) * dutyCycle) / 100;
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

void _int_(27) isr_adc(void)
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

    // Reset AD1IF
    IFS1bits.AD1IF = 0;
}

void _int_(12) isr_T3(void)
{
    static int T3counter = 0;
    if (++T3counter >= 100)
    {
        putc();
        T3counter = 0;
    }

    // Reset T3IF
    IFS0bits.T3IF = 0;
}

void _int_(4) isr_T1(void)
{
    // Start A/D conversion
    AD1CON1bits.ASAM = 1;

    // Reset T1IF
    IFS0bits.T1IF = 0;
}

void configOC1(void)
{
    OC1CONbits.OCM = 6;    // PWN no OCx
    OC1CONbits.OCTSEL = 1; // Usar o timer T3 -> 1, o T2 -> 0
    setPWM(0);
    OC1CONbits.ON = 1;
}

void configureAll(void)
{
    configureDisplay();
    configureAD();
    configureTimer1();
    configureTimer3();
    EnableInterrupts();
    configOC1();
    configureUART(115200, 'N', 1);
}

void putc(char byte2send)
{
    while (U1STAbits.UTXBF == 1)
    {
    }
    U1TXREG = byte2send;
}

void puts(char *str)
{
    while (*str != '\0')
    {
        putc(*str);
        str++;
    }
}

char getc(void)
{
    if (U1STAbits.OERR == 1)
    {
        U1STAbits.OERR = 0;
    }
    while (U1STAbits.URXDA == 0)
    {
    }
    if (U1STAbits.FERR == 1 || U1STAbits.PERR == 1)
    {
        return 0;
    }
    return U1RXREG;
}

int main(void)
{
    configureAll();
    int dutyCycle;
    int inputSwitch;
    while (1)
    {
        // read RB1 and RB0
        inputSwitch = PORTB & 0x0003;
        switch (inputSwitch)
        {
        // measure input voltage
        case 0:
            // enable T1 interrupts
            T1CONbits.TON = 1;

            // LED off
            setPWM(0);
            break;
        // Freeze
        case 1:
            // Disable T1 interrupts
            T1CONbits.TON = 0;

            // LED on
            setPWM(100);
            break;
        // LED brightness control
        default:
            // Enable T1 interrupts
            T1CONbits.TON = 1;

            dutyCycle = voltage * 3;

            setPWM(dutyCycle);
            break;
        }
    };
    return 0;
}
