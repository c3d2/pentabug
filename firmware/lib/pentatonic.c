#include <pentabug/pentatonic.h>

#include <avr/io.h>

// pins affected by pentatonic on each port
const uint8_t bits_c = (1 << 4) | (1 << 5);
const uint8_t bits_d = (1 << 5) | (1 << 6) | (1 << 7);

static inline uint8_t bits_to_c(uint8_t direction) {
	return ((direction & (1 << 3)) << 2) | (direction & (1 << 4));
}

static inline uint8_t bits_to_d(uint8_t direction) {
	return (direction << 5) & bits_d;
}

void pentatonic_direction(uint8_t direction) {
	// map bits to pins

	uint8_t part_c = bits_to_c(direction);
	uint8_t part_d = bits_to_d(direction);

	// reset everything

	PORTC &= ~bits_c;
	DDRC &= ~bits_c;

	PORTD &= ~bits_d;
	DDRD &= ~bits_d;

	// pullup if input, off if output (inverted)

	PORTC |= part_c;
	PORTD |= part_d;

	// set new direction

	DDRC |= part_c;
	DDRD |= part_d;
}

void pentatonic_multi_led_on(uint8_t leds) {
	// map bits to pins

	uint8_t part_c = bits_to_c(leds);
	uint8_t part_d = bits_to_d(leds);

	// set leds on (inverted)

	PORTC &= ~(part_c & bits_c);
	PORTD &= ~(part_d & bits_d);
}

void pentatonic_multi_led_off(uint8_t leds) {
	// map bits to pins

	uint8_t part_c = bits_to_c(leds);
	uint8_t part_d = bits_to_d(leds);

	// set leds on (inverted)

	PORTC |= part_c;
	PORTD |= part_d;
}

void pentatonic_led_on(uint8_t led) {
	pentatonic_multi_led_on(1 << led);
}

void pentatonic_led_off(uint8_t led) {
	pentatonic_multi_led_off(1 << led);
}

void pentatonic_led_set(uint8_t led, uint8_t state) {
	if(state) {
		pentatonic_led_on(led);
	} else {
		pentatonic_led_off(led);
	}
}

void pentatonic_all_led_set(uint8_t leds) {
	pentatonic_multi_led_off(0xff);
	pentatonic_multi_led_on(leds);
}
