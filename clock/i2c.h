/*********************************************************************
*       at328-7.c - Demonstrate reading and writing to an I2C EEPROM
*
*       This program store some information in a 24LC256 (32k x 8)
*       I2C EEPROM and then read it back and reports the results out
*       the RS-232 interface.
*       If the button on PC1 is pressed it writes and then reads.
*       If the button is not pressed it only does the reading part.
*

* Revision History
* Date     Author      Description
* 01/28/08 A. Weber    First Release for MC68HC908JL16
* 02/12/08 A. Weber    Clean up code a bit
* 03/03/08 A. Weber    More code cleanups
* 04/22/08 A. Weber    Added "one" variable to make warning go away
* 04/14/11 A. Weber    Adapted for ATmega168
* 05/12/12 A. Weber    Modified for new I2C routines
* 11/18/13 A. Weber    Renamed for ATmega328P
* 04/01/15 A. Weber    Replaced sprintf's with snprintf's
*********************************************************************/

#include <avr/io.h>
#include <util/delay.h>

void i2c_init(uint8_t);
uint8_t i2c_io(uint8_t, uint8_t *, uint16_t,
               uint8_t *, uint16_t, uint8_t *, uint16_t);

void sci_init(uint8_t);
void sci_out(char);
void sci_outs(char *);

uint8_t rdprom(uint8_t *, uint16_t, uint16_t);
uint8_t wrprom(uint8_t *, uint16_t, uint16_t);

// Find divisors for the UART0 and I2C baud rates
#define FOSC 9830400            // Clock frequency = Oscillator freq.
#define BAUD 9600               // UART0 baud rate
#define MYUBRR FOSC/16/BAUD-1   // Value for UBRR0 register
#define BDIV (FOSC / 100000 - 16) / 2 + 1    // Puts I2C rate just below 100kHz

/* Address of the EEPROM on the I2C bus */
#define EEPROM_ADDR  0xA0

/* Address where we store test data in the EEPROM */
#define DATA_ADDR  500

uint8_t rdata[32];              // Buffer for reading data from EEPROM

#define OSTR_SIZE   80

char ostr[OSTR_SIZE];           // Buffer for creating strings to go out UART0
