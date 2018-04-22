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
	init_time.hour = 11;
	init_time.min = 51;
	init_time.sec = 20;

	// Write data to clock
	//clock_write(&init_time);

	struct clock_time now;
	clock_read(&now);
	char buffer[20];

	while (1) {                 // Loop forever
		_delay_ms(1000);
		struct clock_time now;
		clock_read(&now);
		char buffer[20];
		int flag = print(&now, buffer, 20);		
		lcd_writecommand(LCD_CLEAR);
		lcd_moveto(0, 0);
		lcd_stringout(buffer);	
	}
}
