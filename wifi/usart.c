/* 
 * Edited by Erdong Hu
 * Adapted for Atmega328p
 */

/*
 * USART_RS232_C_file.c
 * http://www.electronicwings.com
 *
 */ 

#include "usart.h"						/* Include USART header file */

void USART_Init(unsigned int ubrr)				/* USART initialize function */
{
	UCSR0A |=(1 << U2X0); 					/* Double speed */
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);	/* Enable USART transmitter and receiver, receiver interrupt enabled */
	UCSR0C |= (1 << UMSEL00)| (1 << UCSZ00) | (1 << UCSZ01);	/* Asynchronous mode for 8 bit data and 1 stop bit */ 
	UBRR0 = ubrr;						/* Load 8 bit of prescale value */
}

char USART_RxChar()							/* Data receiving function */
{
	while (!(UCSR0A & (1 << RXC0)));					/* Wait until new data receive */
	return(UDR0);									/* Get and return received data */ 
}

void USART_TxChar(char data)						/* Data transmitting function */
{
	UDR0 = data;										/* Write data to be transmitting in UDR */
	while (!(UCSR0A & (1<<UDRE0)));					/* Wait until data transmit and buffer get empty */
}

void USART_SendString(char *str)					/* Send string of USART data function */ 
{
	int i=0;																	
	while (str[i]!=0)
	{
		USART_TxChar(str[i]);						/* Send each char of string till the NULL */
		i++;
	}
}
