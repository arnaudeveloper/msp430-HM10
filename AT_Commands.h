/*
 * AT_Commands.h
 *
 *  Created on: 4 de març 2019
 *      Author: arnau
 */
#include <msp430.h>


#ifndef AT_COMMANDS_H_
#define AT_COMMANDS_H_



int AT_2(char *punter);

int AT_RESET2(char *punter);

int AT_RENEW2(char *punter);

int AT_ADDR(char *punter);

int AT_IMME2(char *punter);

int AT_ROLE2(char *punter);

int AT_NOTI(char *punter);

int AT_DISC(char *punter);

int AT_CONN(char *punter, char dis);

int AT_CON(char *punter, char addrex[12]);


#endif /* AT_COMMANDS_H_ */
