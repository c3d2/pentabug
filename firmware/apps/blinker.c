#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>

static void init(void) {
	led_on(RIGHT);
}

static void blinker(void) {
	led_inv(RIGHT);
	led_inv(LEFT);

	wait_ms(500);

	if(button_clicked(RIGHT)) {
		motor_on();
	}

	if(button_clicked(LEFT)) {
		motor_off();
	}

}

REGISTER(blinker, init, NULL);
