#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>

#include <pentabug/hal.h>
#include <pentabug/app.h>
#include <pentabug/lifecycle.h>

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

		if(app_direction > 0) {
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
