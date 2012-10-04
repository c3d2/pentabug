
#include <stdint.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/io.h>
#include "util.h"
#include "bughal.h"
#include "music.h"

//static volatile uint16_t currentNote;

void music_init(void){

   TCCR1A = 0; //TIMER1, normal, no PWM
   TCCR1B = (1 << WGM12) | (1 << CS11); //CTC Mode, Clear Timer on Compare, Prescaler = 8
//   OCR1A = 500; //invert with 2 KHz --> 1 KHz sound --> 8000000/8/2000
//   OCR1A = 10000; 
//   TIMSK1 |= (1 << OCIE1A); //enable Output Compare Interrupt
   TIMSK1 &= ~(1 << OCIE1A); //disable Output Compare Interrupt
	return;
};


void music_setNote(uint16_t note, uint8_t octave){


        cli();
	if (note != NOTE_PAUSE){
	  //Play a Note
	  buzzr_up();
	  TIMSK1 |= (1 << OCIE1A); //enable Output Compare Interrupt
	  OCR1A = octave==0 ? note : note / (1 << octave);
	} else { // Pause (silence)
	  buzzr_off();
	  TIMSK1 &= ~(1 << OCIE1A); //disable Output Compare Interrupt
	}
	sei();
	return;
}




/* timer interrupt function */
ISR(TIMER1_COMPA_vect, ISR_NOBLOCK) {

   // invert buzzer polarity
   buzzr_inv();


}




