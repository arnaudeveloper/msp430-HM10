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
char address[32];
char parameter1;

int n_letters;

char match;
char dis_ok=FALSE;


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

  //Timer
  init_Timer();

  //GPIOs
  init_GPIOs();

  __enable_interrupt();                           //Habilitamos las interrupciones.

  //DEBUG: Falta crear una resposta per quan passa un cert temps i la UART no ha respost
  match=0;
  while(match==0)   //AT: Sembla que ho fa correcte
  {
  n_letters=AT_2(punter);
  TA0CCTL0 = CCIE;                          //Iniciem el Timer
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  match=0;
  while(match==0)   //RENEW: Sembla que ho fa correcte
  {
  n_letters=AT_RENEW2(punter);
  TA0CCTL0 = CCIE;                          //Iniciem el Timer
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  match=0;
  while(match==0)   //RESET: Sembla que ho fa correcte
  {
  n_letters=AT_RESET2(punter);
  TA0CCTL0 = CCIE;                          //Iniciem el Timer
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  match=0;
  while(match==0)   //DEBUG: Sembla que ho fa correcte
  {
  n_letters=AT_2(punter);
  TA0CCTL0 = CCIE;                          //Iniciem el Timer
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  match=0;
  while(match==0)   //IMME:
  {
  n_letters=AT_IMME2(punter);
  TA0CCTL0 = CCIE;                          //Iniciem el Timer
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  match=0;
  while(match==0)   //ROLE:
  {
  n_letters=AT_ROLE2(punter);
  TA0CCTL0 = CCIE;                          //Iniciem el Timer
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  match=0;
  //DEBUG: El timer dona mol pel cul
  TA0CCTL0 &= ~CCIE;                         // CCR0 interrupt disabled
  dis_ok=TRUE;

  while(match==0)   //DISC:
  {
  n_letters=AT_DISC(punter);
//  TA0CCTL0 = CCIE;                          //Iniciem el Timer
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  match=0;
  while(match==0)   //ROLE:
  {
  n_letters= AT_CONN(punter);
  TA0CCTL0 = CCIE;                          //Iniciem el Timer
  __bis_SR_register(LPM3_bits);   // Enter LPM0
  }

  //Configuracio completa

  SEND();   //Enviem dades





  while(1);//Bucle infinit

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
  //Amb la interrupcio capturem el packet, després sortim del mode sleep per analitzar-lo
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

      answer[j] = UCA0RXBUF;//Funciona molt bé Capturem la dada del buffer RX
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
              match = TRUE;     //Això pot donar algun FLAS POSITIU. Ex: OK+
          }
          else
          {
              match= FALSE;
          }

          j=0;  //Resetejem la j
          i=0;  //DEBUG: PROVA. No capturavem correctament la word_cap
                //DEBUG: Aquesta linia es conflictiva
          //DEBUG: En la instruccio AT+DISC? es queda penjat en aquest punt
          //        No captura el '+'.
          if(!dis_ok)
          {
              __bic_SR_register_on_exit(LPM3_bits);
          }
          break;
      }//Fi if OK

//      if(answer[0]=='O' && answer[1]=='K' && answer[2]=='+')
//      if(answer[j-2]=='O' && answer[j-1]=='K' && answer[j]=='+')	//Capturem la resposta d'una instruccio
      if(answer[0]=='+')
      {
//          if(j==2)  //Ens serveix per avaçar una posicio i guardar la dada que succeix a '+'
          if(answer[j]=='+')  //Ens serveix per avaçar una posicio i guardar la dada que succeix a '+'
          {
              j++;
              break;
          }
          else	//Capturem les dades que venen despres del +
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

                  if(checks==w)
                  {
                      match= TRUE;      //Nombre de checks IGUALS
                  }
                  else{
                      match = FALSE;    //Nombre de checks DIFERENTS
                  }

                  i=j=k=0;      //Hem acabat l'adquisicó de dades
                  memset(&answer,0, sizeof answer);     //Esborrem les dades de la variable answer
                  memset(&word_cap,0,sizeof word_cap);  //Esborrem les dades de la varaible word_cap

                  __bic_SR_register_on_exit(LPM3_bits);
                  break;
              }//--RENEW------

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
              }//--RESET---

         //--ADDR------
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
                      //Ja tenim l'adreça
                      i=j=k=0;
                  }

              }//--ADDR---
			  
		//--Set:---	  
			  if(word_cap[0]=='S' && word_cap[1]=='e' && word_cap[2]=='t' && word_cap[3]==':')
			  {
			      if(answer[j]==':')
			      {
			          j++;
			          break;
			      }
			      else
			      {
			          parameter1=answer[j];

			          if(parameter1=='1')
			          {

			              match=TRUE;

		                  i=j=k=0;
		                  parameter1=0;
		                  memset(&answer,0, sizeof answer);
		                  memset(&word_cap,0,sizeof word_cap);
		                  __bic_SR_register_on_exit(LPM3_bits);
		                  break;
			          }
			          else
			          {
			              match=FALSE;
			          }

                     i=j=k=0;
                     parameter1=0;
//                     memset(&parameter1,0, sizeof parameter1);
                     memset(&answer,0, sizeof answer);
                     memset(&word_cap,0,sizeof word_cap);
                     __bic_SR_register_on_exit(LPM3_bits);
			      }
			  }//--SET--
		//--DIS----
//			  if(word_cap[0]=='D' && word_cap[1]=='I' && word_cap[2]=='S' && word_cap[3]=='C' )//Fixat que d'aquesta manera no capturem el OK+DIS
			  if(word_cap[0]=='D' && word_cap[1]=='I' && word_cap[2]=='S')//Fixat que d'aquesta manera no capturem el OK+DIS
              {
			      if(word_cap[4]=='C')
			      {
                      if(i==4)
                      {
                          i++;
                          j++;
                          break;
                      }

	                  //Captura del Start
	                  if(word_cap[5]=='S')
	                  {
	                      //START
//	                      i=k=0;
	                      i=k=j=0;      //DEBUG: Prova
//	                      memset(&answer,0, sizeof answer);
	                      memset(&word_cap,0,sizeof word_cap);
	                      break;
	                      //DEBUG: Captura l'start pero es queda penjat un cop rep del OK
	                  }
	                  if(word_cap[5]=='E')
	                  {
	                      //END
	                      match=TRUE;
                          i=k=j=0;      //DEBUG: Prova

                          memset(&answer,0, sizeof answer);
                          memset(&word_cap,0,sizeof word_cap);
                          __bic_SR_register_on_exit(LPM3_bits);
	                      break;
	                  }
			      }
			      else
			      {
                      if(i==3)  //Ens serveix per saltar a la seguent posicio
                      {
                          i++;
                          j++;
                          break;
                      }

                      address[k]=answer[j];
                      k++;
//                      k++;
                      if(k==14)
                      {
                          //Ja tenim l'adreça
                          i=j=k=0;
                      }

			      }

              }//--DIS---

			  //--CONN---
			  if(word_cap[0]=='C' && word_cap[1]=='O' && word_cap[2]=='N' && word_cap[3]=='N')
			  {
                  if(i==4)  //Ens serveix per saltar a la seguent posicio
                   {
                       i++;
                       j++;
                       break;
                   }
                  else
                  {
                      if (word_cap[5] == 'E' || word_cap[5] == 'F')
                      {
                          //ERROR
                      }
                      match=TRUE;
                      i=k=j=0;      //DEBUG: Prova

                      memset(&answer,0, sizeof answer);
                      memset(&word_cap,0,sizeof word_cap);
                      __bic_SR_register_on_exit(LPM3_bits);
                      break;
                  }
			  }//--CONN--

          }//else que inclou totes les funcions
      }//Fi if +

      j++;  //Al final

      break;

    case 4:break;                             // Vector 4 - TXIFG

    default: break;
    }//switch

    /* SysMin will only print to the console when you call flush or exit */
   // System_flush();


  }//USCI_A0_ISR

// Pragrama per quan s'activi la interrupcio del Port 1
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    char *punter;
    punter = &word_check[0];

   P1IFG &= ~BIT1;  //Baixem la FLAG

   //P4OUT ^= BIT7;     //Canviem l'estat del LED verd Encenem/Apaguem
   //TxUAC0_char();

//   TxPacket();
//   AT();  //OK
//   AT_RESET();  //OK
//   AT_RENEW();    //OK
//    AT_ADDR();    //No captura correctament l'adreça
//    AT_ROLE();
//   AT_IMME();
//   AT_DISC();
   AT_DISC(punter);



   P1IE |= BIT1;


}

// Timer0 A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0
  TA0CTL |= TACLR;                           //  clear TAR
  TA0CCTL0 &= ~CCIE;                         // CCR0 interrupt disabled
  __bic_SR_register_on_exit(LPM3_bits);     //Exit LPM

}
