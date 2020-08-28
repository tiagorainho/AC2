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

void configureUART(void)
{
    // 1 - Configure BaudRate Generator
    U1MODEbits.BRGH = 0; // divide for 16 (baudrate)
    U1BRG = 10;          //U1BRG = Math.ceil(((20 * 10 ^ 6) / (16 * 115200)) - 1)

    // 2 – Configure number of data bits, parity and number of stop bits
    U1MODEbits.PDSEL = 0; // parity bit turned off
    U1MODEbits.STSEL = 1; // stop bit = 1

    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U1STAbits.UTXEN = 1; // Turn on Tx (envio)
    U1STAbits.URXEN = 1; // Turn on Rx (receçao)

    // 4 – Enable UART1 (see register U1MODE)
    U1MODEbits.ON = 1;
}

int main(void)
{
    configureUART();
    while (1)
    {
        putc('+');
        delay(1000);
    }
    return 0;
}
