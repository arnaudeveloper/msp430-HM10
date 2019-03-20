/*
 * AT_Commands.c
 *
 *  Created on: 3 de des. 2018
 *      Author: arnaudeveloper
 *      Email: developerarnau@gmail.com
 *      License: GNU General Public License v3.0
 *      https://github.com/arnaudeveloper/msp430-HM10
 */

#include <msp430.h>
#include <AT_Commands.h>
#include "Functions.h"


/*
 * AT_2(char *punter)
 * Used to test the connection, or disconnect from other module
 * The pointer global variable is used to save data to word_check[].
 */
int AT_2(char *punter)
{
    int i=0;
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T'};
    byte word[]={'O','K'};

    /* Save word[] data to word_check[] */
    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;
}

/*
 * AT_RESET2(char *punter)
 * Used to reset the module.
 * The pointer global variable is used to save data to word_check[].
 */
int AT_RESET2(char *punter)
{
    int i;
    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','R','E','S','E','T'};

    bPacketLength = sizeof(TxBuffer);
    byte word[]={'R','E','S','E','T'};

    bPacketLength = sizeof(TxBuffer);

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;
}

/*
 * AT_RENEW2(char *punter)
 * Used to set the default configuration.
 * The pointer global variable is used to save data to word_check[].
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

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;
}

/*
 * AT_ADDR(char *punter)
 * Used to ask the MAC address.
 * The pointer global variable is used to save data to word_check[].
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

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;

}

/*
 * AT_IMME2(char *punter)
 * Used to set Imme at 1, in order to wait until an AT command.
 * The pointer global variable is used to save data to word_check[].
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

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           // Wait until last byte has been transmitted

    return i;

}

/*
 * SEND: AT+ROLE1
 * Used to set Role at 1 = master.
 * The pointer global variable is used to save data to word_check[].
 */
int AT_ROLE2(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','R','O','L','E','1'}; // Set master role.
    byte word[]={'1'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;

}

/*
 * AT_NOTI(char *punter)
 * Used to set Noti = 1. In order to receive a notification each time that a connection has been established/lost.
 * The pointer global variable is used to save data to word_check[].
 */
int AT_NOTI(char *punter)
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[]={'A','T','+','N','O','T','I','1'};  // Set notification ON
    byte word[]={'1'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;

}

/*
 * SEND: AT+DISC?
 * Used to discovery other devices in a range.
 * The pointer global variable is used to save data to word_check[].
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

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;
}

/*
 * SEND: AT+CONN0
 * Used to connect to x module.
 * The pointer global variable is used to save data to word_check[].
 * dis is the number of the device.
 */
int AT_CONN(char *punter, char dis)
{
    int i=0;
    char s[1]={dis};

    byte bCount,bPacketLength;
    byte TxBuffer[8]={'A','T','+','C','O','N','N'};
    byte word[]={'C','O','N','N'};

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    memcpy(TxBuffer+7, s, 1);                       // Merge AT command with "dis"

    bPacketLength = sizeof(TxBuffer);

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                       // Wait until last byte has been transmitted

    return i;

}

/*
 * AT_CON(char *punter,char addrex[12])
 * Used to connect to a specific MAC address.
 * The pointer global variable is used to save data to word_check[].
 * addrex[12] is the MAC address
 */
int AT_CON(char *punter,char addrex[12])
{
    int i=0;

    byte bCount,bPacketLength;
    byte TxBuffer[18]={'A','T','+','C','O','0'}; // MAC type 0
    byte word[]={'C','O','N','N'};

    memcpy(TxBuffer+6, addrex,12);              //  Merge AT command with the MAC address.

    for(i=0;i< sizeof(word);i++)
    {
        *punter=word[i];
        punter++;
    }

    bPacketLength = sizeof(TxBuffer);

    /* This loop is used to send the data */
    for(bCount=0; bCount<bPacketLength; bCount++)
    {
        TxUAC0(TxBuffer[bCount]);
    }

    while(UCA1STAT & UCBUSY);                           //Wait until last byte has been transmitted

    return i;

}


