#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>

#include "../lib/apps.h"
#include "../lib/bughal.h"
#include "../lib/util.h"
#include "../lib/music.h"

/**
 *   just blink mode
 *   - left/right button switch motor off/on
 */
static void blink(void)
{
	static timer_t mytimer;
	static bool blink;
	if (mode_uninitialized) {
		init_leds();
		mode_uninitialized = false;
		music_setNote(NOTE_PAUSE, 4);	//mute
		set_motor(MOTOR_OFF);
		timer_set(&mytimer, 10);
		blink = false;
	}

	if (timer_expired(&mytimer)) {
		if (!blink) {
			/*lets blink*/
			led_on(LED_L | LED_R);
			timer_set(&mytimer, 1);
			blink = true;
		} else {
			/*stop blink*/
			led_off(LED_L | LED_R);
			timer_set(&mytimer, 123);
			blink = false;
		}

	}//end if timer_expired

	if (btn_state(BTNST_SUP, BTN_LEFT)) {
		button_clear(BTN_LEFT);
		set_motor(MOTOR_OFF);

	}
	if (btn_state(BTNST_SUP, BTN_RIGHT)) {
		button_clear(BTN_RIGHT);
		set_motor(MOTOR_ON);
	}

}

REGISTER(blink)

