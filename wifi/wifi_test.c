/*
* ATmega16_WIFI
* http://www.electronicwings.com
*
*/


#include "wifi.h"				/* Include USART header file */

int main(void)
{
	DDRC |= (1 << PC0);
	PORTC &= ~(1 << PC0);

	PORTC |= (1 << PC0);
	_delay_ms(2000);
	PORTC &= ~(1 << PC0);
	
	int flag = 0;
	char _buffer[150];
	uint8_t Connect_Status;

	USART_Init(MYUBRR);						/* Initiate USART with 115200 baud rate */
	_delay_ms(1000);
	PORTC |= (1 << PC0);
	_delay_ms(1000);
	PORTC &= ~(1 << PC0);

	sei();									/* Start global interrupt */

	while(!ESP8266_Begin()) {
		PORTC |= (1 << PC0);
		_delay_ms(1000);
		PORTC &= ~(1 << PC0);
		_delay_ms(1000);
	}
	_delay_ms(1000);
	PORTC |= (1 << PC0);
	_delay_ms(1000);
	PORTC &= ~(1 << PC0);
	
	ESP8266_WIFIMode(BOTH_STATION_AND_ACCESPOINT);/* 3 = Both (AP and STA) */
	ESP8266_ConnectionMode(SINGLE);			/* 0 = Single; 1 = Multi */
	ESP8266_ApplicationMode(NORMAL);		/* 0 = Normal Mode; 1 = Transperant Mode */
	if(ESP8266_connected() == ESP8266_NOT_CONNECTED_TO_AP)
	{
		PORTC |= (1 << PC0);
		_delay_ms(1000);
		PORTC &= ~(1 << PC0);		
		ESP8266_JoinAccessPoint(SSID, PASSWORD);
	}
	ESP8266_Start(0, DOMAIN, PORT);
	while(1)
	{
		memset(_buffer, 0, 150);
		sprintf(_buffer, "GET /line");
		ESP8266_Send(_buffer);
		Read_Data(_buffer);
	}
}
