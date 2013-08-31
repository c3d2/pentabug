#include <pentabug/photons.h>

#include <avr/io.h>

#include <pentabug/hal.h>
#include <pentabug/lifecycle.h>

void photons_init(void) {}

uint16_t photons_measure(void) {
	// save old state

	uint8_t old_port = PORTC;
	uint8_t old_ddr = DDRC;

	// all leds off

	led_off(RIGHT);
	led_off(LEFT);

	// set direction, no pullups

	DDRC &= ~(1 << 3);
	PORTC &= ~(1 << 3);

	// wait for discharge
	// TODO: needed?

	wait_ms(10);

	// start measurement

	ADMUX  = (1 << REFS0);
	ADCSRA = (1 << ADPS2) | (1 << ADPS1);
	ADCSRA |= (1 << ADEN);

	ADMUX = (ADMUX & ~(0x1F)) | 3;
	ADCSRA |= (1 << ADSC);

	// wait for measurement to finish

	while (ADCSRA & (1 << ADSC)) test_stop_app();

	uint16_t res = ADCW;

	// disable adc

	ADCSRA &= ~(1 << ADEN);

	// restore state

	PORTC = old_port;
	DDRC = old_ddr;

	// done

	return res;
}
