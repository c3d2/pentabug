#ifndef APP_H
#define APP_H 

#include <stdlib.h>

#define MAX_APPS		8

#define REG(run)			REGISTER(run, NULL, NULL);
#define REGISTER(_run, _init, _cleanup)	static void __init(void) __attribute__ ((naked, used, section (".init8")));	\
					void __init(void) {								\
						uint8_t i = 0;								\
						while(apps[i].run) ++i;							\
						apps[i].run = _run;							\
						apps[i].init = _init;							\
						apps[i].cleanup = _cleanup;						\
					}

struct app_t {
	void (*run)(void);
	void (*init)(void);
	void (*cleanup)(void);
};

extern struct app_t apps[MAX_APPS];

#endif /* APP_H */
