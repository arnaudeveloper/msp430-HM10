/*
 * AT_Commands.c
 *
 *  Created on: 4 de març 2019
 *      Author: arnau
 */

#include <msp430.h>
#include <AT_Commands.h>
#include "Functions.h"


/*
 * AT_2(char *punter)
 */
int AT_2(char *punter)
{
    int i=0;
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T'};
    byte word[]={'O','K'};

    //Escribim en la direccio del punter (word_check)
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
 * AT_RESET2(char *punter)
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

/*
 * AT_RENEW2(char *punter)
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
 * AT_ADDR(char *punter)
 */
int AT_ADDR(char *punter)
{
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','A','D','D','R','?'};
    byte word[]={'A','D','D','R'};

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
 * AT_IMME2(char *punter)
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
 * AT_NOTI(char *punter)
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
int AT_CONN(char *punter, char dis)
{
    int i=0;
    char s[1]={dis};  //DEBUG: No l'entenc?¿

    byte bCount,bPacketLength;
//    byte TxBuffer[]={'A','T','+','C','O','N','N','0'}; //En connectem al dispositiu 1
    byte TxBuffer[8]={'A','T','+','C','O','N','N'}; //En connectem al dispositiu 1
    byte word[]={'C','O','N','N'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    memcpy(TxBuffer+7, s, 1);
    bPacketLength = sizeof(TxBuffer);

    for(bCount=0; bCount<bPacketLength; bCount++)       //Aquest bucle es el que envia la trama al Modul Robot.
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Espera fins que s'hagi transmes l'ulTim byte.

    return i;

}

/*
 * AT_CON(char *punter,char addrex[12])
 */
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


