#ifndef APPS_H
#define APPS_H 

#include <inttypes.h>

#define MAX_FUNS	16
#define REGISTER(fun)	static void init(void) __attribute__ ((naked, used, section (".init8")));	\
			void init(void) {								\
				mode_funs[mode_num] = fun;					\
				++mode_num;							\
			}									\

typedef void (*mode_fun)(void);

extern mode_fun mode_funs[MAX_FUNS];
extern uint8_t mode_num;

extern uint8_t mode_uninitialized;
extern uint8_t mode_last_tick;

#endif /* APPS_H */
