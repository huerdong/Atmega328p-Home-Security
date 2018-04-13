#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "i2c.h"

int main() {
	uint8_t status;
	uint8_t n;
	char *sp;

	i2c_init(BDIV);
	sci_init(MYUBRR);
	sci_outs("\r\
}
