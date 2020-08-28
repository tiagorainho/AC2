#include <detpic32.h>

void delay(unsigned int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
            ;
    }
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

void _int_(VECTOR_UART1) isr_uart1(void)
{
    // If U1EIF set then
    //
    if
        overrun error then clear OERR flag
            //
            else read U1RXREG to a dummy variable
                //
                clear UART1 error interrupt flag
            //
            // If U1RXIF set then
            //
            if (U1RXREG == 'L')
            //
            (...)
    // Clear UART1 rx interrupt flag
}