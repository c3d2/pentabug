#include <inttypes.h>
#include "synth.h"
#include "freq_table.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

// sample rate is 8M / (3 * 64)

enum {
	channel_count = 3,
	tick_length = 400,
	row_length = 4,
	pattern_length = 16
};



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


static const uint8_t patterns[][pattern_length][2] PROGMEM = {
	{},
	{
		{   33 - 12, 0 },
		{ 0, 0 },
		{ 0xff, 1 },
		{ 0, 0 },
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
		{ 0, 0 },
		{ 0xff, 1 },
		{ 0, 0 },
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


/* PROTOTYPES */
uint16_t synth_mix(void);

static uint16_t timeslots[SYNTH_BUFSIZE];
static uint8_t timeslots_write; // current write head 
static uint8_t timeslots_read; // current read head 

void enqueue_timeslot(uint16_t synthval);
uint16_t dequeue_timeslot(void);
uint8_t timeslots_fill(void);


void synth_init(void)
{
	sample = 0;
	tick = 0;
	row = 0;
	seq = 0;
	//prefill timeslot buffer
	enqueue_timeslot(synth_mix());
	enqueue_timeslot(synth_mix());
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
				uint8_t note = pgm_read_byte(&patterns[pattern_nr][row][0]);

				if(note) { // new note, maybe?
					if(note == 0xff) {
						chan->level = 0;
					} else {
						chan->level = 80; // TODO: less?
						chan->note = note;
						chan->inst_nr = pgm_read_byte(&patterns[pattern_nr][row][1]);
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

		chan->phase += pgm_read_word(&freq_table[(uint8_t)(chan->note + wave_table[chan->pos][0])]);

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


/* fill all the timeslots */
void synth_poll(void) {
	/* refill timeslots queue */
	while (timeslots_fill() < (SYNTH_BUFSIZE-1))
		enqueue_timeslot(synth_mix());
}

/* timeslot queue handling */
void enqueue_timeslot(uint16_t synthval) {
	timeslots[timeslots_write] = synthval;
	timeslots_write++;
	timeslots_write &= SYNTH_BUFMASK;

}

uint16_t dequeue_timeslot() {
	uint16_t t = timeslots[timeslots_read];
	PORTC = (timeslots_read != timeslots_write) ? 0b00000001 : 0b00000000;
	timeslots_read++;
	if (timeslots_read >= SYNTH_BUFSIZE) timeslots_read =0;

	return t;
}

uint8_t timeslots_fill() {
	if (timeslots_write >= timeslots_read)
		return timeslots_write - timeslots_read;
	else
		return SYNTH_BUFSIZE - (timeslots_read - timeslots_write);
}


ISR(TIMER0_COMPA_vect)
{
	/* calculate next analog sample value in synth mixer:*/
	OCR1B = dequeue_timeslot();
}


