#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "i2c.h"
#include "clock.h"

int main() {
	DDRC |= (1 << PC0);

	lcd_init();
	lcd_moveto(0, 0);

	i2c_init(BDIV);

	struct clock_time init_time;
	init_time.hour = 2;
	init_time.min = 14;
	init_time.sec = 10;
	
	// Write data to clock
	clock_write(&init_time);

	clock_start();
	
	// Delay 5s before reading data in clock
	_delay_ms(5000);
	struct clock_time now;
	clock_read(&now);
	char buffer[20];
	int flag = print(&now, buffer, 20);		
	lcd_stringout(buffer);	

	while (1) {                 // Loop forever
		// Delay 5s before reading data in clock
		_delay_ms(5000);
		struct clock_time now;
		clock_read(&now);
		char buffer[20];
		int flag = print(&now, buffer, 20);		
		lcd_stringout(buffer);	
	}
}
