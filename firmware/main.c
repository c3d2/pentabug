#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"


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
	uint16_t output;
} synth_t;


static synth_t synth;

static void synth_init(void)
{
	// some test values
	synth.channels[0].phase = 0;
	synth.channels[0].speed = 1153;

	synth.channels[1].phase = 0;
	synth.channels[1].speed = 23456;
}


static inline void synth_mix(void)
{
	synth.output = 0;

	for(int i = 0; i < synth_channel_count; i++) {
		synth_channel_t* chan = &synth.channels[i];
		chan->phase += chan->speed;

		synth.output += (chan->phase >> 16) & 0xff;
	}
}






static inline void init_pwm(void)
{
	//PB1 set to output:
	DDRB |= (1<<PORTB2);
	OCR1B = 0xefff; //preselect some default
	ICR1  = 0xffff; // TOP-wert

	TCCR1A = (1<<COM1B1) | (1<<WGM11); // only b-chan , fastpwm (mode 14)
	TCCR1B = (1<<WGM13)|(1<<WGM12) | (1<<CS10); //Fastpwm, no prescale

	return ;
}

static void init_leds(void)
{
	//enable LED channels as output
	DDRC |= (1 << PORTC0) | (1 << PORTC2) | (1 << PORTC3) | (1 << PORTC1);
	PORTC = 1;		//one led is on...
	return;
}

inline void setleds(uint8_t state)
{
	//set leds according to
	PORTC |= (state | 0b00001111);
	PORTC &= ~(state | 0b11110000);
	return;
}

static void init_motor(void)
{
	//vibration motor on B1:
	DDRB |= (1 << PORTB1);

}

static void stupid_pwmtest(void)
{
	uint8_t i, t, r;
	ICR1 = 0xAA00;
	t = r = 1;
	for (;;) {

		t = (r) ? (t+1) : (t-1);

		ICR1 = (t << 7);
		if(t == 0) r ^= 1;
		for(i = 1; i < 100; i++) __asm("nop");


	
		
	}
	return; //never
}

int main(void)
{

	//hardware initialisation:
	init_leds();
	init_motor();
	init_pwm();

	synth_init();

	//just stupid test for now....
	stupid_pwmtest();

	//never get here
	return 0;
}
