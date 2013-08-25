/* small timer library, uses timer2 */

#ifndef _MUSIC_H
#define _MUSIC_H

#define NOTE_PAUSE (65000)	//Pause
//Values for Octave 0
#define NOTE_C  (30577) // note C 
#define NOTE_Db (28862) // note C# / Db 
#define NOTE_D  (27242) // note D 
#define NOTE_Eb (25713) // note D# / Eb 
#define NOTE_E  (24270) // note E 
#define NOTE_F  (22908) // note F 
#define NOTE_Gb (21622) // note F# / Gb 
#define NOTE_G  (20408) // note G 
#define NOTE_Ab (19263) // note G# / Ab 
#define NOTE_A  (18182) // note A 
#define NOTE_Bb (17161) // note A# / Bb 
#define NOTE_B  (16198) // note B 

void music_init(void);
void music_setNote(uint16_t note, uint8_t octave);
#endif
