#include <stdlib.h>

#include <pentabug/music.h>
#include <pentabug/hal.h>
#include <pentabug/helper.h>
#include <pentabug/app.h>

#define NOTE_BREAK		MLDY_LENGTH, 16, NOTE_PAUSE

static uint16_t notes[] = {
	NOTE_C, NOTE_D, NOTE_E, NOTE_F, MLDY_LENGTH, 2, NOTE_G, MLDY_LENGTH, 2, NOTE_G, NOTE_BREAK,
	NOTE_A, NOTE_A, NOTE_A, NOTE_A, MLDY_LENGTH, 2, NOTE_G, NOTE_BREAK,
	NOTE_A, NOTE_A, NOTE_A, NOTE_A, MLDY_LENGTH, 2, NOTE_G, NOTE_BREAK,
	NOTE_F, NOTE_F, NOTE_F, NOTE_F, MLDY_LENGTH, 2, NOTE_E, MLDY_LENGTH, 2, NOTE_E, NOTE_BREAK,
	NOTE_D, NOTE_D, NOTE_D, NOTE_D, MLDY_LENGTH, 1, NOTE_C, NOTE_BREAK,
	NOTE_PAUSE, NOTE_PAUSE, NOTE_PAUSE,
};

static void music(void) {
	play_melody(notes, ARRAY_SIZE(notes), 4, 500);
}

REG(music);
