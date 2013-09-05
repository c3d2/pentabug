#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <pentabug/hal.h>
#include <pentabug/helper.h>
#include <pentabug/app.h>
#include <pentabug/timer.h>


static uint16_t osc[3];
static uint16_t sample;
static uint8_t note;
static uint8_t row;


enum {
	NOTE_LENGTH = 550,
	DUTY = 0xf000,
	xxx = 0,
	g_0, gs0, a_0, as0, b_0,
	c_1, cs1, d_1, ds1, e_1, f_1, fs1, g_1, gs1, a_1, as1, b_1,
	c_2, cs2, d_2, ds2, e_2, f_2, fs2, g_2, gs2, a_2, as2, b_2,
	c_3, cs3, d_3, ds3, e_3, f_3, fs3, g_3, gs3, a_3, as3, b_3,
	c_4,
};


const uint8_t patterns[][3][48] PROGMEM = {
	{
		{e_3,e_3,xxx,e_3,e_3,xxx,xxx,xxx,xxx,e_3,e_3,xxx,xxx,xxx,xxx,c_3,c_3,xxx,e_3,e_3,xxx,xxx,xxx,xxx,g_3,g_3,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{fs2,fs2,xxx,fs2,fs2,xxx,xxx,xxx,xxx,fs2,fs2,xxx,xxx,xxx,xxx,fs2,fs2,xxx,fs2,fs2,xxx,xxx,xxx,xxx,b_2,b_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_2,g_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{d_1,d_1,xxx,d_1,d_1,xxx,xxx,xxx,xxx,d_1,d_1,xxx,xxx,xxx,xxx,d_1,d_1,xxx,d_1,d_1,xxx,xxx,xxx,xxx,g_2,g_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
	},{
		{c_3,c_3,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_2,g_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,e_2,e_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,a_2,a_2,xxx,xxx,xxx,xxx,b_2,b_2,xxx,xxx,xxx,xxx,as2,as2,xxx,a_2,a_2,xxx,xxx,xxx,xxx},
		{e_2,e_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_2,c_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_2,c_2,xxx,xxx,xxx,xxx,d_2,d_2,xxx,xxx,xxx,xxx,cs2,cs2,xxx,c_2,c_2,xxx,xxx,xxx,xxx},
		{g_1,g_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,e_1,e_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_1,c_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,f_1,f_1,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,fs1,fs1,xxx,f_1,f_1,xxx,xxx,xxx,xxx},
	},{
		{g_2,g_2,g_2,xxx,e_3,e_3,e_3,xxx,g_3,g_3,g_3,xxx,a_3,a_3,xxx,xxx,xxx,xxx,f_3,f_3,xxx,g_3,g_3,xxx,xxx,xxx,xxx,e_3,e_3,xxx,xxx,xxx,xxx,c_3,c_3,xxx,d_3,d_3,xxx,b_2,b_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{c_2,c_2,c_2,xxx,g_2,g_2,g_2,xxx,b_2,b_2,b_2,xxx,c_3,c_3,xxx,xxx,xxx,xxx,a_2,a_2,xxx,b_2,b_2,xxx,xxx,xxx,xxx,a_2,a_2,xxx,xxx,xxx,xxx,e_2,e_2,xxx,f_2,f_2,xxx,d_2,d_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{e_1,e_1,e_1,xxx,c_2,c_2,c_2,xxx,e_2,e_2,e_2,xxx,f_2,f_2,xxx,xxx,xxx,xxx,d_2,d_2,xxx,e_2,e_2,xxx,xxx,xxx,xxx,c_2,c_2,xxx,xxx,xxx,xxx,a_1,a_1,xxx,b_1,b_1,xxx,g_1,g_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
	},{
		{xxx,xxx,xxx,xxx,xxx,xxx,g_3,g_3,xxx,fs3,fs3,xxx,f_3,f_3,xxx,ds3,ds3,xxx,xxx,xxx,xxx,e_3,e_3,xxx,xxx,xxx,xxx,gs2,gs2,xxx,a_2,a_2,xxx,c_3,c_3,xxx,xxx,xxx,xxx,a_2,a_2,xxx,c_3,c_3,xxx,d_3,d_3,xxx},
		{xxx,xxx,xxx,xxx,xxx,xxx,e_3,e_3,xxx,ds3,ds3,xxx,d_3,d_3,xxx,b_2,b_2,xxx,xxx,xxx,xxx,c_3,c_3,xxx,xxx,xxx,xxx,e_2,e_2,xxx,f_2,f_2,xxx,g_2,g_2,xxx,xxx,xxx,xxx,c_2,c_2,xxx,e_2,e_2,xxx,f_2,f_2,xxx},
		{c_1,c_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_2,c_2,xxx,xxx,xxx,xxx,f_1,f_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_2,c_2,xxx,c_2,c_2,xxx,xxx,xxx,xxx,f_1,f_1,xxx,xxx,xxx,xxx},
	},{
		{xxx,xxx,xxx,xxx,xxx,xxx,g_3,g_3,xxx,fs3,fs3,xxx,f_3,f_3,xxx,ds3,ds3,xxx,xxx,xxx,xxx,e_3,e_3,xxx,xxx,xxx,xxx,c_4,c_4,xxx,xxx,xxx,xxx,c_4,c_4,xxx,c_4,c_4,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{xxx,xxx,xxx,xxx,xxx,xxx,e_3,e_3,xxx,ds3,ds3,xxx,d_3,d_3,xxx,b_2,b_2,xxx,xxx,xxx,xxx,c_3,c_3,xxx,xxx,xxx,xxx,g_3,g_3,xxx,xxx,xxx,xxx,g_3,g_3,xxx,g_3,g_3,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{c_1,c_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,e_1,e_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_1,g_1,xxx,c_2,c_2,xxx,xxx,xxx,xxx,f_3,f_3,xxx,xxx,xxx,xxx,f_3,f_3,xxx,f_3,f_3,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx},
	},{
		{xxx,xxx,xxx,xxx,xxx,xxx,ds3,ds3,xxx,xxx,xxx,xxx,xxx,xxx,xxx,d_3,d_3,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_3,c_3,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{xxx,xxx,xxx,xxx,xxx,xxx,gs2,gs2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,f_2,f_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,e_2,e_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{c_1,c_1,xxx,xxx,xxx,xxx,gs1,gs1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,as1,as1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_2,c_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_1,g_1,xxx,g_1,g_1,xxx,xxx,xxx,xxx,c_1,c_1,xxx,xxx,xxx,xxx},
	},{
		{c_3,c_3,xxx,c_3,c_3,xxx,xxx,xxx,xxx,c_3,c_3,xxx,xxx,xxx,xxx,c_3,c_3,xxx,d_3,d_3,xxx,xxx,xxx,xxx,e_3,e_3,xxx,c_3,c_3,xxx,xxx,xxx,xxx,a_2,a_2,xxx,g_2,g_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{gs2,gs2,xxx,gs2,gs2,xxx,xxx,xxx,xxx,gs2,gs2,xxx,xxx,xxx,xxx,gs2,gs2,xxx,as2,as2,xxx,xxx,xxx,xxx,g_2,g_2,xxx,e_2,e_2,xxx,xxx,xxx,xxx,e_2,e_2,xxx,c_2,c_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{gs0,gs0,xxx,xxx,xxx,xxx,xxx,xxx,xxx,ds1,ds1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,gs1,gs1,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_1,c_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_0,g_0,xxx,xxx,xxx,xxx},
	},{
		{c_3,c_3,xxx,c_3,c_3,xxx,xxx,xxx,xxx,c_3,c_3,xxx,xxx,xxx,xxx,c_3,c_3,xxx,d_3,d_3,xxx,e_3,e_3,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{gs2,gs2,xxx,gs2,gs2,xxx,xxx,xxx,xxx,gs2,gs2,xxx,xxx,xxx,xxx,gs2,gs2,xxx,as2,as2,xxx,g_2,g_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{gs0,gs0,xxx,xxx,xxx,xxx,xxx,xxx,xxx,ds1,ds1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,gs1,gs1,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,c_1,c_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_0,g_0,xxx,xxx,xxx,xxx},
	},{
		{e_3,e_3,xxx,c_3,c_3,xxx,xxx,xxx,xxx,g_2,g_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,gs2,gs2,xxx,xxx,xxx,xxx,a_2,a_2,xxx,f_3,f_3,xxx,xxx,xxx,xxx,f_3,f_3,xxx,a_2,a_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{c_3,c_3,xxx,a_2,a_2,xxx,xxx,xxx,xxx,e_2,e_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,e_2,e_2,xxx,xxx,xxx,xxx,f_2,f_2,xxx,c_3,c_3,xxx,xxx,xxx,xxx,c_3,c_3,xxx,f_2,f_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{c_1,c_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,fs1,fs1,xxx,g_1,g_1,xxx,xxx,xxx,xxx,c_2,c_2,xxx,xxx,xxx,xxx,f_1,f_1,xxx,xxx,xxx,xxx,f_1,f_1,xxx,xxx,xxx,xxx,c_2,c_2,xxx,c_2,c_2,xxx,f_1,f_1,xxx,xxx,xxx,xxx},
	},{
		{b_2,b_2,b_2,xxx,a_3,a_3,a_3,xxx,a_3,a_3,a_3,xxx,a_3,a_3,a_3,xxx,g_3,g_3,g_3,xxx,f_3,f_3,f_3,xxx,e_3,e_3,xxx,c_3,c_3,xxx,xxx,xxx,xxx,a_2,a_2,xxx,g_2,g_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{g_2,g_2,g_2,xxx,f_3,f_3,f_3,xxx,f_3,f_3,f_3,xxx,f_3,f_3,f_3,xxx,e_3,e_3,e_3,xxx,d_3,d_3,d_3,xxx,c_3,c_3,xxx,a_2,a_2,xxx,xxx,xxx,xxx,f_2,f_2,xxx,e_2,e_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{d_1,d_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,f_1,f_1,xxx,g_1,g_1,xxx,xxx,xxx,xxx,b_1,b_1,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,c_2,c_2,xxx,c_2,c_2,xxx,g_1,g_1,xxx,xxx,xxx,xxx},
	},{
		{b_2,b_2,xxx,f_3,f_3,xxx,xxx,xxx,xxx,f_3,f_3,xxx,f_3,f_3,f_3,xxx,e_3,e_3,e_3,xxx,d_3,d_3,d_3,xxx,c_3,c_3,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{g_2,g_2,xxx,d_3,d_3,xxx,xxx,xxx,xxx,d_3,d_3,xxx,d_3,d_3,d_3,xxx,c_3,c_3,c_3,xxx,b_2,b_2,b_2,xxx,g_2,g_2,xxx,e_2,e_2,xxx,xxx,xxx,xxx,e_2,e_2,xxx,c_2,c_2,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{g_1,g_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_1,g_1,xxx,g_1,g_1,g_1,xxx,a_1,a_1,a_1,xxx,b_1,b_1,b_1,xxx,g_2,g_2,xxx,xxx,xxx,xxx,g_1,g_1,xxx,xxx,xxx,xxx,c_1,c_1,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
	}
};


const uint16_t freq[] PROGMEM = {
	0,
	653, 692, 733, 777, 823, 872,
	924, 979, 1037, 1099, 1164, 1233,
	1306, 1384, 1466, 1554, 1646, 1744,
	1848, 1957, 2074, 2197, 2328, 2466,
	2613, 2768, 2933, 3107, 3292, 3488,
	3695, 3915, 4148, 4394, 4656, 4933,
	5226, 5537, 5866, 6215, 6584, 6976
};


const uint8_t order[] = {
	0,
	1, 2, 1, 2,
	3, 4, 3, 5,
	3, 4, 3, 5,
	6, 7, 6, 0,
	1, 2, 1, 2,
	8, 9, 8, 10,
	8, 9, 8, 10,
	6, 7, 6, 0,
	8, 9, 8, 10
};


static void mix_mario(void) {

	if (++sample >= NOTE_LENGTH) {
		sample = 0;
		if (++note >= 48) {
			note = 0;
			if (++row >= ARRAY_SIZE(order)) row = 1; // skip 0th row
		}
	}

	uint8_t	p = order[row];
	uint8_t n;
	static uint8_t prev_n = 0;


	n = pgm_read_byte(&patterns[p][0][note]);
	if (n == 0) osc[0] = 0;
	osc[0] += pgm_read_word(&freq[n]);
	if (n != prev_n) led_set(RIGHT, (n > 0)); // blink
	prev_n = n;


	n = pgm_read_byte(&patterns[p][1][note]);
	if (n == 0) osc[1] = 0;
	osc[1] += pgm_read_word(&freq[n]);

	n = pgm_read_byte(&patterns[p][2][note]);
	if (n == 0) osc[2] = 0;
	osc[2] += pgm_read_word(&freq[n]);

	uint8_t amp = (osc[0] > DUTY) | (osc[1] > DUTY) | (osc[2] > DUTY);

	static uint8_t prev_amp = 0;
	if (amp != prev_amp) {
		buzzer_inv();
		prev_amp = amp;
	}

}

static void init_mario(void) {
	sample = 0;
	note = 0;
	row = 0;
	start_timer(PRESCALE_8, 100, mix_mario);
	led_on(RIGHT);
}

static void run_mario(void) {}

REGISTER(run_mario, init_mario, NULL);
