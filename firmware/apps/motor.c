#include <pentabug/app.h>
#include <pentabug/hal.h>

static void run(void) {
	if(button_clicked(LEFT)) {
		motor_inv();
		led_inv(LEFT);
		led_inv(RIGHT);
	}
}

REG(run);
