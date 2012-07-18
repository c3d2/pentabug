#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

static inline void init_pwm(void)
{
	//PB1 set to output:
	DDRB |= 0b10;
	OCR1B = 0xefff; //preselect some default
	ICR1  = 0xffff;  // TOP-wert

	TCCR1A = (1<<COM1B1) | (1<<WGM11); // only b-chan , fastpwm (mode 14)
	TCCR1B = (1<<WGM13)|(1<<WGM12) | (1<<CS10); //Fastpwm, no prescale

	return 
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
	PORTC |= (state | 0 b00001111);
	PORTC &= ~(state | 0 b11110000);
	return;
}

static void init_motor(void)
{
	//vibration motor on B1:
	DDRB |= (1 << PORTB1);

}

static void stupid_pwmtest(void)
{
	for (;;) {
		OCR1BH = 0;
		OCR1BL = 0;
		_delay_ms(1);
		OCR1BH = 0xff;
		OCR1BL = 0xff;
		_delay_ms(1);
	}
	return; //never
}

int main(void)
{

	//hardware initialisation:
	init_leds();
	init_motor();
	init_pwm();

	//just stupid test for now....
	stupid_pwmtest();

	//never get here
	return;
}
