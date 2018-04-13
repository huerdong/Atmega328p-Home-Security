#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

// Port definitions
#define LCD_D7_PORT	PORTD
#define LCD_D7_BIT	PD7
#define LCD_D7_DDR	DDRD

#define LCD_D6_PORT	PORTB
#define LCD_D6_BIT	PB4
#define LCD_D6_DDR	DDRB

#define LCD_D5_PORT	PORTB
#define LCD_D5_BIT	PB3
#define LCD_D5_DDR	DDRB

#define LCD_D4_PORT	PORTB
#define LCD_D4_BIT	PB2
#define LCD_D4_DDR	DDRB

#define LCD_RS_PORT     PORTB
#define LCD_RS_BIT	PB0
#define LCD_RS_DDR	DDRB

#define LCD_E_PORT	PORTB
#define LCD_E_BIT	PB1
#define LCD_E_DDR	DDRB

// LCD instructions
#define LCD_CLEAR		0x01	// replace all characters with ASCII 'space'
#define LCD_HOME		0x02	// return cursor to first position on first line
#define LCD_FUNCTIONRESET	0x30	// reset the LCD
#define LCD_FUNCTIONSET4BIT	0x28	// 4-bit data, 2-line display, 5 x 7 font
#define LCD_SETCURSOR		0x80	// set cursor position
#define LCD_DISPLAYON		0x0c	// Display on
#define LCD_DISPLAYOFF		0x80	// Display off
#define LCD_ENTRYMODE		0x06	// Entrymode

void lcd_init(void);
void lcd_moveto(unsigned char, unsigned char);
void lcd_stringout(char *);
void lcd_writecommand(unsigned char);
void lcd_writedata(unsigned char);
void lcd_writebyte(unsigned char);
void lcd_writenibble(unsigned char);
void lcd_wait(void);
void lcd_stringout_P(char *);


