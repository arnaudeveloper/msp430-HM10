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
//    TA0CCR0 = 50000;
    TA0CCR0 = 3200;
//    TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, upmode, clear TAR
    TA0CTL = TASSEL_1 + MC_1 + TACLR;         // ACLK, upmode, clear TAR
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
    lost= FALSE;
    connection= FALSE;

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
 * This function is used to discover the master.
 * 1st. Configure module as role master.
 * 2nd. Discoverd all dispositives in a range.
 * 3rd. Try to connect to each dispositive and ask if it is a master
 *
 */
void config_SEND()
{
    char x;                             //Counter loop "for"

    __delay_cycles(10000);              // Used to pause the data streaming and give enough time to process data

    match=0;                            //Set match = 0

    /* Set IMME=1. See datasheet */
    while(match==0)                     // Resend the command is the communication fail
    {
    n_letters=AT_IMME2(punter);         // AT_IMME command
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    }

    __delay_cycles(10000);              // Used to pause the data streaming and give enough time to process data

    match=0;                            //Set match = 0

    /* Set ROLE=1. See datasheet */
    while(match==0)                     // Resend the command is the communication fail
    {
    n_letters=AT_ROLE2(punter);         // AT_ROLE command
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    }


    __delay_cycles(10000);              // Used to pause the data streaming and give enough time to process data

    match=0;                            // Set match = 0

    /* The following lines ara used to launch the AT_DISC? command
     * In that case we disable the timer and the detection of OK,
     * in order to get improve the response detection.
     * This command has a long response, so timer will interrupt
     * and resend the command
     */
    TA0CCTL0 &= ~CCIE;                  // CCR0 interrupt disabled
    dis_ok=TRUE;                        // Disabled OK detection

    while(match==0 || get_address==0)   // Resend the command is the communication fail
    {
    __delay_cycles(1000000);            // Used to pause the data streaming and give enough time to process data
    n_letters=AT_DISC(punter);          // AT_DISC? command
  //  TA0CCTL0 = CCIE;                          //Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    }

    __delay_cycles(10000);              // Used to pause the data streaming and give enough time to process data

    /* In order to use AT_CONN command. See datasheet */
//    match=0;                          // Set match = 0
//    for(x=contador;x>0;x--)
//    {
//        while(match==0)                   // Resend the command is the communication fail
//        {
//
//            n_letters= AT_CONN(punter,x-1);       //AT_CONN command
//            TA0CCTL0 = CCIE;                  //Start Timer
//            __bis_SR_register(LPM3_bits);     // Enter LPM0
//        }
//        __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data
//
//        SEND();
//        __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data
//
//        n_letters= AT_2(punter);            //Used to cut off communication
//    }


    //DEBUG: Utilitzar la variable contador per saber a quants dispositius ens hem de connectar

//    for(x=contador;x>0;x--)
//    {
//        //Codi
//    }

    //Cambiem la velocitat del rellotge per donar mes temps de resposta
    TA0CCR0 = 12800;    //PROVA, sembla que guai

    match=0;                            // Set match = 0
    x=0;


    /* Set AT_CON*/
    //DEBUG: si ho faig tot seguit sense breakpoint, envia merda
    //DEBUG: Executa dues vagades el bucle, abans de rebre la resposta
    //1er intentem establir connexio, si ho aconseguim pregutem si es el master
    while(match==0)                     // Resend the command is the communication fail
    {
        //DEBUG: WAIT FOR A RESPONSE!!
        n_letters= AT_CON(punter,address2); // AT_CON0 command. Connect to addres2
        TA0CCTL0 = CCIE;                    // Start Timer
        __bis_SR_register(LPM3_bits);       // Enter LPM0
        if(x>10)match=1;                   // Used for bad communications
        x++;

//        __delay_cycles(100000);             // Used to pause the data streaming and give enough time to process data
    }

    __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

    if(connection)      //ens serveix per detectar si hem sortit pq hem fet una connexio o pq a passat l'estona
    {
        /* Send data and disonnect*/
        SEND();
        //DEBUG: ESPERAR LA RESPOSTA
    //    TA0CCTL0 = CCIE;                    // DEBUG: Per si de cas
        __bis_SR_register(LPM3_bits);       // DEBUG: Aquesta lina es perillosa ja que si l'enviem a dormir sense estar segur que la connexio es fiable, potser no rebra mai una resposta
        if(master_detected)
        {
            memcpy(address_M, address2,12);
        }

        while(lost==0)                     // Resend the command is the communication fail
        {
            n_letters= AT_2(punter);            //Used to cut off communication
            TA0CCTL0 = CCIE;                    // Start Timer
            __bis_SR_register(LPM3_bits);       // Enter LPM0
        }
    }

    //Ho fico adalt
////    n_letters= AT_2(punter);            //Used to cut off communication
//    //DEBUG: hauriem d'esperar rebre LOST
//    while(lost==0)                     // Resend the command is the communication fail
//    {
//    n_letters= AT_2(punter);            //Used to cut off communication
//    TA0CCTL0 = CCIE;                    // Start Timer
//    __bis_SR_register(LPM3_bits);       // Enter LPM0
////    if(x>10)match=1;                   // Used for bad communications
////    x++;
//    }

    __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

    connection = FALSE;
    match=0;                            // Set match = 0
    x=0;

    while(match==0)                     // Resend the command is the communication fail
    {
    n_letters= AT_CON(punter,address3); // AT_CON0 command. Connect to addres3
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    if(x>10)match=1;                   // Used for bad communications
    x++;
    }

    __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

    /* Send data and disonnect*/
    SEND();
    n_letters= AT_2(punter);            //Used to cut off communication

    __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

    match=0;                            // Set match = 0
    x=0;

    while(match==0)                     // Resend the command is the communication fail
    {
    n_letters= AT_CON(punter,address4); // AT_CON0 command. Connect to addres4
    TA0CCTL0 = CCIE;                    // Start el Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    if(x>10)match=1;                   // Used for bad communications
    x++;
    }

    __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

    /* Send data and disonnect*/
    SEND();
    n_letters= AT_2(punter);            //Used to cut off communication

    __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data


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





