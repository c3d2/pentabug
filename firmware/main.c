#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

static inline void init_pwm(void)
{
	// TCCR1A xx01xx10
	TCCR1A &= ~0b00100001;
	TCCR1A |= 0b00000010;

	// TCCR1B xxx11001
	TCCR1B &= ~0b00000110;
	TCCR1B |= 0b00011001;

	//PB1 set to output:
	DDRB |= 0b10;
	return;
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

}

int main(void)
{

	init_leds();
	init_motor();
	init_pwm();

	stupid_pwmtest();

	//never get here
	return;
}
