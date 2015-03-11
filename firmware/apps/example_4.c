#include <pentabug/hal.h>
#include <pentabug/app.h>

// was passiert?
// gibt es probleme?
// wie kann man das problem lösen?

static void init(void) {
	led_on(RIGHT);
	led_on(LEFT);
}

static void example(void) {
	led_inv(LEFT);
	led_inv(RIGHT);

	wait_ms(500);
	
	led_inv(LEFT);

	wait_ms(500);

	led_inv(RIGHT);

	wait_ms(500);

	if(button_clicked(LEFT)) {
		motor_inv();
		wait_ms(500);
		motor_inv();
		wait_ms(500);
		motor_inv();
		wait_ms(500);
		motor_inv();
		wait_ms(500);
	}
}

REGISTER(example, init, NULL);
