#include "lcd.h"
#include "clock.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

const unsigned char init_str[] = "System Init    ";
const unsigned char adult_1[] =  "Adult move: 1-2";
const unsigned char adult_2[] =  "Adult move: 2-1";
const unsigned char child_1[] =  "Child move: 1-2";
const unsigned char child_2[] =  "Child move: 2-1";
const unsigned char w_open[] =   "Window Open    ";
const unsigned char w_closed[] = "Window Closed  ";
volatile uint8_t portdhistory = 0b10111111;
struct clock_time now;
int windowOpen = 0;
int startPassing = 0;
volatile int peopleInRoom1 = 0;
volatile int peopleInRoom2 = 2;
int halfwaythrough = 0;
int direction = 0; //0 = 1-2, 1 = 2-1
int high = 0;
char people_str[10];

struct clock_time alarm_start;
struct clock_time alarm_end;
char alarm_cnt = 0;
char alarm_on = 0;

//pd2, pd3, pd4, pd5 = IR sensor inputs (pd2 = high1, pd3 = high2, pd4 = low1, pd5 = low2)
//pd6 = MAG sensor input
//pc1, pc3 = LED output
//pc2 = buzzer output

void init_io() {

    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6));
    DDRB |= (1 << PB5);
    DDRC |= ((1 << PC2) | (1 << PC3) | (1 << PC1));
    PORTD |= ((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6));
    PCICR |= 0b00000100;    // turn on port d interrupts
    PCMSK2 |= 0b01111100;    // turn on pins PD0 & PD7, PCINT16 & PCINT23
    lcd_init();
    i2c_init(BDIV);
    TCCR1B |= (1 << WGM12);     // Set for CTC mode.  OCR1A = modulus
    TIMSK1 |= (1 << OCIE1A);    // Enable CTC interrupt                     // Enable global interrupts
    OCR1A = 28800;              // Set the counter modulus
    TCCR1B |= (1 << CS12);      // Set prescaler for divide by 256,
                                // also starts timer

    // Initialize the time
    clock_read(&now);
    alarm_start.sec = 0;
    alarm_start.min = 0;
    alarm_start.hour = 16;
    alarm_end.sec = 0;
    alarm_end.min = 45;
    alarm_end.hour = 16;

    sei(); //turn on interrupts
}

char alarm_active(struct clock_time* now, struct clock_time* start, struct clock_time* end) {
    char pre = before(now, end);
    char post = before(start, now);
    if (pre != 0 && post != 0) {
        return 1;
    }
    return 0;
}

int main(void) {
    
    init_io();

    lcd_moveto(0, 0);
    lcd_stringout("System Init");      // Print string on line 1
    _delay_ms(1000);
    
    while (1) {           
        if (peopleInRoom2 > 0)
        {
            PORTC |= (1 << PC1);
        }    // Loop forever
        else
        {
            PORTC &= ~(1 << PC1);
        }
        if (peopleInRoom1 > 0)
        {
            PORTC |= (1 << PC3);
        }
        else 
        {
            PORTC &= ~(1 << PC3);
        }
	if (alarm_on == 1) {
	    PORTC |= (1 << PC2);	
	}
	else {
	    PORTC &= ~(1 << PC2);
	}
    }

    return 0;   /* never reached */
}

ISR(PCINT2_vect){    // Port D, PCINT16 - PCINT23
    cli();

    uint8_t changedbits;

    changedbits = PIND ^ portdhistory;
    portdhistory = PIND;


    if(changedbits & (1 << PD2)) // high 1
    {
        /* PD2 changed */
        if ((PIND & (1 << PD2)) == 0){
            high = 1;
        }

    }

    /*if(changedbits & (1 << PD3)) //high 2 not working
    {
         PD3 changed 
        if ((PIND & (1 << PD3)) == 0){
            
            lcd_moveto(0, 0);

            lcd_stringout("Interrupted High2");      // Print string on line 1
        }

        else{
            
            lcd_moveto(0, 0);
            lcd_stringout("Clear High2         ");      // Print string on line 1
        }
    }*/

    if(changedbits & (1 << PD4)) // low 1
    {
        /* PD4 changed */
        if ((PIND & (1 << PD4)) == 0){
            if (startPassing == 0)
            {
                startPassing = 1;
                direction = 0;
            }
        }

        else{
            if (startPassing == 1 && direction == 1)
            {
                startPassing = 0;
                if(high == 0)
                {
                    lcd_moveto(1, 0);
                    lcd_stringout("Child 2-1    ");      // Print string on line 1
                    peopleInRoom2--;
                    peopleInRoom1++;
                }
                else
                {
                    lcd_moveto(1, 0);
                    lcd_stringout("Adult 2-1    ");      // Print string on line 1
                    high = 0;
                    peopleInRoom2--;
                    peopleInRoom1++;
                }
               
            }
        }
    }

    if(changedbits & (1 << PD5)) // low 2
    {
        /* PD5 changed */
        if ((PIND & (1 << PD5)) == 0){
            
            if (startPassing == 0)
            {
                startPassing = 1;
                direction = 1;
            }
        }

        else{
            if (startPassing == 1 && direction == 0)
            {
                startPassing = 0;
                if(high == 0)
                {
                    lcd_moveto(1, 0);
                    lcd_stringout("Child 1-2      ");      // Print string on line 1
                    peopleInRoom1--;
                    peopleInRoom2++;
                }
                else
                {
                    lcd_moveto(1, 0);
                    lcd_stringout("Adult 1-2      ");      // Print string on line 1
                    high = 0;
                    peopleInRoom1--;
                    peopleInRoom2++;
                } 
            }
        }
    }

    if(changedbits & (1 << PD6))
    {
        /* PD6 changed */
        if ((PIND & (1 << PD6)) == 0){
            windowOpen = 0;
            
            lcd_moveto(1, 0);

            lcd_stringout("Window Closed");      // Print string on line 1
        }

        else{
	    if (alarm_active(&now, &alarm_start, &alarm_end) == 1) {
	        alarm_on  =  1;
	    }

            windowOpen = 1;
            
            lcd_moveto(1, 0);
            lcd_stringout("Window Open    ");      // Print string on line 1
        }
    }
    sei();
    
}   

ISR(TIMER1_COMPA_vect)
{
    cli();
    clock_read(&now);
    char buffer[20];
    print(&now, buffer, 20);
    lcd_moveto(0, 0);
    lcd_stringout("                       ");
    lcd_moveto(0, 0);
    lcd_stringout(buffer);
    sprintf(people_str, " 1:%d 2:%d", peopleInRoom1, peopleInRoom2);
    lcd_stringout(people_str);

    if (alarm_on == 1) {
    	alarm_cnt++;
	if (alarm_cnt == 2) {
	    alarm_cnt = 0;
	    alarm_on = 0;
	}
    }

    sei();
}

