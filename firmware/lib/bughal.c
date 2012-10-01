#include <inttypes.h>
#include <avr/io.h>
#include "bughal.h"

/* Hardware abstraction layer for Pentabug hardware */

/*
 * initialize LEDs on C0-C3
 */ 
void init_leds(void){
	//enable LED channels as output
	DDRC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);
	// both LEDs off
	PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3));


	 //TCCR2A = (1 << WGM21);
	  //TCCR2B = (1 << CS20)|(1 << CS21);
	  //OCR2A = 255; /* TOP */
	  //	TCNT2 = 0;
	  //	/*enable interrupt*/
	  //	TIMSK2 |=  (1<<OCIE2A);


  	return;
};

inline void led_on(int leds){
 	PORTC |= leds;
};

inline void led_off(int leds){
	PORTC &= ~leds;
};



void init_buzzr(){
	//its on B2 and C5, for reasons
	DDRC |= (1 << PORTC5);
	DDRB |= (1 << PORTB2);
	//switch it off
	buzzr_off();
	return;
};

void buzzr_up(){
	PORTC &= ~(1 << PORTC5);
	PORTB |= (1 << PORTB2);
	return;
};

void buzzr_down(){
	PORTC |= (1 << PORTC5);
	PORTB &= ~(1 << PORTB2);
};

inline void buzzr_off(){
	PORTC &= ~(1 << PORTC5);
	PORTB &= ~(1 << PORTB2);
};
void buzzr_inv(){
	PORTC ^= (1 << PORTC5);
	PORTB ^= (1 << PORTB2);
};

void init_switch(){
	return;
};

void init_motor(void)
{
	/* vibration motor on B1, initially off: */
	DDRB  |= (1 << PORTB1);
	PORTB &= ~( 1<<PORTB1);
	return;
}

void set_motor(int val){
	PORTB = ~(val);
	return;
};

