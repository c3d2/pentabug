#include <stdlib.h>
#include <pentabug/hal.h>
#include <pentabug/helper.h>
#include <pentabug/app.h>
#include <pentabug/timer.h>

#include <avr/io.h>
#include <avr/pgmspace.h>


static uint16_t osc[3];
static uint16_t speed[3];
static uint16_t sample;
static uint8_t note;
static uint8_t row;

enum {
	NOTE_LENGTH = 600,
	end = 0,
	xxx = 0,

	g_0, gs0, a_0, as0, b_0,
	c_1, cs1, d_1, ds1, e_1, f_1, fs1, g_1, gs1, a_1, as1, b_1,
	c_2, cs2, d_2, ds2, e_2, f_2, fs2, g_2, gs2, a_2, as2, b_2,
	c_3, cs3, d_3, ds3, e_3, f_3, fs3, g_3, gs3, a_3, as3, b_3,
	c_4,
};

const int patterns[][3][48] PROGMEM = {
	{
		{e_3,xxx,end,e_3,xxx,end,xxx,xxx,xxx,e_3,xxx,end,xxx,xxx,xxx,c_3,xxx,end,e_3,xxx,end,xxx,xxx,xxx,g_3,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{fs2,xxx,end,fs2,xxx,end,xxx,xxx,xxx,fs2,xxx,end,xxx,xxx,xxx,fs2,xxx,end,fs2,xxx,end,xxx,xxx,xxx,b_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{d_1,xxx,end,d_1,xxx,end,xxx,xxx,xxx,d_1,xxx,end,xxx,xxx,xxx,d_1,xxx,end,d_1,xxx,end,xxx,xxx,xxx,g_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,g_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx}
	},
	{
		{c_3,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,g_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,e_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,a_2,xxx,end,xxx,xxx,xxx,b_2,xxx,end,xxx,xxx,xxx,as2,xxx,end,a_2,xxx,end,xxx,xxx,xxx},
		{e_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,c_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,g_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,c_2,xxx,end,xxx,xxx,xxx,d_2,xxx,end,xxx,xxx,xxx,cs2,xxx,end,c_2,xxx,end,xxx,xxx,xxx},
		{g_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,e_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,c_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,f_1,xxx,end,xxx,xxx,xxx,g_1,xxx,end,xxx,xxx,xxx,fs1,xxx,end,f_1,xxx,end,xxx,xxx,xxx}
	},
	{
		{g_2,xxx,xxx,end,e_3,xxx,xxx,end,g_3,xxx,xxx,end,a_3,xxx,end,xxx,xxx,xxx,f_3,xxx,end,g_3,xxx,end,xxx,xxx,xxx,e_3,xxx,end,xxx,xxx,xxx,c_3,xxx,end,d_3,xxx,end,b_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx},
		{c_2,xxx,xxx,end,g_2,xxx,xxx,end,b_2,xxx,xxx,end,c_3,xxx,end,xxx,xxx,xxx,a_2,xxx,end,b_2,xxx,end,xxx,xxx,xxx,a_2,xxx,end,xxx,xxx,xxx,e_2,xxx,end,f_2,xxx,end,d_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx},
		{e_1,xxx,xxx,end,c_2,xxx,xxx,end,e_2,xxx,xxx,end,f_2,xxx,end,xxx,xxx,xxx,d_2,xxx,end,e_2,xxx,end,xxx,xxx,xxx,c_2,xxx,end,xxx,xxx,xxx,a_1,xxx,end,b_1,xxx,end,g_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx}
	},
	{

		{xxx,xxx,xxx,xxx,xxx,xxx,g_3,xxx,end,fs3,xxx,end,f_3,xxx,end,ds3,xxx,end,xxx,xxx,xxx,e_3,xxx,end,xxx,xxx,xxx,gs2,xxx,end,a_2,xxx,end,c_3,xxx,end,xxx,xxx,xxx,a_2,xxx,end,c_3,xxx,end,d_3,xxx,end},
		{xxx,xxx,xxx,xxx,xxx,xxx,e_3,xxx,end,ds3,xxx,end,d_3,xxx,end,b_2,xxx,end,xxx,xxx,xxx,c_3,xxx,end,xxx,xxx,xxx,e_2,xxx,end,f_2,xxx,end,g_2,xxx,end,xxx,xxx,xxx,c_2,xxx,end,e_2,xxx,end,f_2,xxx,end},
		{c_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,g_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,c_2,xxx,end,xxx,xxx,xxx,f_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,c_2,xxx,end,c_2,xxx,end,xxx,xxx,xxx,f_1,xxx,end,xxx,xxx,xxx}
	},
	{
		{xxx,xxx,xxx,xxx,xxx,xxx,g_3,xxx,end,fs3,xxx,end,f_3,xxx,end,ds3,xxx,end,xxx,xxx,xxx,e_3,xxx,end,xxx,xxx,xxx,c_4,xxx,end,xxx,xxx,xxx,c_4,xxx,end,c_4,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{xxx,xxx,xxx,xxx,xxx,xxx,e_3,xxx,end,ds3,xxx,end,d_3,xxx,end,b_2,xxx,end,xxx,xxx,xxx,c_3,xxx,end,xxx,xxx,xxx,g_3,xxx,end,xxx,xxx,xxx,g_3,xxx,end,g_3,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{c_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,e_1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,g_1,xxx,end,c_2,xxx,end,xxx,xxx,xxx,f_3,xxx,end,xxx,xxx,xxx,f_3,xxx,end,f_3,xxx,end,xxx,xxx,xxx,g_1,xxx,end,xxx,xxx,xxx}
	},
	{
		{xxx,xxx,xxx,xxx,xxx,xxx,ds3,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,d_3,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,c_3,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{xxx,xxx,xxx,xxx,xxx,xxx,gs2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,f_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,e_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx,xxx},
		{c_1,xxx,end,xxx,xxx,xxx,gs1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,as1,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,c_2,xxx,end,xxx,xxx,xxx,xxx,xxx,xxx,g_1,xxx,end,g_1,xxx,end,xxx,xxx,xxx,c_1,xxx,end,xxx,xxx,xxx}
	},
};



uint16_t freq[] = {
	0,
	412,
	436,
	462,
	489,
	518,
	549,
	582,
	617,
	653,
	692,
	733,
	777,
	823,
	872,
	924,
	979,
	1037,
	1099,
	1164,
	1233,
	1306,
	1384,
	1466,
	1554,
	1646,
	1744,
	1848,
	1957,
	2074,
	2197,
	2328,
	2466,
	2613,
	2768,
	2933,
	3107,
	3292,
	3488,
	3695,
	3915,
	4148,
	4394,
};


uint8_t order[] = { //0, 1, 2, 1, 2
	0,
	1, 2, 1, 2,
	3, 4, 3, 5,
	3, 4, 3, 5,
};

static uint8_t prev_amp = 0;

static void mix_mario(void) {

	if (++sample >= NOTE_LENGTH) {
		sample = 0;
		if (++note >= 48) {
			note = 0;
			if (++row >= ARRAY_SIZE(order)) row = 1;
		}
	}


	uint8_t	p = order[row];

	osc[0] += freq[pgm_read_byte(&patterns[p][0][note])];
	osc[1] += freq[pgm_read_byte(&patterns[p][1][note])];
	osc[2] += freq[pgm_read_byte(&patterns[p][2][note])];

	uint8_t amp =	(osc[0] > 0xf000) |
					(osc[1] > 0xf000) |
					(osc[2] > 0xf000);


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
}

static void run_mario(void) {}

REGISTER(run_mario, init_mario, NULL);
