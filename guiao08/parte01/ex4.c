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
    return U1RXREG;
}

void configureUART(int baudrate, char parity, unsigned int stopBits)
{
    // 1 - Configure BaudRate Generator
    U1MODEbits.BRGH = 0; // divide for 16 (baudrate)
    U1BRG = ((PBCLK + 8 * baudrate) / (16 * baudrate)) - 1;

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

    // 3 – Enable the trasmitter and receiver modules (see register U1STA)
    U1STAbits.UTXEN = 1; // Turn on Tx (envio)
    U1STAbits.URXEN = 1; // Turn on Rx (receçao)

    // 4 – Enable UART1 (see register U1MODE)
    U1MODEbits.ON = 1;
}

int main(void)
{
    configureUART(115200, 'N', 1);
    while (1)
    {
        puts("É lidar irmoum\n");
        delay(1000);
    }
    return 0;
}
