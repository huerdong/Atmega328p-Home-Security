/* 
 * Edited by Erdong Hu
 * Adapted for Atmega328p
 */

/*
 * USART_RS232_H_file.h
 * http://www.electronicwings.com
 *
 */ 


#ifndef USART_H						/* Define library H file if not defined */
#define USART_H

#include <avr/io.h>					/* Include AVR std. library file */
#include <math.h>

void USART_Init(unsigned int);				/* USART initialize function */
char USART_RxChar();					/* Data receiving function */
void USART_TxChar(char);				/* Data transmitting function */
void USART_SendString(char*);				/* Send string of USART data function */


#endif							/* USART_H */
