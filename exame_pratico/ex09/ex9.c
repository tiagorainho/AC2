#include <detpic32.h>

volatile int ad;
volatile int freq;

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

void putc(char byte2send)
{
    while (U1STAbits.URXDA == 1)
    {
    }
    U1TXREG = byte2send;
}

void putS(char *str)
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
    while (U1STAbits.URXDA != 0)
    {
    }
    return U1RXREG;
}

int to_bcd(int value)
{
    return ((value / 10) << 4) + value % 10;
}

void sendToDisplay(int value)
{
    TRISB = TRISB & 0x00FF;
    LATB = LATB & 0x00FF;
    TRISD = TRISD & 0xFF9F;

    static unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static int flag = 0;

    unsigned int low = display7Scodes[value & 0x0F] << 8;
    unsigned int high = display7Scodes[value >> 4] << 8;

    if (flag == 0)
    {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = LATB | low;
    }
    else
    {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = LATB | high;
    }
    flag = !flag;
}

void _int_(27) isr_adc(void)
{
    ad = ADC1BUF0;
    freq = 1 + ad / 255;
    AD1CON1bits.ASAM = 1;
    IFS1bits.AD1IF = 0;
}

void _int_(8) isr_timer2(void)
{
    sendToDisplay(to_bcd(freq));
    IFS0bits.T2IF = 0;
}

int main(void)
{
    EnableInterrupts();

    // configurar interrupcoes do adc
    IPC6bits.AD1IP = 2;
    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;

    // configurar ADC
    TRISB = TRISB | 0x0010;
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 1 - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
    AD1CON1bits.ASAM = 1;

    // configurar interrupcoes do timer
    T2CONbits.TON = 1;
    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;

    // configure timer
    T2CONbits.TCKPS = 2;
    PR2 = 49999;
    TMR2 = 0;

    // configurar switchs
    TRISB = TRISB | 0x00FF;

    // configurar Uart
    int baudrate = 1200;
    U1MODEbits.BRGH = 0;                                       // divisao por 16
    U1BRG = ((FREQ / 2 + 8 * baudrate) / (16 * baudrate)) - 1; // BRG
    U1MODEbits.PDSEL = 0;                                      // bits paridade(0 é sem paridade, 1 é par e 2 é impar)
    U1MODEbits.STSEL = 0;                                      // stop bits
    U1STAbits.UTXEN = 1;                                       // transmissao on
    U1STAbits.URXEN = 1;                                       // rececao on
    U1MODEbits.ON = 1;                                         // UART on

    while (1)
    {
        putS("RB3: ");
        putc(((PORTB & 0x0008) >> 3) + '0');
        putS(", ");
        putS("RB2: ");
        putc(((PORTB & 0x0004) >> 2) + '0');
        putS(", ");
        putS("RB1: ");
        putc(((PORTB & 0x0002) >> 1) + '0');
        putS(", ");
        putS("RB0: ");
        putc((PORTB & 0x0001) + '0');
        delay(1000 / freq);
    }
    return 1;
}
