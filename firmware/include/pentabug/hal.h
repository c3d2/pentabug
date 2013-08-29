#ifndef HAL_H
#define HAL_H 

#include <stdint.h>

#define LEFT		0
#define RIGHT		1


// INITIALIZATION

// initializes the hardware (timers, ...)
void init_hw(void);

// resets hardware to a state which is convenient for starting applications
void reset_hw(void);


// BUTTONS

// returns 1 if the button is pressed, 0 otherwise
uint8_t button_state(uint8_t btn);


// LEDS

void led_on(uint8_t led);
void led_off(uint8_t led);
void led_inv(uint8_t led);

// MOTOR

void motor_on(void);
void motor_off(void);

// WAITING

void wait_ms(uint16_t ms);

#endif /* HAL_H */
