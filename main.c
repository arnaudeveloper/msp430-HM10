/*
 * main.c
 *
 *  Created on: 3 de des. 2018
 *      Author: arnau
 *
 *
 *
 *
 *
 *                 MSP430F5529
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |     P3.3/UCA0TXD|------------>
//            |                 | 9600 - 8N1
//            |     P3.4/UCA0RXD|<------------
//
//
 */
#include <msp430.h>

#include <Functions.h>

static char string1[8];
static char ack[2]={'O','K'};
static char message[];


int hex;

char i;
char j=0;


int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  //UART
  init_UART();

  //GPIOs
  init_GPIOs();




  __bis_SR_register(LPM3_bits + GIE);   // Enter LPM0, interrupts enabled
  __no_operation();                     // For debugger
}



//Echo back RX charcter, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
//    switch(UCA0RXBUF){
//        case '1':
//        P1OUT |=BIT0;
//        //_delay_cycles(200000000);
//        break;
//        case '0':
//        P1OUT &= ~BIT0;
//        //_delay_cycles(200000000);
//        break;
//    }

    string1[j++] = UCA0RXBUF;
   // message[j++] = UCA0RXBUF;
    if (j > sizeof string1-1)
    {
      i = 0;
      j = 0;
//      TXBUF0 = string1[i++];
    }

    if(string1[j-1]=='K' && string1[j-2]=='O' ) //OK es el que es el que es rep en el prinicpi de la resposta
    {
        //Detectem l'ack "OK"
        //HM-10 esta actiu
        P4OUT ^= BIT7;                          // P4.7 OFF
    }

  }

// Pragrama per quan s'activi la interrupcio del Port 1
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
   P1IFG &= ~BIT1;  //Baixem la FLAG

   //P4OUT ^= BIT7;     //Canviem l'estat del LED verd Encenem/Apaguem
   //TxUAC0_char();

   TxPacket();

   P1IE |= BIT1;


}
