/*
 * Functions.h
 *
 *  Created on: 3 de des. 2018
 *      Author: arnau
 */
#include <msp430.h>

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define TRUE 1
#define FALSE 0

typedef unsigned char byte;

char answer[32];        //Used for capture RX packets from HM-10

char word_cap[8];       //Used for capture successive data to '+' in an AT response

char word_check[8];     //Used for compare the answered and the desired/correct answer from HM-10.
                        //If are not equal we resend the AT command.

char address[32];       //Used for capture the MAC address

char address1[12];      // Owner MAC address

char address2[12];      // MAC address 1

char address3[12];      // MAC address 2

char address4[12];      // MAC address 3

char match;             // Used for matching a desired/correct answer

char get_address;       // Used for inform of the acquisition of a MAC address
                        // Combined with match is used to control the resend of the AT command

unsigned int i,j,k,u;   // Variables used to count
                        // i: Successive data to '+' count. (Always the AT response is like: OK+[DATA]. Except in AT alone commmand.
                        // j: RX data count
                        // k: Successive data to ADDR. Used for capture and save MAC address
                        // u

char parameter1;        // Used to capture the data parameter of a SET response

char estat;             // Used to save the state.

int n_letters;

//char dis_ok=FALSE;
//char get_address=FALSE;
//char enviar = FALSE;

char dis_ok;            // Used for disable the detection of OK??



char enviar;

char *punter;


unsigned int contador;  // Used for count the number of MAC address captured



void init_UART();

void init_Timer();

void init_GPIOs();

void config_INITIAL();

void config_SEND();

void SEND();
//void DISCONNECT();

void TxUAC0(byte TXData);



#endif /* FUNCTIONS_H_ */
