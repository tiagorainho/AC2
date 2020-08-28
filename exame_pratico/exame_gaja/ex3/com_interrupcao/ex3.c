#include <detpic32.h>

volatile int continueWithProgram = 1;

void putc(char byte2send)
{
    while (U1STAbits.UTXBF == 1)
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
    while (U1STAbits.URXDA == 0)
    {
    }
    return U1RXREG;
}

void _int_(24) isr_uart(void)
{
    if (IFS0bits.U1RXIF == 1)
    {
        char key = U1RXREG;
        if (key == 'l')
        {
            putS("valor ");
            if ((PORTB & 0x000F) < 0x39)
            {
                putc((PORTB & 0x000F) + '0');
            }
            else
            {
                int v = (PORTB & 0x000F);
                v = v + 8;
                putc(v + '0');
            }

            putS(" é: ");
            if ((PORTB & 0x000F) % 2 == 0)
            {
                putS("par");
            }
            else
            {
                putS("impar");
            }
            putS("\n");
        }
        else if (key == 's')
        {
            putS("TIAGO RAINHO");
            putS("\n");
        }
        else if (key == 'q')
        {
            putS("saiu!");
            continueWithProgram = 0;
        }
        IFS0bits.U1RXIF = 0;
    }
    if (IFS0bits.U1TXIF == 1)
    {
        IFS0bits.U1TXIF = 0;
    }
}

int main(void)
{
    // configure switch
    TRISE = TRISE | 0x000F;

    // configure uart
    int baudrate = 115200;
    U1BRG = (((FREQ / 2) + 8 * baudrate) / (16 * baudrate)) - 1;
    U1MODEbits.BRGH = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;

    // interrupcao na uart
    U1STAbits.UTXSEL = 0;
    U1STAbits.URXISEL = 0;
    IEC0bits.U1RXIE = 1;
    IEC0bits.U1TXIE = 1;
    IPC6bits.U1IP = 2;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;

    EnableInterrupts();
    while (continueWithProgram == 1)
    {
    }
    return 1;
}