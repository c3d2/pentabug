#include <pentabug/app.h>
#include <pentabug/pentatonic.h>

static void init(void) {
	pentatonic_direction(ALL_OUT);
}

static void run(void) {
	pentatonic_led_on(0);

	for(uint8_t i = 0; i < 5; ++i) {
		wait_ms(500);

		pentatonic_led_on((i + 1) % 5);

		wait_ms(200);

		pentatonic_led_off(i);
	}
}

REGISTER(run, init, NULL);
