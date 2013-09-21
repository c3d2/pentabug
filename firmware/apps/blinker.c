#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/matrix.h>

#ifdef MATRIX
uint8_t inv = 0;
#endif

static void init(void) {
	led_on(RIGHT);
	matrix_start();
}

static void blinker(void) {
	led_inv(RIGHT);
	led_inv(LEFT);

#ifdef MATRIX
	for(uint8_t x = 0; x < 3; ++x) {
		for(uint8_t y = 0; y < 3; ++y) {
			matrix_set(x, y, ((x + y) & 1) ^ inv);
		}
	}

	inv = !inv;
#endif

	wait_ms(500);

	if(button_clicked(RIGHT)) {
		motor_on();
	}

	if(button_clicked(LEFT)) {
		motor_off();
	}

}

REGISTER(blinker, init, NULL);
