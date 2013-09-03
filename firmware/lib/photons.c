#include <pentabug/photons.h>

#include <avr/io.h>

#include <pentabug/hal.h>
#include <pentabug/lifecycle.h>

void photons_init(void) {
	ADCSRA |= (1 << ADEN);
}

uint8_t photons_measure(void) {
	// save old state

	uint8_t old_port = PORTC;
	uint8_t old_ddr = DDRC;

	// all leds off

	led_off(RIGHT);
	led_off(LEFT);

	// set to ground for discharge

	PORTC &= ~(1 << 3);

	// wait for discharge

	wait_ms(1);

	// set direction to input for measurement

	DDRC &= ~(1 << 3);

	// collect some photons

	wait_ms(1);

	// start measurement

	ADMUX  = (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1);

	ADMUX = (ADMUX & ~(0x1F)) | 3;

	ADCSRA |= (1 << ADSC);

	// wait for measurement to finish

	while (ADCSRA & (1 << ADSC)) test_stop_app();

	uint8_t res = ADCH;

	// disable adc

	/*ADCSRA &= ~(1 << ADEN);*/

	// restore state

	PORTC = old_port;
	DDRC = old_ddr;

	// done

	return res;
}
