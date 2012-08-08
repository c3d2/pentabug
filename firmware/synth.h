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
	uint8_t		wave_table_pos;
	uint8_t		decay;

} synth_instrument_t;


void synth_init(void);
uint16_t synth_mix(void);

