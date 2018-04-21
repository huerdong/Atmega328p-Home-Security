#include "i2c.h"
#include <string.h>
#include <stdio.h>

#define SLAVE_ADDR 0xA2

struct clock_time {
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
};

uint8_t rd_buf[20];

void clock_start();

char before (struct clock_time* a, struct clock_time* b);

int print(struct clock_time* time, char* buffer, int n);

// Code stolen from https://github.com/apc-io/apc-8750/blob/master/u-boot/rtc/pcf8563.c
unsigned bcd2bin (unsigned char n);

unsigned char bin2bcd(unsigned int n);

void clock_read(struct clock_time* time);

void clock_write(struct clock_time* time);
