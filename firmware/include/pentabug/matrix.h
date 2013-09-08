#ifndef MATRIX_H
#define MATRIX_H 

#include <stdint.h>

void matrix_start(void);
void matrix_stop(void);

void matrix_show(void);
void matrix_init(void);

void matrix_set(uint8_t x, uint8_t y, uint8_t active);
void matrix_brightness(uint8_t x, uint8_t y, uint8_t brightness);

#endif /* MATRIX_H */
