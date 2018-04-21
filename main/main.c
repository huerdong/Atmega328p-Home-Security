#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"

void init_io() {
	// Input enables
	DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6));
 	DDRB &= ~(1 << PB5);
	DDRC &= ~((1 << PC2) | (1 << PC3));

	// I/O Interrupts
	PCMSK0 |= 0b01111110;
	PCMSK1 |= (1 << PCINT13);
	PCMSK2 |= (1 << PCINT0) | (1 << PCINT1);

	// Timers
			

	// Modules	
	clock_init();
	lcd_init();

	// Enable global interrupts at end
	sei();
}

const unsigned char init_str[] = "System Init";
/*const unsigned char adult_1 = "Adult move: 1-2";
const unsigned char adult_2 = "Adult move: 2-1";
const unsigned char child_1 = "Child move: 1-2";
const unsigned char child_2 = "Child move: 2-1";
const unsigned char w_open[] = "Window Open";
const unsigned char w_closed[] = "Window CLosed";*/
volatile uint8_t portdhistory = 0xFF; // What does this mean - Ed
static volatile int window_open = 0;

int main() {
	lcd_clear();
	lcd_moveto(0, 0);
	lcd_stringout(init_str);	
}

ISR(TIMER1_COMPA_vect) {
}
