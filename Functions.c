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
    TA0CCTL0 &= ~CCIE;                          // CCR0 interrupt disabled
//    TA0CCR0 = 3200;
    TA0CCR0 = 12800;
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

    pointer = &word_check[0];

    i=j=k=u=0;

    contador = 0;
    match=0;

    /* AT command */
    while(match==0)
    {
    n_letters=AT_2(pointer);
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM
    }

    __delay_cycles(10000);
    match=0;

    /* RENEW command */
    while(match==0)
    {
    n_letters=AT_RENEW2(pointer);
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM
    }

    __delay_cycles(10000);
    match=0;

    /* RESET command */
    while(match==0)
    {
    n_letters=AT_RESET2(pointer);
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM
    }

    __delay_cycles(10000);
    match=0;

    /* AT command*/
    while(match==0)
    {
    n_letters=AT_2(pointer);
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM
    }

    __delay_cycles(10000);
    match=0;

    /* NOTI command */
    while(match==0)
    {
    n_letters=AT_NOTI(pointer);
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM
    }

    __delay_cycles(10000);
    match=0;

    /* AT command*/
    while(match==0)
    {
    n_letters=AT_2(pointer);
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    }

    __delay_cycles(10000);
    match=0;

    /* ADDR command */
    while(match==0)
    {
    __delay_cycles(10000);
    n_letters=AT_ADDR(pointer);
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    }

    __delay_cycles(10000);

}

/*
 * config_DISC()
 * This function is used to discover the master.
 * 1st. Configure module as role master.
 * 2nd. Discoverd all dispositives in a range.
 * 3rd. Try to connect to each dispositive and ask if it is a master
 * 4th. If it's a master save MAC address
 * 5th. Cut off communication
 *
 */
void config_DISC()
{
    char x;                                 //Counter loop "for"

    pointer = &word_check[0];

    /* Reset all address */
    memset(&address2,0, sizeof address2);
    memset(&address3,0, sizeof address3);
    memset(&address4,0, sizeof address4);
    memset(&address_M,0, sizeof address_M);

    __delay_cycles(10000);              // Used to pause the data streaming and give enough time to process data

    match=0;                            //Set match = 0

    /* Set IMME=1. See datasheet */
    while(match==0)                     // Resend the command is the communication fail
    {
    n_letters=AT_IMME2(pointer);        // AT_IMME command
    TA0CCTL0 = CCIE;                    // Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    }

    __delay_cycles(10000);              // Used to pause the data streaming and give enough time to process data

    match=0;                            //Set match = 0

    /* Set ROLE=1. See datasheet */
    while(match==0)                     // Resend the command is the communication fail
    {
    n_letters=AT_ROLE2(pointer);        // AT_ROLE command
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
    n_letters=AT_DISC(pointer);         // AT_DISC? command
  //  TA0CCTL0 = CCIE;                          //Start Timer
    __bis_SR_register(LPM3_bits);       // Enter LPM0
    }

    __delay_cycles(5000000);              // Used to pause the data streaming and give enough time to process data

    /* In order to use AT_CONN command. See datasheet */
//    match=0;                          // Set match = 0
//    for(x=contador;x>0;x--)
//    {
//        while(match==0)                   // Resend the command is the communication fail
//        {
//
//            n_letters= AT_CONN(pointer,x-1);       //AT_CONN command
//            TA0CCTL0 = CCIE;                  //Start Timer
//            __bis_SR_register(LPM3_bits);     // Enter LPM0
//        }
//        __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data
//
//        SEND();
//        __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data
//
//        n_letters= AT_2(pointer);            //Used to cut off communication
//    }

    //TODO: Fins aqui OK.
    /* Change timer CCR to get more time for processing */
    //DEBUG: I si ho faig desdel principi?
//    TA0CCR0 = 12800;

    dis_ok=FALSE;                        // Enable OK detection

    match=FALSE;                            // Set match = 0
    x=0;
    connection = FALSE;
    lost=FALSE;
    master_detected= FALSE;


    __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

    /* Set AT_CON*/
    while(match==0)                     // Resend the command is the communication fail
    {
        n_letters= AT_CON(pointer,address2);    // AT_CON0 command. Connect to addres2
        TA0CCTL0 = CCIE;                        // Start Timer
        __bis_SR_register(LPM3_bits);           // Enter LPM0
        if(x>10)match=1;                        // Used for bad communications
        x++;
    }

    __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

    /* If connection = TRUE, a connection has been established*/
    if(connection)
    {
        /* Initiate the protocol to know if we have connected to a correct module */
        x=0;
        match=0;

        while(match==0)
        {
            SEND();
            TA0CCTL0 = CCIE;
            __bis_SR_register(LPM3_bits);
            if(x>10)match=1;                   // Used for bad communications
            x++;
        }

        /* If we have connected to the correct module x<10 */
        if(x>10)
        {
            /* We have connected to an incorrect module. Disconnect*/
            match=0;
            while(match==0 && lost==0)
            {
                pointer = &word_check[0];
                n_letters=AT_2(pointer);
                TA0CCTL0 = CCIE;
                __bis_SR_register(LPM3_bits);
            }
            /* Sometimes we recive OK before LOST.
             * If we recive OK this indicates that the connection has been cut,
             * but Green LED will be TURN-ON, so TURN-OFF
             */
            P4OUT &= ~BIT7;   //Green LED OFF
        }
        else
        {
            /* If it is a master, save the address*/
            if(master_detected)
            {
                memcpy(address_M, address2,12);
            }

            while(lost==0)                          // Resend the command is the communication fail
            {
                pointer = &word_check[0];
                __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data
                n_letters= AT_2(pointer);           // Used to cut off communication
                TA0CCTL0 = CCIE;                    // Start Timer
                __bis_SR_register(LPM3_bits);       // Enter LPM0
            }
        }
    }// End of 1st address

    match=0;                                    // Set match = 0
    x=0;
    connection = FALSE;
    lost=0;

    __delay_cycles(500000);                     // Used to pause the data streaming and give enough time to process data

    /* Set AT_CON*/
    while(match==0)                             // Resend the command is the communication fail
    {
        n_letters= AT_CON(pointer,address3);    // AT_CON0 command. Connect to addres2
        TA0CCTL0 = CCIE;                        // Start Timer
        __bis_SR_register(LPM3_bits);           // Enter LPM0
        if(x>10)match=1;                        // Used for bad communications
        x++;
    }

    __delay_cycles(500000);                     // Used to pause the data streaming and give enough time to process data

    if(connection)
    {
        x=0;
        match=0;

        while(match==0)
        {
            SEND();
            TA0CCTL0 = CCIE;
            __bis_SR_register(LPM3_bits);
            if(x>10)match=1;                   // Used for bad communications
            x++;
        }

        if(x>10)
        {
            match=0;
            while(match==0 && lost==0)
            {
                pointer = &word_check[0];
                n_letters=AT_2(pointer);
                TA0CCTL0 = CCIE;                    // Start el Timer
                __bis_SR_register(LPM3_bits);       // Enter LPM0
            }
            P4OUT &= ~BIT7;                         //Green LED OFF

        }
        else
        {
            if(master_detected)
            {
                memcpy(address_M, address3,12);
            }

            while(lost==0)                     // Resend the command if the communication fail
            {
                pointer = &word_check[0];

                __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

                n_letters= AT_2(pointer);           // Used to cut off communication
                TA0CCTL0 = CCIE;                    // Start Timer
                __bis_SR_register(LPM3_bits);       // Enter LPM0
            }
        }
    }// End of 2nd address

    match=0;                                    // Set match = 0
    x=0;
    connection = FALSE;
    lost=0;

    __delay_cycles(500000);                     // Used to pause the data streaming and give enough time to process data

    while(match==0)                             // Resend the command is the communication fail
    {
        n_letters= AT_CON(pointer,address4);    // AT_CON0 command. Connect to addres4
        TA0CCTL0 = CCIE;                        // Start Timer
        __bis_SR_register(LPM3_bits);           // Enter LPM0
        if(x>10)match=1;                        // Used for bad communications
        x++;
    }

    __delay_cycles(500000);                     // Used to pause the data streaming and give enough time to process data

    if(connection)
    {
        x=0;
        match=0;

        while(match==0)
        {
            SEND();
            TA0CCTL0 = CCIE;
            __bis_SR_register(LPM3_bits);
            if(x>10)match=1;                    // Used for bad communications
            x++;
        }

        if(x>10)
        {
            match=0;

            while(match==0 && lost==0)
            {
                pointer = &word_check[0];
                n_letters=AT_2(pointer);
                TA0CCTL0 = CCIE;                // Start el Timer
                __bis_SR_register(LPM3_bits);   // Enter LPM0
            }
            P4OUT &= ~BIT7;                     //Green LED OFF

        }
        else
        {
            if(master_detected)
            {
                memcpy(address_M, address4,12);
            }

            while(lost==0)                      // Resend the command if the communication fail
            {
                pointer = &word_check[0];

                __delay_cycles(500000);             // Used to pause the data streaming and give enough time to process data

                n_letters= AT_2(pointer);           // Used to cut off communication
                TA0CCTL0 = CCIE;                    // Start Timer
                __bis_SR_register(LPM3_bits);       // Enter LPM0
            }
        }
    }

}//End of config_DISC()

/*
 * TxUAC0(byte TXData)
 */
void TxUAC0(byte TXData)
{
    while (!(UCA0IFG & UCTXIFG));   //Wait until TX buffer will be ready
    UCA0TXBUF = TXData;
}

/*
 * SEND()
 */
void SEND()
{
    byte bCount,bPacketLength;
    byte TxBuffer[15]={'#','?','M'};

    memcpy(TxBuffer+3,address1,12);     //Send the address

    bPacketLength = sizeof(TxBuffer);
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    /* Wait until last byte has been transmited*/
    while(UCA1STAT & UCBUSY);
}

void send_rol()
{
    byte bCount,bPacketLength;
    byte TxBuffer[15]={'#','!','M'};

    memcpy(TxBuffer+3,address1,12);     //Send address

    bPacketLength = sizeof(TxBuffer);
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    /* Wait until last byte has been transmited*/
    while(UCA1STAT & UCBUSY);
}

void send_ack()
{
    byte bCount,bPacketLength;
    byte TxBuffer[15]={'#','!'};

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    /* Wait until last byte has been transmited*/
    while(UCA1STAT & UCBUSY);
}







