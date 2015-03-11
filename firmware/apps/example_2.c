#include <pentabug/hal.h>
#include <pentabug/app.h>

// was erwartest du das passiert?
// wie kann man das Problem lösen?

static void example(void) {
	if(button_state(LEFT)) {
		led_inv(LEFT);
	}


	if(button_state(RIGHT)) {
		motor_inv();
	}
}

REG(example);
