#include <pentabug/photons.h>

#include <avr/io.h>

#include <pentabug/hal.h>
#include <pentabug/lifecycle.h>

void photons_init(void) {
	ADCSRA |= (1 << ADEN);
}

void photons_stop(void) {
	ADCSRA &= ~(1 << ADEN);
}

uint8_t photons_measure(void) {
	// save old state

	uint8_t old_right = led_state(RIGHT);
	uint8_t old_left = led_state(LEFT);

	// all leds off

	led_off(RIGHT);
	led_off(LEFT);

	// set to ground for discharge

	DDRC |= 1 << 3;
	PORTC &= ~(1 << 3);

	// wait for discharge

	wait_ms(5);

	// set direction to input for measurement

	DDRC &= ~(1 << 3);

	// collect some photons

	wait_ms(10);

	// start measurement

	// TODO: why can't i move this to the initialization?
	ADMUX  = (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1);

	ADMUX = (ADMUX & ~(0x1F)) | 3;

	ADCSRA |= (1 << ADSC);

	// wait for measurement to finish

	while (ADCSRA & (1 << ADSC)) test_stop_app();

	uint8_t res = ADCH;

	// restore state

	PORTC |= 1 << 3;
	DDRC |= 1 << 3;

	led_set(RIGHT, old_right);
	led_set(LEFT, old_left);

	// done

	return res;
}
