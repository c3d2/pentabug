#include <inttypes.h>
#include "bughal.h"

/* Hardware abstraction layer for Pentabug hardware */

/*
 * initialize LEDs on C0-C3
 */ 
void init_leds(void){
	//enable LED channels as output
	DDRC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);
	// both LEDs off
	PORTC &= ~(1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);


	 //TCCR2A = (1 << WGM21);
	  //TCCR2B = (1 << CS20)|(1 << CS21);
	  //OCR2A = 255; /* TOP */
	  //	TCNT2 = 0;
	  //	/*enable interrupt*/
	  //	TIMSK2 |=  (1<<OCIE2A);


  return;
};

void init_buzzr(int mode);


void init_switch();

static void init_motor(void)
{
	/* vibration motor on B1, initially off: */
	DDRB  |= (1 << PORTB1);
	PORTB &= ~( 1<<PORTB1);
	return;
}

