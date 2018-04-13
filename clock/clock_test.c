#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "i2c.h"

int main() {
	lcd_init();
	lcd_moveto(0, 0);

	uint8_t status;
	uint8_t n;
	char *sp;

	i2c_init(BDIV);

	// Buffer for writing time
	char wt_buffer[40];

	// Buffer for reading time
	char rd_buffer[40];

	// Delay 10s before reading data in clock
	_delay_ms(10000);	
	lcd_stringout_P(buffer);	

	while (1) {                 // Loop forever
	}
}
