#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "i2c.h"
#include "clock.h"

int main() {
	lcd_init();
	lcd_moveto(0, 0);

	i2c_init(BDIV);

	struct clock_time init_time;
	init_time.hour = 14;
	init_time.min = 4;
	init_time.sec = 39;
	
	// Write data to clock
	clock_write(0x02, init_time.sec);
	clock_write(0x03, init_time.min);
	clock_write(0x04, init_time.hour);				
	
	// Delay 10s before reading data in clock
	_delay_ms(10000);
	struct clock_time now;
	now.sec = clock_read(0x02);
	now.min = clock_read(0x03);
	now.hour = clock_read(0x04);
	unsigned char* buffer = print(&now);		
	lcd_stringout_P(buffer);	

	while (1) {                 // Loop forever
	}
}
