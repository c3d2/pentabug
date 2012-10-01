#ifndef BUGHAL_H
#define BUGHAL_H

#include <stdbool.h>

/* Hardware abstraction layer for Pentabug hardware */

enum {  BUZZR_OUT,  //initialize buzzer for OUTPUT mode (emmiting soundwaves)
	BUZZR_IN    //initialize buzzer for INPUT mode (registering soundwaves)
};

#define LED_L (1 << PORTC0)
#define LED_R (1 << PORTC2)
#define MOTOR_ON (0<<PORTB1)
#define MOTOR_OFF (1<<PORTB1)

void init_leds(void);
void led_on(int);
void led_off(int);

void init_buzzr(void);
void buzzr_up(void);
void buzzr_down(void);
void buzzr_off(void);
void buzzr_inv(void);

void init_switch(void);
bool switch_l(void); //switch pressed?
bool switch_r(void); //switch pressed?

void init_motor(void);
void set_motor(int);




#endif
