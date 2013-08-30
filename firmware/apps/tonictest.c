#include <pentabug/app.h>
#include <pentabug/pentatonic.h>
#include <pentabug/hal.h>

static void init(void) {
	pentatonic_direction(ALL_IN);
}

static void run(void) {
	pentatonic_direction(ALL_IN);

	for(;;) {
		uint8_t buttons = pentatonic_buttons();

		if(buttons) {
			pentatonic_direction(ALL_OUT);

			pentatonic_all_led_set(buttons);

			led_inv(RIGHT);

			wait_ms(1000);

			break;
		}

		wait_ms(1);
	}
}

REGISTER(run, init, NULL);
