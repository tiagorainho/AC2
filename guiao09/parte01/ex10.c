#include <detpic32.h>
#include <p32mx795f512h.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

typedef struct
{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer txb; // transmitter buffer
volatile circularBuffer rxb; // receiver buffer

void comDrv_flushRx(void)
{
    rxb.head = 0;
    rxb.tail = 0;
    rxb.count = 0;
}
void comDrv_flushTx(void)
{
    txb.head = 0;
    txb.tail = 0;
    txb.count = 0;
}

void comDrv_putc(char ch)
{
    while (txb.count == BUF_SIZE)
    {
    }
    txb.data[txb.tail] = ch;
    txb.tail = (txb.tail + 1) & INDEX_MASK;
    DisableUart1TxInterrupt();
    txb.count = txb.count + 1;
    EnableUart1TxInterrupt();
}

void comDrv_puts(char *s)
{
    while (txb.count == BUF_SIZE)
    {
    }
    while (*s != '\0')
    {
        comDrv_putc(*s);
        s++;
    }
}

char comDrv_getc(char *pchar)
{
    if (rxb.count == 0)
    {
        return 0;
    }
    // Begin of critical section
    DisableUart1RxInterrupt();
    // Copy character pointed by "head" to *pchar
    *pchar = rxb.data[rxb.head];
    // Decrement "count" variable
    rxb.count = rxb.count - 1;
    // Increment "head" variable (mod BUF_SIZE)
    rxb.head = (rxb.head + 1) & INDEX_MASK;
    //End of critical section
    EnableUart1RxInterrupt();
    return 1;
}

void _int_(24) isr_uart1(void)
{
    if (IFS0bits.U1TXIF == 1)
    {
        if (txb.count > 0)
        {
            U1TXREG = txb.data[txb.head];
            txb.head = (txb.head + 1) & INDEX_MASK;
            txb.count = txb.count - 1;
        }
        if (txb.count == 0)
        {
            DisableUart1TxInterrupt();
        }
        IFS0bits.U1TXIF = 0;
    }

    if (IFS0bits.U1RXIF == 1)
    {
        // Read character from UART and write it to the "tail" position of the reception buffer
        rxb.data[rxb.tail] = U1RXREG;

        //Increment "tail" variable(mod BUF_SIZE)
        rxb.tail = (rxb.tail + 1) & INDEX_MASK;
        //If reception buffer is not full(e.g.count < BUF_SIZE)
        if (rxb.count < BUF_SIZE)
        {
            //increment "count" variable
            rxb.count = rxb.count + 1;
        }
        else
        {
            //increment "head" variable(discard oldest character)
            rxb.head = rxb.head + 1;
        }

        //reset UART1 RX interrupt flag
        IFS0bits.U1RXIF = 0;
    }
}

void prepareInterrupts(void)
{
    U1STAbits.URXISEL = 0;
    U1STAbits.UTXSEL = 0;
    IPC6bits.U1IP = 5;
    IEC0bits.U1TXIE = 1;
    IEC0bits.U1RXIE = 1;
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
}

void comDrv_config(unsigned int baudrate, char parity, unsigned int stopBits)
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

int main(void)
{
    comDrv_config(115200, 'N', 1); // default "pterm" parameters
    // with TX and RX interrupts disabled
    comDrv_flushRx();
    comDrv_flushTx();
    EnableInterrupts();
    comDrv_puts("PIC32 UART Device-driver\n");
    char c;
    while (1)
    {
        if (comDrv_getc(&c))
        {
            comDrv_putc(c);
        }
    }
    return 1;
}
