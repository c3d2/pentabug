#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>
#include "freq_table.h"


static void init_sampletimer(void)
{
	/* 
	 * Timer 0
	 */

	/* set timer0 to CTC & prescaler 64 → 125kHz increment */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << CS01);

//	OCR0A = 6;		/* TOP */
	OCR0A = 8;		/* TOP */
	TCNT0 = 0;
	/*enable interrupt */
	TIMSK0 |= (1 << OCIE0A);
}

static inline void init_pwm(void)
{
	/* set PB2 as output (buzzer pwm port): */
	DDRB |= (1 << PORTB2);
        DDRC |= (1 << PORTC5);
        PORTB &= ~(1 << PORTB2);
        PORTC |= (1 << PORTC5);

	/* analog value preselection : */
	OCR1B = 0x007F;

	/* Top value. Timer overflows here. Thus we have a resulting 8bit pwm */
	ICR1 = 0x00FF;

	/* only b-chan , fastpwm (mode 14), no prescale */
	TCCR1A = (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
	
	TIMSK1 |= (OCIE1B);
	return;
}

ISR(TIMER1_COMPB_vect) {
	PORTC ^= (1 << PORTC5);
        PORTB ^= (1 << PORTB2);
}


void synth_init(void){
	cli();
	init_sampletimer();
	init_pwm();
	sei();
	return;
}


void synth_deinit(void) {
	return;
}




enum {
	SONG_LENGTH = 128,
	end = 0,
	c_0 = 0, cs0 = 1, d_0 = 2, ds0 = 3, e_0 = 4, f_0 = 5, fs0 = 6, g_0 = 7, gs0 = 8, a_0 = 9, as0 = 10, b_0 = 11,
	c_1 = 12, cs1 = 13, d_1 = 14, ds1 = 15, e_1 = 16, f_1 = 17, fs1 = 18, g_1 = 19, gs1 = 20, a_1 = 21, as1 = 22, b_1 = 23,
	c_2 = 24, cs2 = 25, d_2 = 26, ds2 = 27, e_2 = 28, f_2 = 29, fs2 = 30, g_2 = 31, gs2 = 32, a_2 = 33, as2 = 34, b_2 = 35,
	c_3 = 36, cs3 = 37, d_3 = 38, ds3 = 39, e_3 = 40, f_3 = 41, fs3 = 42, g_3 = 43, gs3 = 44, a_3 = 45, as3 = 46, b_3 = 47,
};

const char music_data[2][SONG_LENGTH] = {	{
		e_1, end, e_2, end, e_1, end, e_2, end, e_1, end, e_2, end, e_1, end, e_2, end,
		a_0, end, a_1, end, a_0, end, a_1, end, a_0, end, a_1, end, a_0, end, a_1, end,
		gs0, end, gs1, end, gs0, end, gs1, end, gs0, end, gs1, end, gs0, end, gs1, end,
		a_0, end, a_1, end, a_0, end, a_1, end, a_0, end, a_0, end, b_0, end, c_1, end,
		d_1, end, d_2, end, d_1, end, d_2, end, d_1, end, d_2, end, d_1, end, d_2, end,
		c_1, end, c_2, end, c_1, end, c_2, end, c_1, end, c_2, end, c_1, end, c_2, end,
		b_0, end, b_1, end, b_0, end, b_1, end, b_0, end, b_1, end, b_0, end, b_1, end,
		a_0, end, a_1, end, a_0, end, a_1, end, a_0, end, end, end, end, end, end, end,
	}, {
		e_3, e_3, e_3, e_3, b_2, b_2, c_3, c_3, d_3, d_3, e_3, d_3, c_3, c_3, b_2, b_2,
		a_2, a_2, a_2, a_2, a_2, a_2, c_3, c_3, e_3, e_3, e_3, e_3, d_3, d_3, c_3, c_3,
		b_2, b_2, b_2, b_2, b_2, b_2, c_3, c_3, d_3, d_3, d_3, d_3, e_3, e_3, e_3, e_3,
		c_3, c_3, c_3, c_3, a_2, a_2, a_2, a_2, a_2, a_2, a_2, a_2, a_2, a_2, a_2, a_2,
		end, end, d_3, d_3, d_3, d_3, f_3, f_3, a_3, a_3, a_3, a_3, g_3, g_3, f_3, f_3,
		e_3, e_3, e_3, e_3, e_3, e_3, c_3, c_3, e_3, e_3, e_3, e_3, d_3, d_3, c_3, c_3,
		b_2, b_2, b_2, b_2, b_2, b_2, c_3, c_3, d_3, d_3, d_3, d_3, e_3, e_3, e_3, e_3,
		c_3, c_3, c_3, c_3, a_2, a_2, a_2, a_2, a_2, a_2, a_2, a_2, end, end, end, end,
}	};


uint16_t	osc0 = 0;
uint16_t	osc1 = 0;
uint16_t	sample = 0;
uint8_t		row = 0;

static uint8_t synth_mix()
{
	if (music_data[0][row])	osc0 += freq_table[music_data[0][row]];
	if (music_data[1][row]) osc1 += freq_table[music_data[1][row]];

	if (++sample == 2000) {
		sample = 0;
		if (++row == SONG_LENGTH) row = 0;
	}

	return (osc1 >> 9) + ((osc0 >> 15) ? 0 : 120);
}

ISR(TIMER0_COMPA_vect)
{
	/* calculate next analog sample value in synth mixer: */
	OCR1B = synth_mix();
}





