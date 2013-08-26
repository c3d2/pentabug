#include <pentabug/timer.h>

#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

static timer_fun current_fun = NULL;

void start_timer(uint8_t scaler, uint16_t compare, timer_fun fun) {
	current_fun = fun;

	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | scaler;
	TIMSK1 = 1 << OCIE1A;
	OCR1A = compare;
}

void stop_timer(void) {
	current_fun = NULL;

	TIMSK1 = 0;
	TCCR1A = 0;
	TCCR1B = 0;
}

ISR(TIMER1_COMPA_vect, ISR_NOBLOCK) {
	if(current_fun) {
		current_fun();
	}
}

