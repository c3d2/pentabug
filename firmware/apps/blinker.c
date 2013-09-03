#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>

static void init(void) {
	led_on(RIGHT);
}

static void blinker(void) {
	led_inv(RIGHT);
	led_inv(LEFT);

	uint8_t i;
	for(i = 0; i < 50; ++i) {
		if(button_clicked(RIGHT)) {
			motor_on();
		}

		if(button_clicked(LEFT)) {
			motor_off();
		}

		wait_ms(10);
	}
}

REGISTER(blinker, init, NULL);
