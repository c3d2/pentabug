#include <inttypes.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

#define ever (;;)		/* awesomnes++ */

#include "lib/usart.h"
#include "lib/bughal.h"
#include "lib/util.h"
#include "lib/music.h"

#include "lib/apps.h"

mode_fun mode_funs[MAX_FUNS] = { NULL };
uint8_t mode_num = 0;

/*specialmodes, not in normal mode loop*/
#define MODE_PWDN mode_num		//go to sleep

uint8_t OpMode   = 0;
uint8_t NextMode = 0;

uint8_t mode_uninitialized = true;
uint8_t mode_last_tick = false;

/*check if mode should be changed (one of the buttons long pressed)*/
void modeswitch_poll(void)
{
	if (btn_state(BTNST_LUP, BTN_LEFT)) {
		//opmode --
		NextMode = (OpMode -1) % mode_num;
		button_clear(BTN_LEFT);
	}
	if (btn_state(BTNST_LUP, BTN_RIGHT)) {
		//opmode ++
		NextMode = (OpMode +1) % mode_num;
		button_clear(BTN_RIGHT);
	}
	if ( btn_state(BTNST_LDN, BTN_RIGHT) &&
	     btn_state(BTNST_LDN, BTN_LEFT)   ) {
		NextMode = MODE_PWDN;
	}
	return;
}

void do_powerDown(void)
{
	static timer_t mytimer;
	static uint8_t pwdn_state;
	static bool ledRon;
	uint8_t oldreg;
	if (mode_uninitialized) {
		mode_uninitialized = false;
		pwdn_state = 0;
		timer_set(&mytimer, 5);
		ledRon = true;
	};
	if (timer_expired(&mytimer)) {
		switch (pwdn_state) {
		case 0:
			if (ledRon) {
				led_on(LED_L);
				led_off(LED_R);
			} else {
				led_off(LED_L);
				led_on(LED_R);
			};
			ledRon = !ledRon;
			timer_set(&mytimer, 6);
			if ((btn_state(BTNST_SUP, BTN_RIGHT)
			     || btn_state(BTNST_LUP, BTN_RIGHT))
			    && (btn_state(BTNST_SUP, BTN_LEFT)
				|| btn_state(BTNST_LUP, BTN_LEFT))) {
				//both buttons released
				led_off(LED_L | LED_R);
				pwdn_state = 1;
				timer_set(&mytimer, 10);
			}
			break;
		case 1:
			music_setNote(NOTE_A, 4);
			timer_set(&mytimer, 10);
			pwdn_state++;
			break;
		case 2:
			music_setNote(NOTE_F, 4);
			timer_set(&mytimer, 5);
			pwdn_state++;
			break;
		case 3:
			music_setNote(NOTE_D, 3);
			timer_set(&mytimer, 15);
			pwdn_state++;
			break;
		case 4:
			music_setNote(NOTE_PAUSE, 4);
			timer_set(&mytimer, 1);
			pwdn_state++;
			break;
		case 5:	//now we can really power down
			// lets switch everything off
			oldreg = PCMSK2;
			PCMSK2 |= 3; //PCINT16 PCINT17
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_enable();
			sei();
			sleep_cpu();
			sleep_disable();
			PCMSK2 = oldreg;
			NextMode = 0;
			break;
		default:
			break;

		}		//end switch

	}			//end timer expired
}				// end do_powerDown


ISR(PCINT2_vect){ ; }


void __attribute__ ((noreturn)) main(void)
{
	/* hardware initialisation: */
	init_leds();
	init_buzzr();
	init_switch();
	USART0_Init();
	init_motor();
	/* software initialisation */
	timer_init();
	music_init();

	/* here the show begins: */
	sei();
	for ever {
		//main polling loop;
		button_poll();

		if(OpMode < mode_num) {
			modeswitch_poll();
		}

		if (OpMode != NextMode){
			mode_last_tick = true;
		}

		if(OpMode < mode_num) {
			mode_funs[OpMode]();
		} else {
			do_powerDown();
		}

		if (OpMode != NextMode){
			mode_last_tick = false;
			mode_uninitialized = true;
			OpMode = NextMode;
		}
	}
}
