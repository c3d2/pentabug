#ifndef LIFECYCLE_H
#define LIFECYCLE_H 

#include <setjmp.h>
#include <stdint.h>

extern jmp_buf app_jmp_buf;
extern volatile uint8_t app_should_stop;
extern volatile int8_t app_direction;

// this code does not work ... i have no idea why
// putting the exact same statements at the calling line works ...
/*
// initialize lifecycle, stopped if return != 0
static int enter_app(void) {
	should_stop = 0;
	return setjmp(app_jmp_buf);
}
*/

// stop the running app
inline static void stop_app(void) {
	longjmp(app_jmp_buf, 1);
}

// request to stop the app later
inline static void request_stop_app(void) {
	app_should_stop = 1;
}

// stop app if previously requested
inline static void test_stop_app(void) {
	if(app_should_stop) {
		app_should_stop = 0;
		stop_app();
	}
}

// request the next app to run, set direction
inline static void next_app(int8_t new_direction) {
	app_direction = new_direction;
	request_stop_app();
}

#endif /* LIFECYCLE_H */
