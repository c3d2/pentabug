#include <pentabug/listen.h>

#include <avr/io.h>

#include <pentabug/hal.h>
#include <pentabug/lifecycle.h>

void listen_init(void) {
	ADCSRA |= (1 << ADEN);
}

void listen_stop(void) {
	ADCSRA &= ~(1 << ADEN);
}

uint16_t listen_measure(void) {
	// save previous state

	uint8_t prev_state = PINC & (1 << 0);

	// set to ground for discharge

	DDRC |= 1 << 0;
	PORTC &= ~(1 << 0);

	// wait for discharge

	wait_ms(1);

	// set direction to input for measurement

	DDRC &= ~(1 << 0);

	// collect some sound waves

	wait_ms(1);

	// start measurement

	// TODO: why can't i move this to the initialization?
	ADMUX  = (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1);

	ADMUX = (ADMUX & ~(0x1F)) | 0;

	ADCSRA |= (1 << ADSC);

	// wait for measurement to finish

	while (ADCSRA & (1 << ADSC)) test_stop_app();

	uint16_t res = ADCL;
	res |= ADCH << 8;

	// restore state

	if(prev_state) {
		PORTC |= 1 << 0;
	}

	DDRC |= 1 << 0;

	// done

	return res;
}
