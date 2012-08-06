#include <inttypes.h>

// sample rate is 8M / (3 * 64)


enum {
	synth_channel_count = 2
};

typedef struct {
	uint16_t phase;
	uint16_t speed;

} synth_channel_t;

typedef struct {
	synth_channel_t channels[synth_channel_count];
} synth_t;

static synth_t synth;


void synth_init(void)
{
	// some test values
	synth.channels[0].phase = 0;
	synth.channels[0].speed = 1153;

	synth.channels[1].phase = 0;
	synth.channels[1].speed = 1728;
}

uint16_t synth_mix(void)
{
	uint16_t output = 0;

	for (int i = 0; i < synth_channel_count; i++) {
		synth_channel_t *chan = &synth.channels[i];
		chan->phase += chan->speed;
		output += ((chan->phase >> 8) & 0xff) / 2;
	}

	return output;
}

