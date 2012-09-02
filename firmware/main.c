#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"
#include "lib/synth.h"
#include "lib/usart.h"
#include "lib/bughal.h"


uint16_t sw_v = 0;
uint16_t darkness = 0;
uint16_t darkval  = 0;
uint16_t last_darkval  = 0;


static void init_sampletimer(void)
{
	/* 
	 * Timer 0
	 */

	/* set timer0 to CTC & prescaler 64 → 125kHz increment */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << CS01);

	OCR0A = 6; /* TOP */
	TCNT0 = 0;
	/*enable interrupt*/
	TIMSK0 |=  (1<<OCIE0A);
}

static inline void init_pwm(void)
{
	/* set PB2 as output (buzzer pwm port):*/
	DDRB  |= (1 << PORTB2);

	/* analog value preselection :*/
	OCR1B  =  0x007F;

	/* Top value. Timer overflows here. Thus we have a resulting 8bit pwm */
	ICR1   =  0x00FF;

	/* only b-chan , fastpwm (mode 14), no prescale */
	TCCR1A = (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13 ) | (1 << WGM12) | (1 << CS10);

	return;
}


inline void setleds(uint8_t state)
{
	/* set leds according to */
	PORTC |= (state | 0b00001111);
	PORTC &= ~(state | 0b11110000);
	return;
}




void __attribute__((noreturn)) 
main(void)
{
	/* hardware initialisation: */
	init_leds();
	//USART0_Init();
	// init_motor();
	//init_pwm();
	//init_sampletimer();
	
	/* syntesizer initialisation */
	//synth_init();

	/* here the show begins:*/
	//sei();
	for(;;) /* ever */  {

		//do something
		//synth_poll();
		//USART0_put_uint16(0xA09F);
		//USART0_crlf();
	}

	/* never  return 0; */
}



ISR(TIMER2_COMPA_vect)
{
darkval++;
	switch(sw_v){

	case 0 :	// Set PC0 to 1 and PC1 to 0 - charge LED
			
			DDRC  = 0b00000011;
			PORTC = 0b00000010;
			
			darkness = 0;
			sw_v++;
			
			break;

	case 10 : 	// set PC0 to 0 and PC1 as input
			
			DDRC  = 0b00000001;
		 	PORTC = 0b00000000;
			
			sw_v++;
			break;
	case 5000 :	sw_v = 0;
			//darkval = darkness;
			//darkval = 1234;
			break;
	default :	//measure if PC1 falls to LOW
			if ((darkness==0) && ((PINC && 0b00000010)==0)){
				darkness = sw_v;
			};
			sw_v++;
	
			
	}; //end switch
	
	//PORTC ^= 0b00001111; //toggle C0, C1, C2,C3
	//PORTC = 0;
	
}
