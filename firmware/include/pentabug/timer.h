#ifndef TIMER_H
#define TIMER_H 

#include <stdint.h>

// this library addresses the 16 bit timer

#define PRESCALE_1		(1 << CS10)
#define PRESCALE_8		(1 << CS11)
#define PRESCALE_64		(1 << CS11 | 1 << CS10)
#define PRESCALE_256		(1 << CS12)
#define PRESCALE_1024		(1 << CS12 | 1 << CS10)

typedef void (*timer_fun)(void);

void start_timer(uint8_t scaler, uint16_t compare, timer_fun fun);

void stop_timer(void);

#endif /* TIMER_H */
