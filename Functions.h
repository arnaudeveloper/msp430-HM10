/*
 * Functions.h
 *
 *  Created on: 3 de des. 2018
 *      Author: arnau
 */
#include <msp430.h>

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

typedef unsigned char byte;


void init_UART();

void init_GPIOs();

void TxUAC1(byte TXData);

void TxUAC0_char(void);


void TxPacket();

void AT();
int AT_2(char *punter);

void AT_RESET();
int AT_RESET2(char *punter);

void AT_RENEW();

int AT_RENEW2(char *punter);

void AT_ADDR();

void AT_ROLE();






#endif /* FUNCTIONS_H_ */
