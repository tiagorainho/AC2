#include <detpic32.h>
#include <p32mx795f512h.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0btis.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0btis.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0btis.U1TXIE = 1
