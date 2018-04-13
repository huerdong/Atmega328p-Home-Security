/*************************************************************
*       at328-5.c - Demonstrate interface to a parallel LCD display
*
*       This program will print a message on an LCD display
*       using the 4-bit wide interface method
*
*       PORTB, bit 4 (0x10) - output to RS (Register Select) input of display
*              bit 3 (0x08) - output to R/W (Read/Write) input of display
*              bit 2 (0x04) - output to E (Enable) input of display
*       PORTB, bits 0-1, PORTD, bits 2-7 - Outputs to DB0-DB7 inputs of display.
*
*       The second line of the display starts at address 0x40.
*
* Revision History
* Date     Author      Description
* 11/17/05 A. Weber    First Release for 8-bit interface
* 01/13/06 A. Weber    Modified for CodeWarrior 5.0
* 08/25/06 A. Weber    Modified for JL16 processor
* 05/08/07 A. Weber    Some editing changes for clarification
* 06/25/07 A. Weber    Updated name of direct page segment
* 08/17/07 A. Weber    Incorporated changes to demo board
* 01/15/08 A. Weber    More changes to the demo board
* 02/12/08 A. Weber    Changed 2nd LCD line address
* 04/22/08 A. Weber    Added "one" variable to make warning go away
* 04/15/11 A. Weber    Adapted for ATmega168
* 01/30/12 A. Weber    Moved the LCD strings into ROM
* 02/27/12 A. Weber    Corrected port bit assignments above
* 11/18/13 A. Weber    Renamed for ATmega328P
* 04/10/15 A. Weber    Extended "E" pulse, renamed strout to strout_P
* 05/06/17 A. Weber    Change to use new LCD routines
*************************************************************/

#include "lcd.h"

/*
  Use the "PROGMEM" attribute to store the strings in the ROM
  instead of in RAM.
*/
const unsigned char str1[] PROGMEM = ">> at328-5.c hi <<901234";
const unsigned char str2[] PROGMEM = ">> USC EE459L <<78901234";

int main(void) {

    lcd_init();                 // Initialize the LCD display

    lcd_moveto(0, 0);
    lcd_stringout_P((char *)str1);      // Print string on line 1

    lcd_moveto(1, 0);
    lcd_stringout_P((char *)str2);      // Print string on line 2
 
    int counter = 0;
    while (1) {                 // Loop forever
        lcd_moveto(0, counter);
        counter = (counter + 1) % 16;
	_delay_ms(500);
    }

    return 0;   /* never reached */
}

