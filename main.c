/*
 * main.c
 *
 *  Created on: 3 de des. 2018
 *      Author: arnaudeveloper
 *      Email: developerarnau@gmail.com
 *      License: GNU General Public License v3.0
 *      https://github.com/arnaudeveloper/msp430-HM10
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
 *
 *
 */

#include <msp430.h>
/*Important Library for mem functions*/
#include <string.h>

/*HM-10 Library*/
#include <Functions.h>


/*
 * main
 * 1st. Initial config
 * 2nd. Enter in an infinite loop.
 *      In this state program is go to sleep, wake up by an interrupt timer and toggle a LED continuously.
 *      An interrupt could change the default state for transmit data, or establish connection with another device.
 *      There are 3 possible states:
 *      1. Connections mode. In this mode the module establish an stable connection with another device.
 *      2. Send data mode. In this mode the module establish connection only for send stream data, and then return to normal mode.
 *      3.Disconnect mode. This mode is used for disconnect from and established connection.
 */
int main(void)
{
    /* Stop WDT*/
    WDTCTL = WDTPW+WDTHOLD;

    /*Initialize UART to establish communication between the HM-10 and msp430*/
    init_UART();

    /* Initialize Timer for create a periodic clock to wake up the msp
    * This Timer is important because it's used in internal functions to resend AT commands
    * that have not been answered.
    * */
    init_Timer();

    /*Initialize GPIO. LEDs and Buttons. Only for test*/
    init_GPIOs();

    /* Enable interrupts*/
    __enable_interrupt();

    /*Initial config for HM-10*/
    config_INITIAL();

    /*Infinite loop*/
    while(1)
      {
          switch(estat)
          {
          case 1:
              /* In that case, return to initial configure, slave role*/
              config_INITIAL();
              estat=0;
              break;
          case 2:
              /* In that case, discover the other Bluetooth devices*/
              config_DISC();
              estat=1;
              break;
          case 3:
              /* In that case, 1st is connected to a specific MAC address and send data */
              connect_ARDU();
              send_hello();
              estat=1;
              break;
          case 4:
              /*In that case, sends the answer after analyze the question*/
              /*Analyze the 2nd position of the answer array*/
              switch (answer[2])
              {
                  case 'M': //master?

                      send_rol();       //Answer
                      break;
              }

              /*Return to normal state after answer*/
              estat=0;
              break;
          default:
              /*Default code for infinite loop*/
              TA0CCTL0 = CCIE;                  // Start Timer
              __bis_SR_register(LPM3_bits);     // Enter LPM0
              break;
          }
      }
}

//Echo back RX character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    unsigned int w;         // "for" counter
    unsigned int checks;    // Used for count the equal number of letters captured

    switch(__even_in_range(UCA0IV,4))
    {
    case 0:break;                           // Vector 0 - no interrupt
    case 2:                                 // Vector 2 - RXIFG
      while (!(UCA0IFG & UCTXIFG));         // USCI_A0 TX buffer ready?

      answer[j] = UCA0RXBUF;				//RX data captured 
											//Saved to answer[j]

      if (j > sizeof answer-1)              //If RX data received is greater than answer[], restart count.
      {
        j = 0;
      }

      /* Used to detect the owner protocol */
      if(answer[j]=='#')
      {
          answer[0]=answer[j];
          j=0;                  // Start to build the array. At the end j will increase

      }

      if(answer[0]=='#')    // Symbol to start the protocol
      {
          if(answer[1]=='?')    // Question
          {
              if(answer[2]=='M')    // Are you a master?
              {
                  /* Change "estat" to 4, for answer the question */
                  estat = 4;
              }
              //Code...
          }

          if(answer[1]=='!')    // Answer
          {
              match=TRUE;

              if(answer[2]=='M')    // It is a master
              {
                  /* It is a master, so we must to save the MAC address */
                  master_detected = TRUE;

                  i=j=k=0;      //Reset
                  memset(&answer,0, sizeof answer);     // Reset of the answer variable
                  memset(&word_cap,0,sizeof word_cap);  // Reset of the word_cap variable
                  __bic_SR_register_on_exit(LPM3_bits); // Exit LPM

              }
              //Code...
          }
          //Code...
      }// End of owner protocol

      /* Used to capture an "OK" response from HM-10 */
      if(answer[j-1]=='O' && answer[j]=='K')
      {
          if(answer[j]==word_check[1] && answer[j-1]==word_check[0])
          {
              match = TRUE;
          }
          else
          {
              match= FALSE;
          }

          i=j=0;            //Reset

          if(!dis_ok)
          {
              __bic_SR_register_on_exit(LPM3_bits);
          }
          break;
      }//End of OK


      /*
       * Used to detect the AT command response
       * Initialize the AT command response protocol
       */
      if(answer[0]=='+')
      {
          /* Used to move along and save successive data to '+' */
          if(answer[j]=='+')
          {
              j++;
              break;
          }

          /* Capturing successive data to '+'. In this bloc we analyze the capture data to identify a response*/
          else
          {
              word_cap[i]=answer[j];
              i++;

         //--RENEW------
              /* If word_cap[] is equal to "RENEW" we have a coincidence */
              if(word_cap[0]=='R' && word_cap[1]=='E' && word_cap[2]=='N' && word_cap[3]=='E' && word_cap[4]=='W')
              {
                  checks=0;     //reset

                  //Checking if is the correct answer
                  for(w=0;w<i;w++)
                  {
                      if(word_check[w]==word_cap[w])    // If word_check chain and word_cap chain are equal we have a check.
                      {
                          checks++;     //The total number of checks must be equal to the total number of letters of the word
                      }
                  }

                  if(checks==w)
                  {
                      match= TRUE;      // Total number are EQUAL
                  }
                  else
                  {
                      match = FALSE;    // Total number are DIFFERENT
                  }

                  /* The acquisition is over*/

                  i=j=k=0;      //Reset
                  memset(&answer,0, sizeof answer);     //Reset of the answer variable
                  memset(&word_cap,0,sizeof word_cap);  //Reset of the word_cap variable

                  __bic_SR_register_on_exit(LPM3_bits); //Exit of the LPM
                  break;

              }//-- End of RENEW------

         //--RESET-------
              /* If word_cap[] is equal to "RESET" we have a coincidence */
              if(word_cap[0]=='R' && word_cap[1]=='E' && word_cap[2]=='S' && word_cap[3]=='E' && word_cap[4]=='T')
              {
                  checks=0;     //reset

                  //Checking if is the correct answer
                  for(w=0;w<i;w++)
                  {
                      if(word_check[w]==word_cap[w])    // If word_check chain and word_cap chain are equal we have a check.
                      {
                          checks++;     //The total number of checks must be equal to the total number of letters of the word
                      }
                  }

                  if(checks==w)
                  {
                      match= TRUE;      // Total number are EQUAL
                  }
                  else
                  {
                      match = FALSE;    // Total number are DIFFERENT
                  }

                  /* The acquisition is over*/

                  i=j=k=0;      //Reset
                  memset(&answer,0, sizeof answer);     //Reset of the answer variable
                  memset(&word_cap,0,sizeof word_cap);  //Reset of the word_cap variable

                  __bic_SR_register_on_exit(LPM3_bits); //Exit of the LPM
                  break;

              }//--End of RESET---

         //--ADDR------
              /* If word_cap[] is equal to "ADDR" we have a coincidence */
              if(word_cap[0]=='A' && word_cap[1]=='D' && word_cap[2]=='D' && word_cap[3]=='R')
              {
                  /*Just move forward*/
                  if(i==4)
                  {
                      i++;
                      break;
                  }

                  address[k]=answer[j];

                  /* Address captured*/
                  if(k==12)
                  {
                      match=TRUE;                           //We have an address

                      memcpy(address1, &address[1], 12);    //Save owner MAC address

                      i=j=k=0;                              //Reset
                      memset(&answer,0, sizeof answer);     //Reset of the answer variable
                      memset(&word_cap,0,sizeof word_cap);  //Reset of the word_cap variable

                      __bic_SR_register_on_exit(LPM3_bits); //Exit of the LPM
                      break;
                  }

                  k++;  //one more

              }//--End of ADDR---
			  
		//--Set:---	  
              /* If word_cap[] is equal to "Set:" we have a coincidence */
			  if(word_cap[0]=='S' && word_cap[1]=='e' && word_cap[2]=='t' && word_cap[3]==':')
			  {
			      /* Just move forward*/
			      if(answer[j]==':')
			      {
			          j++;
			          break;
			      }

			      /* Capturing the parameter
			       * In that case all parameters must be 1
			       */
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

			         //Reset all parameters
                     i=j=k=0;
                     parameter1=0;
                     memset(&answer,0, sizeof answer);
                     memset(&word_cap,0,sizeof word_cap);
                     __bic_SR_register_on_exit(LPM3_bits);
			      }
			  }//--End of SET--

		//--DIS----
              /* If word_cap[] is equal to "DIS" we have a coincidence
               * In that case we have more possibilities
               * 1. DISCS: Start
               * 2. DISCE: End
               * 3. DIS0[MAC]: different values and lengths
               *
               * Example: OK+DISCSOK+DIS0:90E202020A47OK+DIS0:341513CE6FF6OK+DIS0:90E202020100OK+DISCE
               *
               * See Datasheet HM-10 for more info
               */
			  if(word_cap[0]=='D' && word_cap[1]=='I' && word_cap[2]=='S')
              {
			      /* 1rst and 2nd case*/
			      if(word_cap[4]=='C')
			      {
                      /* Move forward*/
			          if(i==4)
                      {
                          i++;
                          j++;
                          break;
                      }

			          /*Capturing the Start*/
	                  if(word_cap[5]=='S')
	                  {
	                      i=k=j=0;                                  // Reset
	                      memset(&word_cap,0,sizeof word_cap);
	                      memset(&answer,0, sizeof answer);
	                      __bic_SR_register_on_exit(LPM3_bits);
	                      break;
	                  }

	                  /*Capturing the End*/
	                  if(word_cap[5]=='E')
	                  {
	                      //END
	                      match=TRUE;   // We captured the End, so we must to finish this acquisition

	                      /* Reset and end */
	                      i=k=j=0;
                          memset(&answer,0, sizeof answer);
                          memset(&word_cap,0,sizeof word_cap);
                          __bic_SR_register_on_exit(LPM3_bits);
	                      break;
	                  }
			      }//End of 1st and 2nd case

			      /*3rd case
			       * In that case could be various values
			       * OK+DIS0:90E202020A47OK+DIS0:341513CE6FF6OK+DIS0:90E202020100
			       * */
			      else
			      {
                      /* Move forward */
			          if(i==3)
                      {
                          i++;
                          j++;
                          break;
                      }

                      /* Capturing the address */
			          address[k]=answer[j];
                      k++;

                      if(k==14)
                      {
                          /* Address captured */
                          get_address=TRUE;

                          /* Saving MAC address in a global variable*/
                          if(contador==0)
                          {
                              memcpy(address2, &address[2], 12);
                              contador++;
                          }
                          else if(contador==1)
                          {
                              memcpy(address3, &address[2], 12);
                              contador++;
                          }
                          else if(contador==2)
                          {
                              memcpy(address4, &address[2], 12);
                              contador++;
                          }
                          //Code...

                          match=FALSE;      //Only if we first captured an address we will finish the loop

                          /* Reset all*/
                          i=j=k=0;
                          memset(&answer,0, sizeof answer);
                          memset(&word_cap,0,sizeof word_cap);
                          memset(&address,0,sizeof address);
                          __bic_SR_register_on_exit(LPM3_bits);
                      }

			      }

              }//--End of DIS---

		//--CONN---
              /* If word_cap[] is equal to "CONN" we have a coincidence */
			  if(word_cap[0]=='C' && word_cap[1]=='O' && word_cap[2]=='N' && word_cap[3]=='N')
			  {
                  /*Connection established*/
			      match=TRUE;
			      connection=TRUE;

			      P4OUT |= BIT7;        //Green LED ON

                  /* Reset */
			      i=k=j=0;
                  memset(&answer,0, sizeof answer);
                  memset(&word_cap,0,sizeof word_cap);
                  __bic_SR_register_on_exit(LPM3_bits);     //exit
                  break;

			  }//--End of CONN--

		  //--LOST---
              /* If word_cap[] is equal to "LOST" we have a coincidence */
              if(word_cap[0]=='L' && word_cap[1]=='O' && word_cap[2]=='S' && word_cap[3]=='T')
              {
                  /* Connection LOST*/
                  lost = TRUE;

                  P4OUT &= ~BIT7;   //Green LED OFF

                  /* Reset */
                  i=k=j=0;
                  memset(&answer,0, sizeof answer);
                  memset(&word_cap,0,sizeof word_cap);
                  __bic_SR_register_on_exit(LPM3_bits);
                  break;

              }//--End of LOST--

          }//--End of else--
      }//--End of if + ---

      j++;  //Increment RX array position

      break;

    case 4:break;                             // Vector 4 - TXIFG

    default: break;
    }//switch

  }//USCI_A0_ISR

// Port1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
   P1IFG &= ~BIT1;      // Clear the flag
//   estat=2;             // Change state to disc
   estat=3;             // Change state to connect to static MAC
   P1IE |= BIT1;        // Enable interrupt for Button P1.1
}

// Port2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
   P2IFG &= ~BIT1;      // Clear the flag
   estat=1;             // Change state to initial
   P2IE |= BIT1;        // Enable interrupt for Button P2.1
}

// Timer0 A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0
  TA0CTL |= TACLR;                          // Clear TAR
  TA0CCTL0 &= ~CCIE;                        // CCR0 interrupt disabled
  __bic_SR_register_on_exit(LPM3_bits);     // Exit LPM

}
