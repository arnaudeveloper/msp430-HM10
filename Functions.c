/*
 * Functions.c
 *
 *  Created on: 3 de des. 2018
 *      Author: arnau
 */

#include <msp430.h>
#include <string.h>
#include "Functions.h"

/* AT_Commands library*/
#include "AT_Commands.h"

/*
 * init_UART()
 */
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

/*
 * init_Timer()
 */
void init_Timer()
{
//    P1DIR |= 0x01;                            // P1.0 output
    TA0CCTL0 &= ~CCIE;                          // CCR0 interrupt disabled
    TA0CCR0 = 50000;
    TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, upmode, clear TAR
}

/*
 * init_GPIOs()
 */
void init_GPIOs()
{
    //LED1
    P1DIR |= BIT0;                            // P1.0 set as output
    P1OUT |= BIT0;                          // P1.0 ON

    //LED2
    P4DIR |= BIT7;                            // P4.7 set as output
    P4OUT &= ~BIT7;                          // P4.7 OFF

    //BUTTON 1
    P1REN |= BIT1;              // Habilitem les reisitencies internes de pull-up i pull-down del pin P1.1 (el boto)
    P1OUT |= BIT1;              //Seleccionem el mode pull-up per P1.1

    P1IE |= BIT1;               // Activem les interrupcions per P1.1
    P1IES |= BIT1;              // Indiquem el canvi de High a Low per P1.1
    P1IFG &= ~BIT1;             // Posem a zero la Flag del boto

    //BUTTON 2
    P2REN |= BIT1;              // Habilitem les reisitencies internes de pull-up i pull-down del pin P2.1 (el boto)
    P2OUT |= BIT1;              //Seleccionem el mode pull-up per P2.1

    P2IE |= BIT1;               // Activem les interrupcions per P2.1
    P2IES |= BIT1;              // Indiquem el canvi de High a Low per P2.1
    P2IFG &= ~BIT1;             // Posem a zero la Flag del boto



}

/*
 * config_INITIAL()
 * This functions is used to establish the default configuration of the HM-10,
 * set the Notifications ON and get the owner MAC address.
 * AT commands for config_INITIAL are:
 * 1. AT_RENEW
 * 2. AT_RESET
 * 3. AT_NOTI1
 * 4. AT_ADDR?
 */
void config_INITIAL()
{
    dis_ok=FALSE;
    get_address=FALSE;
    enviar = FALSE;
    estat=0;

    punter = &word_check[0];

    i=j=k=u=0;

    contador =0; //Variable per contar les adreces trobades

    //DEBUG: Falta crear una resposta per quan passa un cert temps i la UART no ha respost
    match=0;
    while(match==0)   //AT: Sembla que ho fa correcte
    {
    n_letters=AT_2(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no


    match=0;
    while(match==0)   //RENEW: Sembla que ho fa correcte
    {
    n_letters=AT_RENEW2(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no


    match=0;
    while(match==0)   //RESET: Sembla que ho fa correcte
    {
    n_letters=AT_RESET2(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no


    match=0;
    while(match==0)   //DEBUG: Sembla que ho fa correcte
    {
    n_letters=AT_2(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no

    match=0;
    while(match==0)   //DEBUG: Sembla que ho fa correcte
    {
    n_letters=AT_NOTI(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no

    match=0;
    while(match==0)   //DEBUG: Sembla que ho fa correcte
    {
    n_letters=AT_2(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no

//    TA0CCTL0 &= ~CCIE;                         // CCR0 interrupt disabled

    match=0;
    while(match==0)   //DEBUG: Sembla que ho fa correcte
    {
    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no
    n_letters=AT_ADDR(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no

}

/*
 * config_SEND()
 */
void config_SEND()
{

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no


    match=0;
    while(match==0)   //IMME:
    {
    n_letters=AT_IMME2(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no


    match=0;
    while(match==0)   //ROLE:
    {
    n_letters=AT_ROLE2(punter);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }


    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no
                                    //Prova de posar delay

    match=0;
    //DEBUG: El timer dona mol pel cul
    TA0CCTL0 &= ~CCIE;                         // CCR0 interrupt disabled
    dis_ok=TRUE;

  //  while(match==0 && get_address==0)   //DISC:
    while(match==0 || get_address==0)   //DISC:

    {
    __delay_cycles(1000000);        //DEBUG: Amb breack points si que ho fa, sense no
    n_letters=AT_DISC(punter);
  //  TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(10000);        //DEBUG: Amb breack points si que ho fa, sense no

//    match=0;
//    while(match==0)   //CONN:
//    {
//    n_letters= AT_CONN(punter);
//    TA0CCTL0 = CCIE;                          //Iniciem el Timer
//    __bis_SR_register(LPM3_bits);   // Enter LPM0
//    }

    match=0;
    while(match==0)   //CONN:
    {
    n_letters= AT_CON(punter,address2);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(500000);        //DEBUG: Amb breack points si que ho fa, sense no

    SEND();   //Enviem dades

//    match=0;
//    while(match==0)   //CONN:
//    {
    n_letters= AT_2(punter);        //Ho utilitzem per desconnectar-se
//    TA0CCTL0 = CCIE;                          //Iniciem el Timer
//    __bis_SR_register(LPM3_bits);   // Enter LPM0
//    }

    __delay_cycles(500000);        //DEBUG: Amb breack points si que ho fa, sense no

    match=0;
    while(match==0)   //CONN:
    {
    n_letters= AT_CON(punter,address3);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(500000);        //DEBUG: Amb breack points si que ho fa, sense no

    SEND();   //Enviem dades


//    match=0;
//    while(match==0)   //CONN:
//    {
    n_letters= AT_2(punter);
//    TA0CCTL0 = CCIE;                          //Iniciem el Timer
//    __bis_SR_register(LPM3_bits);   // Enter LPM0
//    }

    __delay_cycles(500000);        //DEBUG: Amb breack points si que ho fa, sense no

    match=0;
    while(match==0)   //CONN:
    {
    n_letters= AT_CON(punter,address4);
    TA0CCTL0 = CCIE;                          //Iniciem el Timer
    __bis_SR_register(LPM3_bits);   // Enter LPM0
    }

    __delay_cycles(500000);        //DEBUG: Amb breack points si que ho fa, sense no

    //Configuracio completa

    SEND();   //Enviem dades

    n_letters= AT_2(punter);

    __delay_cycles(500000);        //DEBUG: Amb breack points si que ho fa, sense no



}

/*
 * TxUAC0(byte TXData)
 */
void TxUAC0(byte TXData)
{
    while (!(UCA0IFG & UCTXIFG)); //Espera a que estigui preparat el buffer de transmissio.
    UCA0TXBUF = TXData;
}

/*
 * SEND()
 */
void SEND()
{
    byte bCount,bPacketLength;
    byte TxBuffer[15]={'#','?','M'};

    memcpy(TxBuffer+3,address1,12); //Enviem l'adresa

    bPacketLength = sizeof(TxBuffer);
    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
}

//void DISCONNECT()
//{
//    byte bCount,bPacketLength;
//    byte TxBuffer[]={'D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T'};
//
//    bPacketLength = sizeof(TxBuffer);
//    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
//    {
//        TxUAC0(TxBuffer[bCount]);
//    }
//
//    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
//}





