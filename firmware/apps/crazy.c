#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>

#include "../lib/apps.h"
#include "../lib/bughal.h"
#include "../lib/util.h"
#include "../lib/music.h"

/**
 * crazymoves mode
 * - play random sounds and move in random fashion
 */
static void crazy(void)
{
	static timer_t mytimer;
	uint8_t max      = 50;
	uint8_t min      = 5;
	uint16_t maxfreq = 5000;
	uint16_t minfreq = 1000;

	if (mode_uninitialized) {
		mode_uninitialized = false;
		music_setNote(NOTE_PAUSE, 4);	//mute
		timer_set(&mytimer, 10);
		init_leds();
		led_off(LED_L | LED_R);
		set_motor(MOTOR_OFF);
	}

	if (timer_expired(&mytimer)) {
		set_motor(MOTOR_OFF);
		music_setNote(NOTE_PAUSE, 0);	//mute
		/*set random led*/
		switch (rand() % 4) {
		case 0: led_on(LED_L); break;
		case 1: led_on(LED_R); break;
		case 2: led_on(LED_L | LED_R); break;
		default: led_off(LED_L | LED_R); break;
		};
		/*decide if to switch motor on (40% chance)*/
		if (rand() % 5 > 2)
			set_motor(MOTOR_ON);

		/*decide if to play sound (70% chance)*/
		if (rand() % 10 > 2) {
			music_setNote((rand() % (maxfreq - minfreq)) + minfreq,
				      0);

		}

		timer_set(&mytimer, (rand() % (max - min)) + min);
	}//end if timer_expired

	/*deinialisation */
	if (mode_last_tick) {
		set_motor(MOTOR_OFF);
		music_setNote(NOTE_PAUSE, 0);	//mute
	}
}

REGISTER(crazy)

