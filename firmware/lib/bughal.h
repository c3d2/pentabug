#ifndef BUGHAL_H
#define BUGHAL_H

/* Hardware abstraction layer for Pentabug hardware */

enum {  BUZZR_OUT,  //initialize buzzer for OUTPUT mode (emmiting soundwaves)
	BUZZR_IN    //initialize buzzer for INPUT mode (registering soundwaves)
};



void init_leds(void);
void init_buzzr(int);
void init_switch(void);
void init_motor(void);

#endif
