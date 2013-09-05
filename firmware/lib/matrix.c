#include <pentabug/matrix.h>

#include <pentabug/timer.h>
#include <pentabug/hal.h>

static uint8_t pixels[9];
static uint8_t index;

void matrix_init(void) {
	DDRD |= (1 << 5) | (1 << 6) | (1 << 7);
	DDRC |= (1 << 4) | (1 << 5);
	DDRB |= 1 << 2;
}

static void move_line(uint8_t line) {
	PORTD &= ~0xe0;
	PORTD |= 1 << (line + 5);
}

static void write_line(uint8_t data[]) {
	PORTC |= (1 << 5) | (1 << 4);
	PORTB |= 1 << 2;

	if(data[0]) {
		PORTC &= ~(1 << 5);
	}

	if(data[1]) {
		PORTC &= ~(1 << 4);
	}

	if(data[2]) {
		PORTB &= ~(1 << 2);
	}
}

static void matrix_int(void) {
	++index;

	if(index >= 3) {
		index = 0;
	}

	move_line(index);
	write_line(pixels + (index * 3));
}

void matrix_start(void) {
	matrix_init();
	start_timer(PRESCALE_64, 125, matrix_int);
}

void matrix_stop(void) {
	stop_timer();
}

void matrix_show(void) {
	uint8_t i;

	for(i = 0; i < 3; ++i) {
		move_line(i);
		write_line(pixels + (i * 3));
		wait_ms(3);
	}
}

void matrix_set(uint8_t x, uint8_t y, uint8_t active) {
	pixels[x*3+y] = active;
}

