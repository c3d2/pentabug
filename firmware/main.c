#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>

#include <avr/io.h>

#include <pentabug/app.h>
#include <pentabug/lifecycle.h>

static inline void reset_hw(void) {
	DDRB = 0;
	DDRC = 0;
	DDRD = 0;

	PORTB = 0;
	PORTC = 0;
	PORTD = 0;

	PINB = 0;
	PINC = 0;
	PIND = 0;
}

static inline void run_app(struct app_t* app) {
	if(enter_app()) {
		// this is the exit
		if(app->cleanup) {
			app->cleanup();
		}

		return;
	}

	if(app->init) {
		app->init();
	}

	for(;;) {
		app->run();
		test_stop_app();
	}
}

int main(void) {
	uint8_t app_index = 0;
	int8_t direction = 1;

	// we need to get real fast (8MHz) to handle 38kHz IR frequency ...

	CLKPR = 0b10000000;
	CLKPR = 0b00000000;

	// cycle through apps

	for(;;) {
		reset_hw();

		run_app(&apps[app_index]);

		for(;;);

		if(direction > 0) {
			app_index++;

			if(apps[app_index].run == NULL) {
				app_index = 0;
			}
		} else {
			if(app_index == 0) {
				app_index = MAX_APPS - 1;
				while(apps[app_index].run == NULL) --app_index;
			} else {
				app_index--;
			}
		}
	}

}
