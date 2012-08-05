#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

volatile uint8_t sample_pending;


// sample rate is 8M / (5 * 64) = 25000


enum {
	synth_channel_count = 2
};

typedef struct {
	uint16_t phase;
	uint16_t speed;

} synth_channel_t;

typedef struct {
	synth_channel_t channels[synth_channel_count];
} synth_t;

static synth_t synth;


static void synth_init(void)
{
	// some test values
	synth.channels[0].phase = 0;
	synth.channels[0].speed = 1153;

	synth.channels[1].phase = 0;
	synth.channels[1].speed = 1728;
}

static inline uint16_t synth_mix(void)
{
	uint16_t output = 0;

	for (int i = 0; i < synth_channel_count; i++) {
		synth_channel_t *chan = &synth.channels[i];
		chan->phase += chan->speed;
		output += ((chan->phase >> 8) & 0xff) / 2;
	}

	return output;
}

static void init_sampletimer(void)
{
	// Timer 0
	// set timer0 to CTC & prescaler 64 == 125k
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << CS01);
	// count up to 5 :
	OCR0A = 3;
	TCNT0 = 0;
	// enable interrupt
	TIMSK0 |=  (1<<OCIE0A);
}

static inline void init_pwm(void)
{
	// PB2 set to output:
	DDRB |= (1 << PORTB2);
	OCR1B = 0x001F;		// preselect some default
	ICR1 = 0x00FF;		// TOP-wert

	TCCR1A = (1 << COM1B1) | (1 << WGM11);	// only b-chan , fastpwm (mode 14)
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);	// Fastpwm, no prescale

	// TIMSK1 |=  (1 << OCIE1B); //enable timer 1 Output compare
	return;
}

static void init_leds(void)
{
	// enable LED channels as output
	DDRC |= (1 << PORTC0) | (1 << PORTC2) | (1 << PORTC3) | (1 << PORTC1);
	PORTC = 1;		//one led is on...
	return;
}

inline void setleds(uint8_t state)
{
	// set leds according to
	PORTC |= (state | 0b00001111);
	PORTC &= ~(state | 0b11110000);
	return;
}

static void init_motor(void)
{
	// vibration motor on B1:
	DDRB |= (1 << PORTB1);

}




int main(void)
{

	// hardware initialisation:
	init_leds();
	// init_motor();
	init_pwm();
	init_sampletimer();

	synth_init();
	// OCR1B = 0x00F0;
	sei();

	while(1) {}

	// never get here
	return 0;
}


// 25kHz
ISR(TIMER0_COMPA_vect)
{
	// test stuff
	/*
	static uint16_t counter = 0;
	static uint16_t pulsewidth = 0;
	static uint16_t maxcounter = 0x133;
	static uint16_t pulsecounter = 0;
	counter++;
	if (counter > maxcounter){
	 	counter = 0;
	};

	pulsecounter++;
	if (pulsecounter > 0x300){
		 pulsecounter = 0;
		 pulsewidth++;
		 if (pulsewidth > maxcounter){
			pulsewidth = 0;
		}
	}
	OCR1B = ((counter > pulsewidth) ? 0xff : 0x00);
	*/


	OCR1B = synth_mix();

}



