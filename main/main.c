#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "clock.h"
#include "lcd.h"

// Global variables
static volatile uint8_t prev_pind;
static volatile uint8_t window_open;
static volatile uint8_t direction; // 0: 1->2 1: 2->1, treat 0 as "in" and 1 as "out"
static volatile uint8_t height; // 0: child, 1: adult
static volatile struct clock_time now;

//pd2, pd3, pd4, pd5 = IR sensor inputs (pd2 = high1, pd3 = high2, pd4 = low1, pd5 = low2)
//pd6 = MAG sensor input
//pb5, pc3 = LED output
//pc2 = buzzer output
void init_io() {
	// Input enables
	DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6));
 	DDRB |= (1 << PB5);
	DDRC |= ((1 << PC2) | (1 << PC3));

	// Pull resistors 
	PORTD |= ((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6));

	// I/O Interrupts
	PCICR |= 0b00000100;
	PCMSK2 |= 0b01111100;

	// Timers and clock

	// Modules	
	lcd_init();

	// Default values for global variables
	prev_pind = PIND;
	window_open = 0;
	direction = 0; 
	height = 0;

	// Enable global interrupts at end
	sei();
}

const unsigned char init_str[] = "System Init    ";
const unsigned char adult_1[] =  "Adult move: 1-2";
const unsigned char adult_2[] =  "Adult move: 2-1";
const unsigned char child_1[] =  "Child move: 1-2";
const unsigned char child_2[] =  "Child move: 2-1";
const unsigned char w_open[] =   "Window Open    ";
const unsigned char w_closed[] = "Window Closed  ";

int main() {
	init_io();

	lcd_clear();
	lcd_moveto(0, 0);
	lcd_stringout(init_str);	

	while(1);
}

ISR(PCINT2_vect) {
	cli();
	
	uint8_t changed_pind = PIND ^ prev_pind;	
	
	uint8_t through1 = (PIND & (1 << PD2)) | (PIND & (1 << PD4));
	uint8_t through2 = (PIND & (1 << PD3)) | (PIND & (1 << PD5));

	// Magnetic sensor
	if (changed_pind & (1 << PD6)) {
		if ((PIND & (1 << PD6) == 0)) {
			window_open = 0;
			lcd_moveto(1, 0);
			lcd_stringout(w_closed);
		} else {
			window_open = 1;
			lcd_moveto(1, 0);
			lcd_stringout(w_open);		
		}	
	}

	
	
	if ((changed_pind & (1 << PD2)) | (changed_pind & (1 << PD3)) | 
		(changed_pind & (1 << PD4)) | (changed_pind & (1 << PD5))) {
		uint8_t passing = 0;
		uint8_t prev_passing = 0;
		int i;
		for (i = 2; i <= 5; ++i) {
			passing |= (PIND & (1 << i));
			prev_passing |= (prev_pind & (1 << i));
		}

		height |= (PIND & (1 << PD2)) | (PIND & (1 << PD3)); 

		if (passing != 0 && prev_passing == 0) {
			// Rising edge, starting to pass through set direction
			if((PIND & (1 << PD3)) | (PIND & (1 << PD5))) {
				direction = 1;	
			} 
			else {
				direction = 0;
			}	
		} 
		else if (passing == 0 && prev_passing != 0) {
			// Flush lcd
			lcd_moveto(1, 0);			
			lcd_stringout("               ");
			lcd_moveto(1, 0);

			if (height == 0) {
				if (direction == 0) {
					lcd_stringout(child_1);
				}
				else {
					lcd_stringout(child_2);
				}			
			}
			else {
				if (direction == 0) {
					lcd_stringout(adult_1);
				}
				else {
					lcd_stringout(adult_2);
				}
			}		

			height = 0;
			direction = 0;			
		}	
	}

	// Update prev_pind
	prev_pind = PIND;

	sei();
}

ISR(TIMER1_COMPA_vect) {
	cli();



	sei();
}
