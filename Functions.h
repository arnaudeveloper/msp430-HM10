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

char answer[32];
char word_cap[8];
char word_check[8];
char address[32];
char parameter1;
char estat;

int n_letters;

char match;

//char dis_ok=FALSE;
//char get_address=FALSE;
//char enviar = FALSE;
char dis_ok;
char get_address;
char enviar;

char *punter;

unsigned int i,j,k;



void init_UART();

void init_Timer();

void init_GPIOs();

void config_INITIAL();

void config_SEND();

//void TxUAC1(byte TXData);
//
//void TxUAC0_char(void);
//
//
//void TxPacket();

//--AT COMMANDS---
void AT();
int AT_2(char *punter);

//void AT_RESET();
int AT_RESET2(char *punter);

//void AT_RENEW();

int AT_RENEW2(char *punter);

void AT_ADDR();

//void AT_IMME();
int AT_IMME2(char *punter);

//void AT_ROLE();
int AT_ROLE2(char *punter);

int AT_NOTI(char *punter);

int AT_DISC(char *punter);


//void AT_ROLE();

int AT_CONN(char *punter);

void SEND();
void DISCONNECT();



#endif /* FUNCTIONS_H_ */
