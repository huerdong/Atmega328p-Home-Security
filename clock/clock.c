#include "clock.h"

char before (struct clock_time* a, struct clock_time* b) {
	if (a->hour != b->hour) {
		return a->hour < b->hour;
	}
	if (a->min != b->min) {
		return a->min < b->min;
	}
	return a->sec < b->sec;
}

unsigned char* print(struct clock_time* time) {
	memset(print_buf, 0, 20);	
	int n = sprintf(print_buf, "%d:%d%d", time->hour, time->min, time->sec);
	return print_buf;
}

unsigned bcd2bin (unsigned char n) {
	return ((((n >> 4) & 0x0F) * 10) + (n & 0x0F));
}

unsigned char bin2bcd (unsigned int n) {
	return (((n / 10) << 4) | (n % 10));
}

unsigned int clock_read(unsigned val_addr) {
	memset(rd_buf, 0, 20);
	uint8_t abuf = {val_addr};
	i2c_io(SLAVE_ADDR, abuf, 1, NULL, 0, rd_buf, 20);
	return bcd2bin(rd_buf[0]); 
}

void clock_write(unsigned val_addr, int val) {
	write_byte(SLAVE_ADDR, val_addr, bin2bcd(val));	
}


