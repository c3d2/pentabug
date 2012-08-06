#include <inttypes.h>

// sample rate is 8M / (3 * 64)


enum {
	synth_channel_count = 2,
};

enum {
	WAVE_PULSE,
	WAVE_SAW,
};


typedef struct {
	uint8_t	wave;

	uint16_t phase;
	uint16_t speed;

	uint32_t pulse_width;
	uint8_t pulse_sweep;

	uint8_t	level;			// envelop level

} synth_channel_t;

typedef struct {
	synth_channel_t channels[synth_channel_count];
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

		default:
			amp = 0;
			break;
		}

		output += (amp & 0xff) / 2;
	}

	return output;
}

