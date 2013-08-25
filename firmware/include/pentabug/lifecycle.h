#ifndef LIFECYCLE_H
#define LIFECYCLE_H 

#include <setjmp.h>
#include <stdint.h>

extern jmp_buf app_jmp_buf;
extern uint8_t should_stop;

// initialize lifecycle, stopped if return != 0
inline static int enter_app(void) {
	should_stop = 0;
	return setjmp(app_jmp_buf);
}

// stop the running app
inline static void stop_app(void) {
	longjmp(app_jmp_buf, 1);
	for(;;);
}

// request to stop the app later
inline static void request_stop_app(void) {
	should_stop = 1;
}

// stop app if previously requested
inline static void test_stop_app(void) {
	if(should_stop) {
		stop_app();
	}
}

#endif /* LIFECYCLE_H */
