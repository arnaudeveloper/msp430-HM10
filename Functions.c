/*
 * Functions.c
 *
 *  Created on: 3 de des. 2018
 *      Author: arnau
 */

#include <msp430.h>
#include <string.h>
#include "Functions.h"

//byte word_check;



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

void init_Timer()
{
//    P1DIR |= 0x01;                            // P1.0 output
    TA0CCTL0 &= ~CCIE;                          // CCR0 interrupt disabled
    TA0CCR0 = 50000;
    TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, upmode, clear TAR
}

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

//    match=0;
//    while(match==0)   //CONN:
//    {
//    n_letters= AT_2(punter);
//    TA0CCTL0 = CCIE;                          //Iniciem el Timer
//    __bis_SR_register(LPM3_bits);   // Enter LPM0
//    }
//
//    __delay_cycles(500000);        //DEBUG: Amb breack points si que ho fa, sense no


    //Configuracio completa

    SEND();   //Enviem dades

}

void config_INITIAL()
{
    dis_ok=FALSE;
    get_address=FALSE;
    enviar = FALSE;
    estat=0;

    punter = &word_check[0];

    i=j=k=u=0;

    contador =0; //Varaible per contar les adreces trobades

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

}

//Funcio TxUAC0(): envia una dada per la UART 0
//void TxUAC0_char(void)
//{
//    while (!(UCA0IFG & UCTXIFG)); //Espera a que estigui preparat el buffer de transmissio.
//    UCA0TXBUF = 'a';
//}
//
void TxUAC0(byte TXData)
{
    while (!(UCA0IFG & UCTXIFG)); //Espera a que estigui preparat el buffer de transmissio.
    UCA0TXBUF = TXData;
}
//
//void TxPacket()
//{
//    byte bCount,bPacketLength;
//    byte TxBuffer[2];
////    byte TxBuffer[]={'A','T','+','R','E','S','E','T'};
//    TxBuffer[0] = 'A';                                 //Primers 2 bytes que indiquen inici de la trama FF, FF.
//    TxBuffer[1] = 'T';                                 //Primers 2 bytes que indiquen inici de la trama FF, FF.
//    bPacketLength = 2;
////    bPacketLength = sizeof TxBuffer-1;
//    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
//    {
//        TxUAC0(TxBuffer[bCount]);
//    }
//
//    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
//
//
//}

void AT()
{
//    int i=0;
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T'};
//    TxBuffer[0] = 'A';                                 //Primers 2 bytes que indiquen inici de la trama FF, FF.
//    TxBuffer[1] = 'T';                                 //Primers 2 bytes que indiquen inici de la trama FF, FF.
//    byte word[]={'O','K'};
//    byte *pointer=&word_check[0];

//    for(i=0;i< sizeof(word);i++)
//    {
//        *punter=word[i];
//        punter++;
//    }

    bPacketLength = sizeof(TxBuffer);
//    bPacketLength = sizeof TxBuffer-1;
    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

//    return i;
}

/*
 * SEND: AT
 */
int AT_2(char *punter)
{
    int i=0;
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T'};
    //Primers 2 bytes que indiquen inici de la trama FF, FF.
    byte word[]={'O','K'};
//    byte *pointer=&word_check[0];

    //Escribim en la direccio del punter (word_check)
    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);
//    bPacketLength = sizeof TxBuffer-1;
    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;
}


//void AT_RESET()
//{
//    byte bCount,bPacketLength;
//    byte TxBuffer[]={'A','T','+','R','E','S','E','T'};
//
//    bPacketLength = sizeof(TxBuffer);
////    byte word_check[]={'R','E','S','E','T'};
//    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
//    {
//        TxUAC0(TxBuffer[bCount]);
//    }
//
//    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
//
////    return word_check;
//
//}

/*
 * SEND: AT+RESET
 */
int AT_RESET2(char *punter)
{
    int i;
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','R','E','S','E','T'};

    bPacketLength = sizeof(TxBuffer);
    byte word[]={'R','E','S','E','T'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;

}

//void AT_RENEW()
//{
//    byte bCount,bPacketLength;
//    byte TxBuffer[]={'A','T','+','R','E','N','E','W'};
////    byte word_check[]={'R','E','N','E','W'};
//
//    bPacketLength = sizeof(TxBuffer);
//
//    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
//    {
//        TxUAC0(TxBuffer[bCount]);
//    }
//
//    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
//
////    return &word_check;
//}

/*
 * SEND: AT+RENEW
 */
int AT_RENEW2(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','R','E','N','E','W'};
    byte word[]={'R','E','N','E','W'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;
}

/*
 * SEND: AT+ADDR?
 */
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

//void AT_IMME()
//{
////    int i=0;
//
//     byte bCount,bPacketLength;
//     byte TxBuffer[]={'A','T','+','I','M','M','E','1'};
////     byte word[]={'1'};
////
////     for(i=0;i< sizeof(word);i++)
////     {
////         *punter=word[i];
////         punter++;
////     }
//
//     bPacketLength = sizeof(TxBuffer);
//
//     for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
//     {
//         TxUAC0(TxBuffer[bCount]);
//     }
//
//     while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
//
////     return i;
//
//}

/*
 * SEND: AT+IMEE1
 */
int AT_IMME2(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','I','M','M','E','1'};
    byte word[]={'1'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;

}

void AT_ROLE()
{
//    int i=0;

     byte bCount,bPacketLength;
     byte TxBuffer[]={'A','T','+','R','O','L','E','1'}; //Definim com a Master
//     byte word[]={'1'};
//
//     for(i=0;i< sizeof(word);i++)
//     {
//         *punter=word[i];
//         punter++;
//     }

     bPacketLength = sizeof(TxBuffer);

     for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
     {
         TxUAC0(TxBuffer[bCount]);
     }

     while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

//     return i;

}

/*
 * SEND: AT+ROLE1
 */
int AT_ROLE2(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','R','O','L','E','1'}; //Definim com a Master
    byte word[]={'1'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;

}

/*
 * SEND: AT+NOTI1
 */
int AT_NOTI(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','N','O','T','I','1'}; //Definim com a Master
    byte word[]={'1'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;

}

/*
 * SEND: AT+DISC?
 */
int AT_DISC(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','D','I','S','C','?'}; //Definim com a Master
    byte word[]={'D','I','S'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;

}

/*
 * SEND: AT+CONN0
 */
int AT_CONN(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','C','O','N','N','0'}; //En connectem al dispositiu 1
    byte word[]={'C','O','N','N'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;

}

//Connect to a MAC
//Si no rep resposta vol dir que li he enviat malament
int AT_CON(char *punter,char addrex[12])
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[18]={'A','T','+','C','O','0'}; //En connectem a una MAC estil 0
    byte word[]={'C','O','N','N'};

    memcpy(TxBuffer+6, addrex,12);    //Generem l'array per enviar-lo

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    //Juntar en una sola string

    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;

}

//ok
void SEND()
{
    byte bCount,bPacketLength;
    byte TxBuffer[]={'#','?','M'};

    bPacketLength = sizeof(TxBuffer);
    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
}
void DISCONNECT()
{
    byte bCount,bPacketLength;
    byte TxBuffer[]={'D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T','D','I','S','C','O','N','N','E','C','T'};

    bPacketLength = sizeof(TxBuffer);
    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.
}




