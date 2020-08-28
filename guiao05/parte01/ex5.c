#include <detpic32.h>
#include <p32mx795f512h.h>

void delay(unsigned int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
            ;
    }
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

void send2Displays(unsigned char value)
{
    static char displayFlag = 0;
    static unsigned int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    if (displayFlag == 0)
    {
        // configure right display
        LATD = 0xFFBF;

        // set value on right display
        LATB = (LATB & 0x00FF) | (display7Scodes[value & 0x0F] << 8);

        // turn on and off the unitary float point
        if (value % 2 != 0)
        {
            LATBbits.LATB15 = 1;
        }
        else
        {
            LATBbits.LATB15 = 0;
        }
    }
    else
    {
        // configure left display
        LATD = 0xFFDF;

        // set value on left display
        LATB = (LATB & 0x00FF) | (display7Scodes[value >> 4] << 8);

        // turn on and off the decimal float point
        if (value % 2 == 0)
        {
            LATBbits.LATB15 = 1;
        }
        else
        {
            LATBbits.LATB15 = 0;
        }
    }
    displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int main(void)
{
    TRISBbits.TRISB4 = 1;     // Porta Desligar Digital
    AD1PCFGbits.PCFG4 = 0;    // Porta Ligar Ler Analogico
    AD1CHSbits.CH0SA = 4;     // Porta a ir buscar para o multiplexer analogico
    AD1CON1bits.CLRASAM = 1;  // Precisamos de dar ordem de inicio
    AD1CON1bits.SSRC = 7;     // Trigger para inicio da conversão
    AD1CON3bits.SAMC = 16;    // Duração do tempo de amostragem
    AD1CON2bits.SMPI = 4 - 1; // Numero de BUFx que vai usar (nº conversoes)
    AD1CON1bits.ON = 1;       // Ligar a cena
    int i;
    int val;
    int c = 0;
    configureDisplay();
    while (1)
    {
        if (c++ % 25 == 0)
        {
            AD1CON1bits.ASAM = 1; // start
            while (IFS1bits.AD1IF == 0)
                ;

            val = 0;
            int *p = (int *)(&ADC1BUF0);
            for (i = 0; i < 16; i++)
            {
                val += p[i * 4];
            }
            val = val / 4;
            val = (val * 33 + 511) / 1023;
            IFS1bits.AD1IF = 0; // restart values
        }
        send2Displays(toBcd(val));
        delay(10);
    }
    return 0;
}