#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

#define ever (;;) /* awesomnes++ */

#include "lib/usart.h"
#include "lib/bughal.h"
#include "lib/util.h"
#include "lib/music.h"
#include "lib/synth.h"

//operartion modes
#define MODE0 0
#define MODE1 1
#define MODE2 2
#define MODE3 3
#define MODE4 4
#define MODE5 5		
#define NUM_MODES 6

uint8_t OpMode   = MODE1;	
uint8_t NextMode = MODE1;

bool mode_uninitialized = true;

// check if mode should be changed (one of the buttons long pressed)
void modeswitch_poll(void)
{
	if (btn_state(BTNST_LUP, BTN_LEFT)) {
		//opmode -
		NextMode = (0 == OpMode) ? (NUM_MODES - 1) : (OpMode - 1);
		//mode_uninitialized = true;
		button_clear(BTN_LEFT);
	};
	if (btn_state(BTNST_LUP, BTN_RIGHT)) {
		//opmode +
		//mode_uninitialized = true;
		NextMode = ((NUM_MODES - 1) == OpMode) ? 0 : (OpMode + 1);
		button_clear(BTN_RIGHT);
	};
	return;
}

/**
 *  sound detection mode
 *  beeps,blinks and moves when sound is detected
 * - beep on/off left switch (short)
 * - motor on/off right switch (short)
 */
void do_mode0(void)
{
	static timer_t mytimer;
	static uint16_t maxval;		//maximum of ADC values read during the las timer interval
	static uint16_t lastmaxval;	//maximum of values during last timer interval
	uint16_t curval;		//current value on D5 (one pin of the piezo,the other is on GND)
	static bool signaling;		//are we currently signaling (beeping, blinking etc...)
	static bool sound_on;		//should sound be on when signaling
	static bool motor_on;		//should motor be on when signaling

	if (mode_uninitialized) {	//init after mode change
		maxval = 0;
		lastmaxval = 000;
		mode_uninitialized = false;
		signaling = false;
		sound_on = true;
		motor_on = true;
		init_mic();
		init_leds();
		timer_set(&mytimer, 10);
	}

	// single ADC measurement
	curval = ADCW;		// read result
	maxval = (curval > maxval) ? curval : maxval;

	//check for Buttons
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
			maxval     = 0;
		}//end if soundon

	}//end if timer_expired

} /* end mode0 */

/**
 * do crazy synthesizer mode
 *
 */
void do_mode1(void)
{
	if (mode_uninitialized) {
		mode_uninitialized = false;
		synth_init();
	}

	/*deinialisation required*/
	if(OpMode != NextMode){
		synth_deinit();
	}

	return;
}


/**
 * crazymoves mode
 * - play random sounds and move in random fashion
 */
void do_mode2(void)
{
	static timer_t mytimer;
	uint8_t max = 50;
	uint8_t min = 5;
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
		// set random led
		switch (rand() % 4) {
		case 0:
			led_on(LED_L);
			break;
		case 1:
			led_on(LED_R);
			break;
		case 2:
			led_on(LED_L | LED_R);
			break;
		default:
			led_off(LED_L | LED_R);
			break;
		};
		// decide if to switch motor on (40% chance)
		if (rand() % 5 > 2)
			set_motor(MOTOR_ON);

		//decide if to play sound (70% chance)
		if (rand() % 10 > 2) {
			music_setNote((rand() % (maxfreq - minfreq)) + minfreq,
				      0);

		}

		timer_set(&mytimer, (rand() % (max - min)) + min);
	}//end if timer_expired

	/*deinialisation required*/
	if(OpMode != NextMode){
		set_motor(MOTOR_OFF);
		music_setNote(NOTE_PAUSE, 0);	//mute
	}
}


/**
 *   just blink mode
 *   - left/right button switch motor off/on
 */
void do_mode3(void)
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
			//lets blink
			led_on(LED_L | LED_R);
			timer_set(&mytimer, 1);
			blink = true;
		} else {
			//stop blink
			led_off(LED_L | LED_R);
			timer_set(&mytimer, 123);
			blink = false;
		}

	}			//end if timer_expired

	if (btn_state(BTNST_SUP, BTN_LEFT)) {
		button_clear(BTN_LEFT);
		set_motor(MOTOR_OFF);

	}
	if (btn_state(BTNST_SUP, BTN_RIGHT)) {
		button_clear(BTN_RIGHT);
		set_motor(MOTOR_ON);
	}

}



/**
 * ggrbug mode
 * - simulate geiger counter sounds
 */
void do_mode4(void)
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
			//lets blink
			int i = (rand() % 3);
			switch (i) {
			case 0:
				led_on(LED_L);
				break;
			case 1:
				led_on(LED_R);
				break;
			default:
				led_on(LED_L | LED_R);
				break;
			};
			if (rand() % 10 > 8)
				set_motor(MOTOR_ON);
			music_setNote(NOTE_C, 5);
			timer_set(&mytimer, 2);
			blink = true;
		} else {
			//stop blink
			led_off(LED_L | LED_R);
			set_motor(MOTOR_OFF);
			music_setNote(NOTE_PAUSE, 0);
			timer_set(&mytimer, (rand() % (max - min)) + min);
			blink = false;
		}

	}			//end if timer_expired

}

void do_mode5(void)
{
	static bool discharge;
	static timer_t mytimer;
	uint16_t led1;
	uint16_t led2;

	if (mode_uninitialized) {
		mode_uninitialized = false;
		set_motor(MOTOR_OFF);
		ADMUX = (1 << REFS0);	//use VCC reference
		ADCSRA = (1 << ADPS1) | (1 << ADPS0);	// prescaler F_CPU/8
		ADCSRA |= (1 << ADEN);	// ADC enable - turn it on
		// do one conversion
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC)) {
		}		//wait for conversion to end
		uint16_t __attribute__ ((unused)) dummy = ADCW;	//read once
		timer_set(&mytimer, 10);
		discharge = true;
	}
	if (timer_expired(&mytimer)) {
		if (discharge) {
			DDRC |= (1 << PORTC0) | (1 << PORTC1) | //set LED Ports  to output:
			        (1 << PORTC2) | (1 << PORTC3)  ;
			// discharge
			PORTC = (PORTC & 0b11110000);
			//set C0 and C2 to input (disable pullups)
			DDRC &= ~((1 << PORTC0) | (1 << PORTC2)); //set Led Ports to input
			//pull ups off
			PORTC &= ~((1 << PORTC0) | (1 << PORTC2));
			discharge = false;
		} else {
			// single measurement
			ADMUX   = (ADMUX & ~(0x1F)) | 0;	// select channel 0
			ADCSRA |= (1 << ADSC);			// start single conversion
			while     (ADCSRA & (1 << ADSC)) ;	// wait for conversion to end
			led1    = ADCW;				// read result

			ADMUX   = (ADMUX & ~(0x1F)) | 2;	// select channel 2
			ADCSRA |= (1 << ADSC);			// start single conversion
			while     (ADCSRA & (1 << ADSC)) ;	// wait for conversion to end
			led2    = ADCW;				// read result
#if 0
			USART0_putc('1');USART0_putc(':');USART0_put_uint16(led1);USART0_crlf();
			USART0_putc('2');USART0_putc(':');USART0_put_uint16(led2);USART0_crlf();
#endif
			music_setNote(400 +((0x1ff - led1) + (0x1ff - led2)) * 5, 0);
			discharge = true;
		}
		timer_set(&mytimer, 2); //relaunch timer
	}//end if timer_expired
}//end mode5


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
		modeswitch_poll();
		switch (OpMode) {
		case MODE1: do_mode1() ; break ;
		case MODE2: do_mode2() ; break ;
		case MODE3: do_mode3() ; break ;
		case MODE4: do_mode4() ; break ;
		case MODE5: do_mode5() ; break ;
		default: do_mode0()    ; break ;
		}
		if (OpMode!=NextMode) mode_uninitialized = true;
		OpMode = NextMode;
	}
}


