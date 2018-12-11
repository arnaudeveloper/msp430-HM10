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
 *
 *  word check serveix per comprobar que el dispostiu ha rebut la comanda
 *  funcio per cridar les instruccions
 */
#include <msp430.h>
#include <stdio.h>

#include <Functions.h>

volatile static char answer[32];
volatile static char word[16];
volatile static char address[12];


//static char ack[2]={'O','K'};
//static char message;


int hex;

unsigned char i,j,k;



int main(void)
{
//  volatile unsigned int i;
    i=j=k=0;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  //UART
  init_UART();

  //GPIOs
  init_GPIOs();

//  puts("Hello");
//  printf("Hola");

//  AT();
//  AT_RENEW();
//  AT_RESET();




  __bis_SR_register(LPM3_bits + GIE);   // Enter LPM0, interrupts enabled
  __no_operation();                     // For debugger
}



//Echo back RX charcter, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV,4))
    {
    case 0:break;                             // Vector 0 - no interrupt
    case 2:                                   // Vector 2 - RXIFG
      while (!(UCA0IFG & UCTXIFG));             // USCI_A0 TX buffer ready?

      answer[j] = UCA0RXBUF;
      //if j==size of answer
      //do
     // message[j++] = UCA0RXBUF;
      if (j > sizeof answer-1)
      {
        i = 0;
        j = 0;
  //      TXBUF0 = answer[i++];
      }

      if(answer[j]=='K' && answer[j-1]=='O' ) //OK es el que es el que es rep en el prinicpi de la resposta
      {
          //Detectem l'ack "OK"
          //HM-10 esta actiu
          P4OUT ^= BIT7;                          // P4.7 OFF
      }

      if(answer[0]=='O' && answer[1]=='K' && answer[2]=='+')
      {
          if(j==2)  //Ens serveix per avaçar una posicio i guardar la dada que succeix a '+'
          {
              j++;
              break;
          }

          if(word[0]=='A' && word[1]=='D' && word[2]=='D' && word[3]=='R')
          {
              if(i==4)
              {
                  i++;
                  break;
              }
              address[k]=answer[j];
              k++;
              if(k==12)
              {
                  //Ja tenim l'adreça
                  i=j=k=0;
              }

          }
          else
          {
              word[i]=answer[j];
              i++;
          }

  //        while(1);
      }

      j++;  //Al final

      break;

    case 4:break;                             // Vector 4 - TXIFG

    default: break;
    }

    /* SysMin will only print to the console when you call flush or exit */
   // System_flush();


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
//   AT_RESET();
//   AT_RENEW();
    AT_ADDR();


   P1IE |= BIT1;


}
