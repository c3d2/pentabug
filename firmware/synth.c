#include <inttypes.h>
// sample rate is 8M / (3 * 64)


enum { WAVE_PULSE, WAVE_SAW, WAVE_NOISE };

typedef struct {
	uint8_t	wave;

	uint16_t phase;
	uint16_t speed;

	uint32_t pulse_width;
	uint8_t pulse_sweep;

	uint8_t	level;			// envelop level

} synth_channel_t;



enum {
	synth_channel_count = 2,
	tick_length = 500,
	row_length = 6,
	pattern_length = 16
};

typedef struct {
	synth_channel_t channels[synth_channel_count];

	int16_t sample;
	int8_t tick;
	int8_t row;
	int8_t seq;

} synth_t;

static synth_t synth;



void synth_init(void)
{
	// some test values
	synth.channels[0].wave = WAVE_PULSE;
	synth.channels[0].speed = 1153;
	synth.channels[0].pulse_sweep = 0;
	synth.channels[0].pulse_width = 1 << 31;

	synth.channels[1].wave = WAVE_SAW;
	synth.channels[1].speed = 1728;
}

uint16_t synth_mix(void)
{
	if(synth.sample == 0) {
		// new tick
		if(synth.tick == 0) {
			// new row
			if(synth.row == 0) {
				// new pattern
			}
		}
	}
	if(++synth.sample == tick_length) {
		synth.sample = 0;
		if(++synth.tick == row_length) {
			synth.tick = 0;
			if(++synth.row == pattern_length) {
				synth.row = 0;
			}
		}
	}


	uint16_t output = 0;
	for(int i = 0; i < synth_channel_count; i++) {
		synth_channel_t *chan = &synth.channels[i];


		chan->phase += chan->speed;
		chan->pulse_width += chan->pulse_sweep << 8;

		uint8_t amp;
		switch(chan->wave) {
		case WAVE_PULSE:
			amp = -(chan->phase < (chan->pulse_width >> 16));
			break;

		case WAVE_SAW:
			amp = (chan->phase >> 8);
			break;

		case WAVE_NOISE:	 // shitty noise
			chan->phase = (chan->phase >> 1) ^ (-(chan->phase & 1) & 0xb400);
			amp = (chan->phase >> 8);
			break;

		default:
			amp = 0;
			break;
		}

		output += (amp & 0xff) / 2;
	}

	return output;
}

