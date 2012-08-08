enum {  WAVE_OFF, WAVE_PULSE, WAVE_SAW, WAVE_NOISE };

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
	uint8_t		decay;
	uint8_t		wave_table_pos;
} synth_instrument_t;


uint16_t synth_mix(void);

enum {
	channel_count = 3,
	tick_length = 400,
	row_length = 4,
	pattern_length = 16
};


extern synth_instrument_t instruments[256];
extern uint8_t wave_table[256][2];
extern uint8_t patterns[256][pattern_length][2];
extern uint8_t pattern_table[256][channel_count];
extern uint8_t tune_length;

extern int16_t sample;
extern int8_t tick;
extern int8_t row;
extern int8_t seq;
