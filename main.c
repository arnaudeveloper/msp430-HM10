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
//----------------------------------------------------
 * Dubtes:
 * 1. Com puc saber quan la UART ha deixat de rebre dades.
 *  Per exemple un cop s'hagi deixat de rebre dades executar una funcio.
 */
#include <msp430.h>
#include <stdio.h>

#include <Functions.h>

volatile static char answer[16];
//static char ack[2]={'O','K'};
//static char message;


int hex;

char i;
unsigned char j=0;


int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  //UART
  init_UART();

  //GPIOs
  init_GPIOs();

//  puts("Hello");
//  printf("Hola");

  AT();
  AT_RENEW();
  AT_RESET();




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
//    message = UCA0RXBUF;
//    puts(&message);

    /* SysMin will only print to the console when you call flush or exit */
   // System_flush();

    answer[j++] = UCA0RXBUF;
    //j++;
    //if j==size of answer
    //do

   // message[j++] = UCA0RXBUF;
    if (j > sizeof answer-1)
    {
      i = 0;
      j = 0;
//      TXBUF0 = answer[i++];
    }

    if(answer[j-1]=='K' && answer[j-2]=='O' ) //OK es el que es el que es rep en el prinicpi de la resposta
    {
        //Detectem l'ack "OK"
        //HM-10 esta actiu
        P4OUT ^= BIT7;                          // P4.7 OFF
    }

    if(answer[j-2]=='O' && answer[j-1]=='K' && answer[j]=='+')
    {
        //codi
//        j=0;
        P1OUT ^= BIT0;                          // P4.7 OFF

//        while(1);
    }

  }

// Pragrama per quan s'activi la interrupcio del Port 1
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
   P1IFG &= ~BIT1;  //Baixem la FLAG

   //P4OUT ^= BIT7;     //Canviem l'estat del LED verd Encenem/Apaguem
   //TxUAC0_char();

//   TxPacket();
//   AT();
   AT_RESET();

   P1IE |= BIT1;


}
