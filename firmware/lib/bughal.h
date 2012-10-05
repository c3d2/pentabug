#ifndef BUGHAL_H
#define BUGHAL_H

#include <stdbool.h>

/* Hardware abstraction layer for Pentabug hardware */

enum {  BUZZR_OUT,  //initialize buzzer for OUTPUT mode (emmiting soundwaves)
	BUZZR_IN    //initialize buzzer for INPUT mode (registering soundwaves)
};

#define LED_L (1 << PORTC0)
#define LED_R (1 << PORTC2)
#define MOTOR_ON 1
#define MOTOR_OFF 0

#define BTN_RIGHT		1
#define BTN_LEFT		0
#define BTN_BUTTONS     2  //numer of Buttons
#define BTN_T_DEBOUNCE  5  // 50ms debounce time = minimum short press time
#define BTN_T_LONGFACT  10 // after 10 * T_DEBOUNCE = 500ms button registers as long pressed

//BUTTON state machine states

#define BTNST_NTRL	0  // neutral - initial state nothing interesting, please go along

#define BTNST_DBNC	1  // debounce - pin went up, but we wait for things to stabilize and stop oscillating

#define BTNST_SDN 	4  // affirmative, button is pressed,
			   // and it's pressed no longer than
			   // BTN_T_LONGFACT * BTN_T_DEBOUNCE * 10ms

#define BTNST_SUP	8  // and button went up after beeing pressed for a _short_ time

#define BTNST_LDN  	16  // button is still down for more than
			   //BTN_T_LONGFACT * BTN_T_DEBOUNCE * 10ms

#define BTNST_LUP   	32  // button came up after being pressed for a long time

#define BTNST_SUPDBNC 64 // debounce after short up
#define BTNST_LUPDBNC 128 // debounce after long up

void init_leds(void);
void led_on(int);
void led_off(int);

void init_buzzr(void);
void init_mic(void);
void buzzr_up(void);
void buzzr_down(void);
void buzzr_off(void);
void buzzr_inv(void);

// init ports and stuff for button functions
void init_switch(void);

// direct low level access to switches
bool switch_l(void); //switch pressed?
bool switch_r(void); //switch pressed?

void init_motor(void);
void set_motor(int);


// higher level functions for accessing switches

void button_poll(void); //needs to be polled in regular intervalls (lets say every 10ms)

// reset button to neutral state
void button_clear(uint8_t button);

//test if buttonstate of btn eqals btnstate, returns true if yes
bool btn_state(uint8_t btnstate, uint8_t btn);


#endif
