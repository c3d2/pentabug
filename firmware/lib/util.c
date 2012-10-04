
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/io.h>
#include "util.h"

static volatile uint8_t internal_counter;

void timer_init(void)
{
    /* initialize timer2, CTC at 10ms, prescaler 1024 */
    OCR2A = F_CPU/1024/100;
    TCCR2A = _BV(WGM21);
    TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
    TIMSK2 = _BV(OCIE2A);
}

void timer_set(timer_t *t, uint8_t timeout)
{
    t->current = internal_counter;
    t->timeout = timeout;
}

void timerL_set(timerL_t *t, uint16_t timeout)
{
    t->current = internal_counter;
    t->timeout = timeout;
}




bool timer_expired(timer_t *t)
{
    if (t->timeout == 0)
        return true;

    /* attention: may fail if internal counter is incremented by more than one
     * between two calls of timer_expired()! (that is if its is called less than every 10ms*/
    if (t->current != internal_counter) {
        t->timeout--;
        t->current = internal_counter;
    }

    return false;
}


bool timerL_expired(timerL_t *t)
{
    if (t->timeout == 0)
        return true;

    /* attention: may fail if internal counter is incremented by more than one
     * between two calls of timer_expired()! */
    if (t->current != internal_counter) {
        t->timeout--;
        t->current = internal_counter;
    }

    return false;
}



/* timer interrupt function */
ISR(TIMER2_COMPA_vect, ISR_NOBLOCK) {
    internal_counter++;
}

