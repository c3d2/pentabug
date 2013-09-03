#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>

#include <pentabug/hal.h>
#include <pentabug/app.h>
#include <pentabug/lifecycle.h>
#include <pentabug/music.h>
#include <pentabug/helper.h>

static uint16_t up_mldy[] = { NOTE_C, NOTE_E, NOTE_G, MLDY_LENGTH, 2, NOTE_C/2, NOTE_PAUSE };
static uint16_t down_mldy[] = { NOTE_C/2, NOTE_G, NOTE_E, MLDY_LENGTH, 2, NOTE_C, NOTE_PAUSE };

static inline void run_app(struct app_t* app) {
	app_should_stop = 0;

	if(setjmp(app_jmp_buf) == 0) {
		// initial call

		if(app->init) {
			app->init();
		}

		for(;;) {
			app->run();
			test_stop_app();
		}
	}

	// returned after longjmp()

	if(app->cleanup) {
		app->cleanup();
	}

	return;
}

int main(void) {
	uint8_t app_index = 0;

	init_hw();

	// cycle through apps

	for(;;) {
		reset_hw();

		run_app(&apps[app_index]);

		reset_hw();

		if(app_direction > 0) {
			play_melody(up_mldy, ARRAY_SIZE(up_mldy), 4, 100);

			app_index++;

			if(apps[app_index].run == NULL) {
				app_index = 0;
			}
		} else {
			play_melody(down_mldy, ARRAY_SIZE(down_mldy), 4, 100);

			if(app_index == 0) {
				app_index = MAX_APPS - 1;
				while(apps[app_index].run == NULL) --app_index;
			} else {
				app_index--;
			}
		}
	}
}
