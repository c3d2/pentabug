#include <inttypes.h>
#include "freq_table.h"
// sample rate is 8M / (3 * 64)


enum {  WAVE_OFF, WAVE_PULSE, WAVE_SAW, WAVE_NOISE };
enum {
	channel_count = 3,
	tick_length = 800,
	row_length = 4,
	pattern_length = 16
};

typedef struct {
	uint8_t	note;
	uint8_t	inst_nr;
	uint8_t pos;

	uint16_t phase;
	uint16_t pulse_width;

	uint8_t	level;			// envelop level

} synth_channel_t;


typedef struct {
	uint16_t	pulse_width;
	uint8_t		pulse_sweep;
	uint8_t		wave_table_pos;
	uint8_t		decay;

} synth_instrument_t;


static const synth_instrument_t instruments[] = {
	{ 1<<15, 100, 12 },
	{ 0, 100, 12 },
	{ 0, 200, 10 },
	{ 1<<13, 0, 0, 2 },
	{ 1<<13, 0, 5, 2 },
};

static const uint8_t wave_table[][2] = {
	{  0, WAVE_PULSE },
	{  3, WAVE_PULSE },
	{  7, WAVE_PULSE },
	{  12, WAVE_PULSE },
	{  256 - 4, 0xff },

	{  0, WAVE_PULSE },
	{  2, WAVE_PULSE },
	{  7, WAVE_PULSE },
	{  10, WAVE_PULSE },
	{  256 - 4, 0xff },

	{  0, WAVE_NOISE },
	{  0, WAVE_PULSE },
	{  0xff, 0xff },

	{  0, WAVE_PULSE },
	{  0xff, 0xff },

};


static const uint8_t patterns[][pattern_length][2] = {
	{},
	{
		{   33 - 12, 0 },
		{ 0xff, 1 },
		{   33 - 12, 1 },
		{ 0xff, 1 },
		{   33, 1 },
		{ 0xff, 1 },
		{   33, 1 },
		{ 0xff, 1 },
		{   33, 1 },
		{ 0xff, 1 },
		{   33 - 12, 1 },
		{ 0xff, 1 },
		{   33 - 12, 1 },
		{ 0xff, 1 },
		{   33, 1 },
		{ 0xff, 1 },
	},
	{
		{   28 - 12, 0 },
		{ 0xff, 1 },
		{   28 - 12, 1 },
		{ 0xff, 1 },
		{   28, 1 },
		{ 0xff, 1 },
		{   28, 1 },
		{ 0xff, 1 },
		{   28, 1 },
		{ 0xff, 1 },
		{   28 - 12, 1 },
		{ 0xff, 1 },
		{   28 - 12, 1 },
		{ 0xff, 1 },
		{   28, 1 },
		{ 0xff, 1 },
	},
	{
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 57, 3 },
	},
	{
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 57, 4 },
	},


	{
		{ 60, 2 },
	},
	{
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 57, 2 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 55, 2 },
		{ 0, 0 },
		{ 57, 2 },
		{ 0, 0 },
	},
	{
		{ 55, 2 },
	},
	{
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 57, 2 },
	},
	{
		{ 55-3, 2 },
	},




};

static const uint8_t pattern_table[][channel_count] = {
	{ 1, 0, 5 },
	{ 1, 3, 0 },
	{ 1, 0, 7 },
	{ 1, 3, 6 },
	{ 2, 0, 7 },
	{ 2, 4, 8 },
	{ 2, 0, 9 },
	{ 2, 4, 0 },
};
enum {
	pattern_table_length = sizeof(pattern_table) / sizeof(pattern_table[0])
};


static synth_channel_t channels[channel_count];
static int16_t sample;
static int8_t tick;
static int8_t row;
static int8_t seq;


void synth_init(void)
{
	sample = 0;
	tick = 0;
	row = 0;
	seq = 0;

/*
	// some test values
	channels[0].wave = WAVE_PULSE;
	channels[0].pulse_width = 1 << 15;

	channels[1].wave = WAVE_SAW;
*/
}

uint16_t synth_mix(void)
{
	if(sample == 0) { // new tick
		for(int i = 0; i < channel_count; i++) {
			synth_channel_t* chan = &channels[i];

			const synth_instrument_t* inst = &instruments[chan->inst_nr];

			if(chan->level > inst->decay) chan->level -= inst->decay;
			else chan->level = 0;

			chan->pulse_width += inst->pulse_sweep;


			chan->pos++;
			if(wave_table[chan->pos][1] == 0xff) chan->pos += wave_table[chan->pos][0];


			// enter new row
			if(tick == 0) {
				uint8_t pattern_nr = pattern_table[seq][i];
				uint8_t note = patterns[pattern_nr][row][0];

				if(note) { // new note, maybe?
					if(note == 0xff) {
						chan->level = 0;
					} else {
						chan->level = 100;
						chan->note = note;
						chan->inst_nr = patterns[pattern_nr][row][1];
						inst = &instruments[chan->inst_nr];
						chan->pos = inst->wave_table_pos;
						if(inst->pulse_width) chan->pulse_width = inst->pulse_width;
					}
				}
			}
		}
	}
	if(++sample == tick_length) {
		sample = 0;
		if(++tick == row_length) {
			tick = 0;
			if(++row == pattern_length) {
				row = 0;
				if(++seq == pattern_table_length) {
					seq = 0;
				}
			}
		}
	}


	uint16_t output = 0;
	for(int i = 0; i < channel_count; i++) {
		synth_channel_t* chan = &channels[i];
		const synth_instrument_t* inst = &instruments[chan->inst_nr];

		chan->phase += freq_table[(uint8_t)(chan->note + wave_table[chan->pos][0])];

		uint8_t amp;
		switch(wave_table[chan->pos][1]) {
		case WAVE_PULSE:
			amp = -(chan->phase < chan->pulse_width);
			break;

		case WAVE_SAW:
			amp = (chan->phase >> 8);
			break;

		case WAVE_NOISE: // shitty noise
			chan->phase = (chan->phase >> 1) ^ (-(chan->phase & 1) & 0xb400);
			amp = (chan->phase >> 8);
			break;

		default:
			amp = 0;
			break;
		}

		output += ((amp & 0xff) * chan->level) >> 8;
	}

	return output;
}

