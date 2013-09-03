#include <stdlib.h>

#include <pentabug/music.h>
#include <pentabug/hal.h>
#include <pentabug/helper.h>
#include <pentabug/app.h>


static uint16_t notes[] = {
	NOTE_C, NOTE_D, NOTE_E, NOTE_F, MLDY_LENGTH, 2, NOTE_G, MLDY_LENGTH, 2, NOTE_G,
	NOTE_A, NOTE_A, NOTE_A, NOTE_A, MLDY_LENGTH, 3, NOTE_G, NOTE_PAUSE,
	NOTE_A, NOTE_A, NOTE_A, NOTE_A, MLDY_LENGTH, 3, NOTE_G, NOTE_PAUSE,
	NOTE_F, NOTE_F, NOTE_F, NOTE_F, MLDY_LENGTH, 2, NOTE_E, MLDY_LENGTH, 2, NOTE_E,
	NOTE_G, NOTE_G, NOTE_G, NOTE_G, MLDY_LENGTH, 3, NOTE_C, NOTE_PAUSE,
};

static void music(void) {
	play_melody(notes, ARRAY_SIZE(notes), 4, 300);
}

REG(music);
