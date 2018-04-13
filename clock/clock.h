#include "i2c.h"
#include <string.h>
#include <stdio.h>

#define SLAVE_ADDR 0xA2

struct clock_time {
	unsigned int sec;
	unsigned int min;
	unsigned int hour;
};

uint8_t rd_buf[20];
uint8_t print_buf[20];


char before (struct clock_time* a, struct clock_time* b);

unsigned char* print(struct clock_time* time);

// Code stolen from https://github.com/apc-io/apc-8750/blob/master/u-boot/rtc/pcf8563.c
unsigned bcd2bin (unsigned char n);

unsigned char bin2bcd(unsigned int n);

unsigned int clock_read(unsigned val_addr);

void clock_write(unsigned val_addr, int val);
