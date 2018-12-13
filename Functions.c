/*
 * Functions.c
 *
 *  Created on: 3 de des. 2018
 *      Author: arnau
 */

#include <msp430.h>
#include "Functions.h"

byte word_check;

void init_UART()
{
    P3SEL = BIT3+BIT4;  //P3.4 RX ; P3.3 TX

    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_1;
    UCA0BR0 = 0x03;
    UCA0BR1 = 0x00;
    UCA0MCTL = UCBRS_3+UCBRF_0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
}

void init_GPIOs()
{
    //LED1
    P1DIR |= BIT0;                            // P1.0 set as output
    P1OUT |= BIT0;                          // P1.0 ON

    //LED2
    P4DIR |= BIT7;                            // P4.7 set as output
    P4OUT &= ~BIT7;                          // P4.7 OFF

    //BUTTON
    P1REN |= BIT1;              // Habilitem les reisitencies internes de pull-up i pull-down del pin P1.3 (el boto)
    P1OUT |= BIT1;              //Seleccionem el mode pull-up per P1.3

    P1IE |= BIT1;               // Activem les interrupcions per P1.3
    P1IES |= BIT1;              // Indiquem el canvi de High a Low per P1.3
    P1IFG &= ~BIT1;             // Posem a zero la Flag del boto
}

//Funcio TxUAC0(): envia una dada per la UART 0
void TxUAC0_char(void)
{
    while (!(UCA0IFG & UCTXIFG)); //Espera a que estigui preparat el buffer de transmissio.
    UCA0TXBUF = 'a';
}

void TxUAC0(byte TXData)
{
    while (!(UCA0IFG & UCTXIFG)); //Espera a que estigui preparat el buffer de transmissio.
    UCA0TXBUF = TXData;
}

void TxPacket()
{
    byte bCount,bPacketLength;
    byte TxBuffer[2];
//    byte TxBuffer[]={'A','T','+','R','E','S','E','T'};
    TxBuffer[0] = 'A';                                 //Primers 2 bytes que indiquen inici de la trama FF, FF.
    TxBuffer[1] = 'T';                                 //Primers 2 bytes que indiquen inici de la trama FF, FF.
    bPacketLength = 2;
//    bPacketLength = sizeof TxBuffer-1;
    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.


}

void AT()
{
    byte bCount,bPacketLength;
    byte TxBuffer[2];
    TxBuffer[0] = 'A';                                 //Primers 2 bytes que indiquen inici de la trama FF, FF.
    TxBuffer[1] = 'T';                                 //Primers 2 bytes que indiquen inici de la trama FF, FF.
    bPacketLength = 2;
//    bPacketLength = sizeof TxBuffer-1;
    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
}

void AT_RESET()
{
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','R','E','S','E','T'};

    bPacketLength = sizeof(TxBuffer);
//    byte word_check[]={'R','E','S','E','T'};
    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

//    return word_check;

}

void AT_RENEW()
{
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','R','E','N','E','W'};
//    byte word_check[]={'R','E','N','E','W'};

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

//    return word_check;
}

//unsigned char AT_ADDR()
void AT_ADDR()
{
    byte bCount,bPacketLength;
    //unsigned char lenght=20; //8+12 de MAC

    byte TxBuffer[]={'A','T','+','A','D','D','R','?'};
    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

//    return word_check;
    //Hauriem de rotornar el nombre de Bytes que ens esperem capturar
    //8+12=20
    //return lenght

}

void AT_ROLE()
{
    byte bCount,bPacketLength;

    byte TxBuffer[]={'A','T','+','R','O','L','E','?'};
    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

}

