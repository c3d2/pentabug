#ifndef HAL_H
#define HAL_H 

#include <stdint.h>

#define LEFT		0
#define RIGHT		1

#define OFF		0
#define ON		1

// INITIALIZATION

// initializes the hardware (timers, ...)
void init_hw(void);

// resets hardware to a state which is convenient for starting applications
void reset_hw(void);


// BUTTONS

// returns 1 if the button is currently pressed, 0 otherwise
// WARNING: you should really use button_clicked() instead
uint8_t button_state(uint8_t btn);

// returns 1 if the button was clicked since the last call to this function or button_reset()
uint8_t button_clicked(uint8_t btn);
void button_reset(uint8_t btn);

// LEDS

void led_set(uint8_t led, uint8_t state);
void led_on(uint8_t led);
void led_off(uint8_t led);
void led_inv(uint8_t led);

// MOTOR

void motor_set(uint8_t state);
void motor_on(void);
void motor_off(void);
void motor_inv(void);

// BUZZER

void buzzer_up(void);
void buzzer_down(void);
void buzzer_inv(void);
void buzzer_off(void);

// WAITING

// waits the given amount of ms
// WARNING: the time is actually measured in pentamilliseconds which are similar but not identical to regular milliseconds
void wait_ms(uint16_t ms);

#endif /* HAL_H */
