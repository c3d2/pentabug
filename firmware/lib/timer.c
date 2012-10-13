#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include <lib/timer.h>

timer_cb cur_cb;

void start_timer(timer_cb cb) {
	cur_cb = cb;

	cli();

	/* set timer0 to CTC & prescaler 64 → 125kHz increment */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << CS01);

	OCR0A = 4;  /* TOP */
	TCNT0 = 0;

	/*enable interrupt */
	TIMSK0 |= (1 << OCIE0A);

	sei();
}

void stop_timer(void) {
	cli();
	TIMSK0 = 0;
	sei();

	cur_cb = NULL;
}

ISR(TIMER0_COMPA_vect,ISR_NOBLOCK) {
	if(cur_cb != NULL) {
		cur_cb();
	}
}

