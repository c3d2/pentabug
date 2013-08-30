#ifndef PENTATONIC_H
#define PENTATONIC_H 

#include <stdint.h>

#define ALL_OUT		0xff
#define ALL_IN		0x00

// every call affecting multiple LEDs/buttons expects a bitmap
// the first 5 bits of this bitmap repressent the LEDs/buttons (from left to right)

// set direction in which to use pentatonic extension, you might use one of the ALL_* from above
// IMPORTANT: you have to call this function before using any other function from this file
void pentatonic_direction(uint8_t direction);

// direct access to single LED
void pentatonic_led_on(uint8_t led);
void pentatonic_led_off(uint8_t led);

// turn multiple LEDs on or off with one call
void pentatonic_multi_led_on(uint8_t leds);
void pentatonic_multi_led_off(uint8_t leds);

// set the state of a single LEDs
void pentatonic_led_set(uint8_t led, uint8_t state);

// sets the state of all LEDs
void pentatonic_all_led_set(uint8_t leds);

// get the state of all buttons
uint8_t pentatonic_buttons(void);

// get the state of a specific button
uint8_t pentatonic_button(uint8_t button);

#endif /* PENTATONIC_H */
