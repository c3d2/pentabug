/* small timer library, uses timer2 */

#ifndef _UTIL_H
#define _UTIL_H

#include <stdint.h>
#include <stdbool.h>

/* structures */
typedef struct {
    uint8_t current;
    uint8_t timeout;
} timer_t;

typedef struct {
    uint16_t current;
    uint16_t timeout;
} timerL_t;


/* functions */
void timer_init(void);
void timer_set(timer_t *t, uint8_t timeout);
bool timer_expired(timer_t *t);
void timerL_set(timerL_t *t, uint16_t timeout);
bool timerL_expired(timerL_t *t);

#endif
