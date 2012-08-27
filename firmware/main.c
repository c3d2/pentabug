#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"



uint16_t sw_v = 0;
uint16_t darkness = 0;
uint16_t darkval  = 0;
uint16_t last_darkval  = 0;




void __attribute__((noreturn)) 
main(void)
{

	/* here the show begins:*/
	
			DDRC  = 0b00100011;
			PORTC = 0b00000001;
			PORTC = 0b00000000;
			DDRD = 0x00;
			PORTD = 0b00000011; //enable pullups
			DDRB = 0xFF;
			PORTB=0xFF;

	/* set timer2 to CTC & prescaler 64 →  ???125kHz increment */
	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << CS20)|(1 << CS21);

	OCR2A = 0x3F; /* TOP */
	TCNT2 = 0;
	/*enable interrupt*/
	TIMSK2 |=  (1<<OCIE2A);

	sei();


	for(;;) /* ever */  {

		//do something
		//synth_poll();
	/*	if (10 == 1){
			uint16_t out = darkval;
			USART0_put_uint16(out);
			USART0_crlf();
			//last_darkval=darkval;
		};*/

	DDRC |= (1 << PORTC0) | (1 << PORTC2) |
		(1 << PORTC3) | (1 << PORTC1) ;
	/* initially one led is on */
	if ((PIND & 0b00000001)==0){
	  PORTC |= 0b00000001;
	  PORTB |= 0b00000010;
	} else {
	  PORTC &= 0b11111110;
	  //PORTB &= 0b11111101;

	};
	if ((PIND & 0b00000010)==0){
	  PORTC |= 0b00000100;
	} else {
	  PORTC &= 0b11111011;

	};


		//USART0_put_uint16(0xA09F);
		//USART0_crlf();
	}

	/* never  return 0; */
}



ISR(TIMER2_COMPA_vect)
{
	//invert c5 and b2
	PORTB ^=0b00000100;
	PORTC ^= 0b00100000;

}
