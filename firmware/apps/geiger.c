#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>

#include "../lib/apps.h"
#include "../lib/bughal.h"
#include "../lib/util.h"
#include "../lib/music.h"
#include "../lib/synth.h"
#include "../lib/usart.h"

/**
 * ggrbug mode
 * - simulate geiger counter sounds
 */
static void geiger(void)
{
	uint8_t max = 200;
	uint8_t min = 10;

	static timer_t mytimer;
	static bool blink;
	if (mode_uninitialized) {
		init_leds();
		music_setNote(NOTE_PAUSE, 0);
		mode_uninitialized = false;
		timer_set(&mytimer, 10);
		blink = false;
	};
	if (timer_expired(&mytimer)) {
		if (!blink) {
			/*lets blink*/
			int i = (rand() % 3);
			switch (i) {
				case 0: led_on(LED_L); break;
				case 1: led_on(LED_R); break;
				default: led_on(LED_L | LED_R); break;
			};
			if (rand() % 10 > 8)
				set_motor(MOTOR_ON);
			music_setNote(NOTE_C, 5);
			timer_set(&mytimer, 2);
			blink = true;
		} else {
			/*stop blink*/
			led_off(LED_L | LED_R);
			set_motor(MOTOR_OFF);
			music_setNote(NOTE_PAUSE, 0);
			timer_set(&mytimer, (rand() % (max - min)) + min);
			blink = false;
		}

	}//end if timer_expired

}

REGISTER(geiger)
