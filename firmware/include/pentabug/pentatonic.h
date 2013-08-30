#ifndef PENTATONIC_H
#define PENTATONIC_H 

#include <stdint.h>

#define ALL_OUT		0xff
#define ALL_IN		0x00

void pentatonic_direction(uint8_t direction);

void pentatonic_led_on(uint8_t led);
void pentatonic_led_off(uint8_t led);

void pentatonic_multi_led_on(uint8_t leds);
void pentatonic_multi_led_off(uint8_t leds);

void pentatonic_led_set(uint8_t led, uint8_t state);
void pentatonic_all_led_set(uint8_t leds);

#endif /* PENTATONIC_H */
