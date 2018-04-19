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

int print(struct clock_time* time, char* buffer, int n) {
	memset(buffer, 0, n);	
	return sprintf(buffer, "%d:%d:%d", time->hour, time->min, time->sec);
}

unsigned bcd2bin (unsigned char n) {
	return ((((n >> 4) & 0x0F) * 10) + (n & 0x0F));
}

unsigned char bin2bcd (unsigned int n) {
	return (((n / 10) << 4) | (n % 10));
}


void clock_read(struct clock_time* t) {	
	int sec = t->sec;
	int min = t->min;
	int hour = t->hour;

	memset(rd_buf, 0, 20);
	uint8_t abuf[3] = {0x02, 0x03, 0x04};
	i2c_io(SLAVE_ADDR, abuf, 1, NULL, 0, rd_buf, 3);
	
	t->sec = bcd2bin(rd_buf[0] & 0x7F);
	t->min = bcd2bin(rd_buf[1] & 0x7F);
	t->hour = bcd2bin(rd_buf[2] & 0x3F);
}

void clock_write(struct clock_time* t) {
	uint8_t abuf[3] = {0x02, 0x03, 0x04};
	uint8_t val[3] = {bin2bcd(t->sec), bin2bcd(t->min), bin2bcd(t->hour)};
	i2c_io(SLAVE_ADDR, abuf, 3, val, 3, NULL, 0); 
}


