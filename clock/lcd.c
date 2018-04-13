#include "lcd.h"

/*
  lcd_stringout_P - Print the contents of the character string "s" starting at LCD
  RAM location "x" where the string is stored in ROM.  The string must be
  terminated by a zero byte.
*/
void lcd_stringout_P(char *s)
{
    unsigned char ch;

    /* Use the "pgm_read_byte()" routine to read the date from ROM */
    while ((ch = pgm_read_byte(s++)) != '\0') {
        lcd_writedata(ch);             // Output the next character
    }
}

/*
  lcd_init - Do various things to force a initialization of the LCD
  display by instructions, and then set up the display parameters and
  turn the display on.
*/
void lcd_init(void)
{
    LCD_D7_DDR |= (1 << LCD_D7_BIT);
    LCD_D6_DDR |= (1 << LCD_D6_BIT);
    LCD_D5_DDR |= (1 << LCD_D5_BIT);
    LCD_D4_DDR |= (1 << LCD_D4_BIT);
    LCD_RS_DDR |= (1 << LCD_RS_BIT);
    LCD_E_DDR |= (1 << LCD_E_BIT);

    LCD_RS_PORT &= ~(1 << LCD_RS_BIT);
    LCD_E_PORT &= ~(1 << LCD_E_BIT);

    _delay_ms(15);              // Delay at least 15ms

    // Reset the LCD controller
    lcd_writenibble(LCD_FUNCTIONRESET);      
    _delay_ms(5);              
    lcd_writenibble(LCD_FUNCTIONRESET); 
    _delay_us(120);             
    lcd_writenibble(LCD_FUNCTIONRESET);
    _delay_ms(2);
   
    // Set to 4-bit mode 
    lcd_writenibble(LCD_FUNCTIONSET4BIT);
    _delay_ms(2);

    // Function Set instruction
    lcd_writecommand(LCD_FUNCTIONSET4BIT);   
    
    // Initialize display
    lcd_writecommand(LCD_DISPLAYOFF);    
    lcd_writecommand(LCD_CLEAR);
    _delay_ms(20);
    lcd_writecommand(LCD_DISPLAYON);

    // Display on
    lcd_writecommand(0x0f);
}

/*
  lcd_moveto - Move the cursor to the row and column given by the arguments.
  Row is 0 or 1, column is 0 - 15.
*/
void lcd_moveto(unsigned char row, unsigned char col)
{
    unsigned char pos;
    pos = row * 0x40 + col;
    lcd_writecommand(0x80 | pos);
}

/*
  lcd_stringout - Print the contents of the character string "str"
  at the current cursor position.
*/
void lcd_stringout(char *str)
{
    char ch;
    while ((ch = *str++) != '\0')
	lcd_writedata(ch);
}

/*
  lcd_writecommand - Output a byte to the LCD command register.
*/
void lcd_writecommand(unsigned char x)
{
    LCD_RS_PORT &= ~(1 << LCD_RS_BIT);       // Clear RS for command write
    lcd_writebyte(x);
    lcd_wait();
}

/*
  lcd_writedata - Output a byte to the LCD data register
*/
void lcd_writedata(unsigned char x)
{
    LCD_RS_PORT |= (1 << LCD_RS_BIT);	// Set RS for data write
    lcd_writebyte(x);
    lcd_wait();
}

/*
  lcd_writebyte - Output a byte to the LCD
*/
void lcd_writebyte(unsigned char x)
{
    lcd_writenibble(x);
    lcd_writenibble(x << 4);
}

/*
  lcd_writenibble - Output a 4-bit nibble to the LCD
*/
void lcd_writenibble(unsigned char x)
{
    
    // Reset bits
    LCD_D7_PORT &= ~(1 << LCD_D7_BIT);
    LCD_D6_PORT &= ~(1 << LCD_D6_BIT);
    LCD_D5_PORT &= ~(1 << LCD_D5_BIT);
    LCD_D4_PORT &= ~(1 << LCD_D4_BIT);

    // Set values
    if (x & (1<<7)) LCD_D7_PORT |= (1 << LCD_D7_BIT);
    if (x & (1<<6)) LCD_D6_PORT |= (1 << LCD_D6_BIT);
    if (x & (1<<5)) LCD_D5_PORT |= (1 << LCD_D5_BIT);
    if (x & (1<<4)) LCD_D4_PORT |= (1 << LCD_D4_BIT);

    LCD_E_PORT &= ~(1 << LCD_E_BIT);	// E=0
    LCD_E_PORT |= (1 << LCD_E_BIT);	// Set E to 1
    LCD_E_PORT |= (1 << LCD_E_BIT);	// Set E to 1 and extend pulse width to 230ns
    LCD_E_PORT &= ~(1 << LCD_E_BIT);	// Set E to 0
}

/*
  lcd_wait - Wait for the BUSY flag to reset
*/
void lcd_wait()
{
    _delay_ms(2);		// Delay for 2ms
}
