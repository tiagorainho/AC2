#include <detpic32.h>
#include <p32mx795f512h.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0btis.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0btis.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0btis.U1TXIE = 1

#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE – 1)

typedef struct
{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

volatile circularBuffer txb; // transmitter buffer
volatile circularBuffer rxb; // receiver buffer