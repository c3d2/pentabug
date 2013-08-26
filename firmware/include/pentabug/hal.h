#ifndef HAL_H
#define HAL_H 

#include <stdint.h>

#define BUTTON_L	0
#define BUTTON_R	1

void init_hw(void);
void reset_hw(void);

int8_t button_state(void);

#endif /* HAL_H */
