#include <inttypes.h>
#include "synth.h"
#include "freq_table.h"


uint8_t wave_table[256][2];
synth_instrument_t instruments[256];
uint8_t patterns[256][pattern_length][2];
uint8_t pattern_table[256][channel_count];
uint8_t tune_length;

int16_t sample;
int8_t tick;
int8_t row;
int8_t seq;


static synth_channel_t channels[channel_count];


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
						chan->level = 80; // TODO: less?
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
				if(++seq == tune_length) {
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

