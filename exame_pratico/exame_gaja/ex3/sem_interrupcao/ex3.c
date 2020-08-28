#include <detpic32.h>

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
    int key;

    while (1)
    {
        key = inkey();
        if (key == 'l')
        {
            putS("valor ");
            putc((PORTB & 0x000F) + '0');
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
            break;
        }
    }
    return 1;
}