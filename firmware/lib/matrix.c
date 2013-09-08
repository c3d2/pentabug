#include <pentabug/matrix.h>

#include <pentabug/timer.h>
#include <pentabug/hal.h>

static uint8_t pixels[9] = {0};
static uint8_t index;
static uint8_t cycle;

void matrix_init(void) {
	DDRD |= (1 << 5) | (1 << 6) | (1 << 7);
	DDRC |= (1 << 4) | (1 << 5);
	DDRB |= 1 << 2;
}

static void move_line(uint8_t line) {
	PORTD |= 0xe0;
	PORTD &= ~(1 << (line + 5));
}

static void write_line(uint8_t data[], uint8_t timing) {
	PORTC &= ~((1 << 5) | (1 << 4));
	PORTB &= ~(1 << 2);

	if(data[0] > timing) {
		PORTC |= 1 << 5;
	}

	if(data[1] > timing) {
		PORTC |= 1 << 4;
	}

	if(data[2] > timing) {
		PORTB |= 1 << 2;
	}
}

static void matrix_int(void) {
	++index;

	if(index >= 3) {
		++cycle;
		index = 0;
	}

	move_line(index);
	write_line(pixels + (index * 3), cycle & 3);
}

void matrix_start(void) {
	matrix_init();
	start_timer(PRESCALE_64, 32, matrix_int);
}

void matrix_stop(void) {
	stop_timer();
}

void matrix_show(void) {
	for(uint8_t n = 0; n < 4; ++n) {
		uint8_t i;

		for(i = 0; i < 3; ++i) {
			move_line(i);
			write_line(pixels + (i * 3), n);
			wait_ms(1);
		}
	}
}

void matrix_set(uint8_t x, uint8_t y, uint8_t active) {
	pixels[x*3+y] = active ? 4 : 0;
}

void matrix_brightness(uint8_t x, uint8_t y, uint8_t brightness) {
	pixels[x*3+y] = brightness;
}

