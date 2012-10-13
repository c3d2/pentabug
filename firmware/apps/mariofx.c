#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <lib/bughal.h>
#include <lib/freq_table.h>
#include <lib/apps.h>

static uint16_t osc;
static uint16_t sample;
static uint8_t row;


#define LENGTH(x) (sizeof(x) / sizeof(x[0]))
static const uint8_t mushroom[] = {6,1,6,10,13,18,13,1,6,8,13,8,13,18,20,25,20,3,8,11,15,11,15,20,23,27,23};

static void mariofx_ISR(void) {
	if (row < LENGTH(mushroom)) {
		osc += pgm_read_word(&freq_table[mushroom[row] + 18]) * 3;
		if (++sample == 800 ) {
			sample = 0;
			row++;
		}
	} else {
		if (btn_state(BTNST_SUP, BTN_RIGHT)) {
			button_clear(BTN_RIGHT);
			row = 0;
		}
	}

	if (osc >= 0xc000) {
		PORTB |= (1 << PORTB2);
		PORTC &= ~(1<< PORTC5);
	}
	else {
		PORTB &= ~(1<< PORTB2);
		PORTC |= (1 << PORTC5);
	}
}



static void mariofx(void) {
	/* initialisation required */
	if (mode_uninitialized) {
		mode_uninitialized = false;

		init_buzzr();
		osc = sample = row = 0;
		start_timer(mariofx_ISR);
	}

	/*deinialisation required */
	if (mode_last_tick) {
		stop_timer();
	}

	return;
}


REGISTER(mariofx);
