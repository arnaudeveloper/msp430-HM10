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
 *  ----------------------------------------------------
 *  Instruccions AT
 *  1. AT -> OK
 *  2. AT+RENEW -> OK+RENEW
 *  3. AT+RESET -> OK+RESET
 *  4. AT+IMME[P1] -> OK+Set:[P1]
 *  5. AT+ROLE[P1] -> OK+Set:[P1]
 *  6. AT+DISC? -> OK+DIS[P0][P1]
 *  7. AT+CON[MAC] -> OK+CONNA
 *  8.AT+ADDR? -> OK+ADDR[MAC]
 *
 */
#include <msp430.h>
#include <stdio.h>
#include <string.h>

#include <Functions.h>

#define TRUE 1
#define FALSE 0

//volatile static char answer[32];
//volatile static char word[8];
//volatile static char word_check[8];
//volatile static char address[12];
//volatile static char parameter1[1];

char answer[32];
char word_cap[8];
char word_check[8];
char address[12];
char parameter1[1];

int n_letters;

char match;


//static char ack[2]={'O','K'};
//static char message;


//int hex;

//unsigned char i,j,k;
unsigned int i,j,k;



int main(void)
{
//  volatile unsigned int i;

    char *punter;
    punter = &word_check[0];

  i=j=k=0;
//  volatile static char *puntero = &word_check[0];

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  //UART
  init_UART();

  //GPIOs
  init_GPIOs();

  __enable_interrupt();                           //Habilitamos las interrupciones.

  //DEBUG: Falta crear una resposta per quan passa un cert temps i la UART no ha respost
  match=0;
  while(match==0)   //DEBUG: Sembla que ho fa correcte
  {
  n_letters=AT_2(punter);
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

//  AT_RENEW();
//  __bis_SR_register(LPM3_bits);   // Enter LPM0


  match=0;
  while(match==0)   //RENEW: Sembla que ho fa correcte
  {
  n_letters=AT_RENEW2(punter);
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  match=0;
  while(match==0)   //RESET: Sembla que ho fa correcte
  {
  n_letters=AT_RESET2(punter);
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  //DEBUG: Fa correcte la inicialitzacio!!

//  match = FALSE;
//  while(!match)
//  {
//      n_letters=AT(punter);
//      __bis_SR_register(LPM3_bits);
//  }
//  AT_RENEW();
//  __bis_SR_register(LPM3_bits);   // Enter LPM0. Esperem a la interrupcio de la UART
//  AT_RESET();




//  __bis_SR_register(LPM3_bits + GIE);   // Enter LPM0, interrupts enabled
  __bis_SR_register(LPM3_bits);   // Enter LPM0

  __no_operation();                     // For debugger
  //Aqui s'ha d'afegir un codi per poder analitzar el packet
  //Amb la interrupcio capturem el packet, despr�s sortim del mode sleep per analitzar-lo
//  __bis_SR_register(LPM0_bits);   // Enter LPM0
//  while(1)
//  {
//      //hola
//      P4OUT ^= BIT7;                          // P4.7 OFF
//
//  }
}



//Echo back RX charcter, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    unsigned int w, checks;

    switch(__even_in_range(UCA0IV,4))
    {
    case 0:break;                             // Vector 0 - no interrupt
    case 2:                                   // Vector 2 - RXIFG
      while (!(UCA0IFG & UCTXIFG));             // USCI_A0 TX buffer ready?

      answer[j] = UCA0RXBUF;//Funciona molt b� Capturem la dada del buffer RX
      //if j==size of answer
      //do
     // message[j++] = UCA0RXBUF;
      if (j > sizeof answer-1)
      {
        j = 0;
  //      TXBUF0 = answer[i++];
      }

      //if(j>packet_lenght-1)
//      {
          //guarda-ho en el buffer
//      }

      if(answer[j-1]=='O' && answer[j]=='K') //OK es el que es el que es rep en el prinicpi de la resposta
      {
          //Detectem l'ack "OK"
          //HM-10 esta actiu

          P4OUT ^= BIT7;                          // P4.7 OFF

          //DEBUG: Aqui passa algo raro...Pero el match=1
          if(answer[j]==word_check[1] && answer[j-1]==word_check[0])
          {
              match = TRUE;
          }
          else
          {
              match= FALSE;
          }
          j=0;  //Resetejem la j
          __bic_SR_register_on_exit(LPM3_bits);
          break;
      }

//      if(answer[0]=='O' && answer[1]=='K' && answer[2]=='+')
//      if(answer[j-2]=='O' && answer[j-1]=='K' && answer[j]=='+')	//Capturem la resposta d'una instruccio
      if(answer[0]=='+')
      {
//          if(j==2)  //Ens serveix per ava�ar una posicio i guardar la dada que succeix a '+'
          if(answer[j]=='+')  //Ens serveix per ava�ar una posicio i guardar la dada que succeix a '+'
          {
              j++;
              break;
          }
          else
          {

              word_cap[i]=answer[j];
              i++;


         //--RENEW------
              if(word_cap[0]=='R' && word_cap[1]=='E' && word_cap[2]=='N' && word_cap[3]=='E' && word_cap[4]=='W')
              {
                  checks=0;
                  //TENIM UNA COINCIDENCIA => Executem bucle de comprobacio
                  for(w=0;w<i;w++)
                  {
                      if(word_check[w]==word_cap[w])    //Si la cadena word_check i word tenen el mateix valor tenim check!
                      {
                          checks++;     //El nombre de checks total ha de ser igual al nombre de lletres de la paraula
                      }
                  }
    //
                  if(checks==w)
                  {
                      match= TRUE;      //Nombre de checks IGUALS
                  }
                  else{
                      match = FALSE;    //Nombre de checks DIFERENTS
                  }

                  i=j=k=0;      //Hem acabat l'adquisic� de dades
                  memset(&answer,0, sizeof answer);     //Esborrem les dades de la variable answer
                  memset(&word_cap,0,sizeof word_cap);  //Esborrem les dades de la varaible word_cap

                  __bic_SR_register_on_exit(LPM3_bits);
                  break;
              }

         //--RESET-------
    //          if(word[0]=='R' && word[1]=='E' && word[2]=='S' && word[3]=='E' && word[4]=='T')
              if(word_cap[0]=='R' && word_cap[1]=='E' && word_cap[2]=='S' && word_cap[3]=='E' && word_cap[4]=='T')
              {
                  //Codi
                  /*
                   * for per comprobar que la word check i la word son iguals, tamany del for igual a la i-1
                   */
                  checks=0;
                  for(w=0;w<i;w++)
                  {
                      if(word_check[w]==word_cap[w])
                      {
                          checks++;
                      }
                  }
                  if(checks==w)
                  {
                      match= TRUE;
                  }
                  else{
                      match = FALSE;
                  }
                  i=j=k=0;
                  memset(&answer,0, sizeof answer);
                  memset(&word_cap,0,sizeof word_cap);
                  __bic_SR_register_on_exit(LPM3_bits);

                  break;
              }


              //ADDR
              if(word_cap[0]=='A' && word_cap[1]=='D' && word_cap[2]=='D' && word_cap[3]=='R')
    //          if(word=="0x00242C")
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
                      //Ja tenim l'adre�a
                      i=j=k=0;
                  }

              }

    //          if(word[0]=='G' && word[1]=='E' && word[2]=='T')
    //          {
    //              if(i==3)
    //              {
    //                  i++;
    //                  break;
    //              }
    //              parameter1[k]=answer[j]; //Ja tenim el parametre
    //              i=j=k=0;
    //          }

    //          else
    //          {
    //              word[i]=answer[j];
    //              i++;
    //          }

      //        while(1);
          }
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
//   AT();  //OK
//   AT_RESET();  //OK
   AT_RENEW();    //OK
//    AT_ADDR();    //No captura correctament l'adre�a
//    AT_ROLE();


   P1IE |= BIT1;


}
