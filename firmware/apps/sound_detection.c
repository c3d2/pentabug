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
 *  sound detection mode
 *  beeps,blinks and moves when sound is detected
 * - beep on/off left switch (short)
 * - motor on/off right switch (short)
 */
static void sound_detection(void)
{
	static timer_t mytimer;
	static uint16_t maxval;     // maximum of ADC values read during the las timer interval
	static uint16_t lastmaxval; // maximum of values during last timer interval
	uint16_t curval;            // value on D5 (pin of piezo,the other is on GND)
	static bool signaling;      // are we currently signaling (beeping, blinking etc...)
	static bool sound_on;       // should sound be on when signaling
	static bool motor_on;       // should motor be on when signaling

	if (mode_uninitialized) {	//init after mode change
		maxval             = 0;
		lastmaxval         = 0;
		mode_uninitialized = false;
		signaling          = false;
		sound_on           = true;
		motor_on           = true;
		init_mic();
		init_leds();
		timer_set(&mytimer, 10);
	}
	/*single ADC measurement*/
	curval = ADCW;		// read result
	maxval = (curval > maxval) ? curval : maxval;

	/*check for Buttons*/
	if (btn_state(BTNST_SUP, BTN_LEFT)) {
		button_clear(BTN_LEFT);
		sound_on = !sound_on;
	}
	if (btn_state(BTNST_SUP, BTN_RIGHT)) {
		button_clear(BTN_RIGHT);
		motor_on = !motor_on;
	}
	if (timer_expired(&mytimer)) {
		if (signaling) {
			//turn off sound
			music_setNote(NOTE_PAUSE, 0);	//mute
			set_motor(MOTOR_OFF);
			//re-init mic
			init_mic();
			led_off(LED_R | LED_L);
			timer_set(&mytimer, 1);
			signaling = false;
			lastmaxval = 10000;
		} else {	//sound was off wer're in measuring mode
			if (maxval > lastmaxval) {
				USART0_put_uint16(maxval);
				USART0_crlf();
				led_on(LED_R | LED_L);
				init_buzzr();	//buzzr to output
				if (sound_on)
					music_setNote(NOTE_C, 5);
				if (motor_on)
					set_motor(MOTOR_ON);
				signaling = true;
				timer_set(&mytimer, 5);	//sound duration
			} else {
				timer_set(&mytimer, 1);
			}
			lastmaxval = maxval;
			maxval = 0;
		}//end if soundon

	}//end if timer_expired

}				/* end mode0 */

REGISTER(sound_detection)
