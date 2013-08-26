#ifndef MUSIC_H
#define MUSIC_H 

#include <stdint.h>
#include <stdlib.h>

#define NOTE_PAUSE	0xffff	// Pause
#define NOTE_C		30577 // note C 
#define NOTE_Db		28862 // note C# / Db 
#define NOTE_D		27242 // note D 
#define NOTE_Eb		25713 // note D# / Eb 
#define NOTE_E		24270 // note E 
#define NOTE_F		22908 // note F 
#define NOTE_Gb		21622 // note F# / Gb 
#define NOTE_G		20408 // note G 
#define NOTE_Ab		19263 // note G# / Ab 
#define NOTE_A		18182 // note A 
#define NOTE_Bb		17161 // note A# / Bb 
#define NOTE_B		16198 // note B 

#define MLDY_PAUSE	0xfffe	// user defined pause for melody, the next value is the amount of ms to wait

// plays the note (see defines above) at the given octave until another note is played or the note is stopped
void set_note(uint16_t note, uint8_t octave);

// plays a melody consisting of the array of tunes of length len with speed ms for each tune
void play_melody(uint16_t melody[], size_t len, uint8_t octave, int speed);

// stop the currently playing note
void stop_note(void);

#endif /* MUSIC_H */
