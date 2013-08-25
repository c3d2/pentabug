#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>

#include "../lib/apps.h"
#include "../lib/bughal.h"
#include "../lib/util.h"
#include "../lib/music.h"
#include "../lib/usart.h"

inline uint16_t biased_random(uint16_t value) {
	return value / 8 * (rand() & 7);
}

static void led_sound(void)
{
	static bool discharge;
	static timer_t mytimer;
	uint16_t led1;

	if(mode_last_tick) {
		led_off(LED_L | LED_R);
		set_motor(MOTOR_OFF);
		music_setNote(NOTE_PAUSE, 0);
		return;
	}

	if (mode_uninitialized) {
		mode_uninitialized = false;
		set_motor(MOTOR_OFF);
		ADMUX   = (1 << REFS0);                // use VCC reference
		ADCSRA  = (1 << ADPS1) | (1 << ADPS0); // prescaler F_CPU/8
		ADCSRA |= (1 << ADEN);                 // ADC enable - turn it on

		timer_set(&mytimer, 10);
		discharge = true;
	}
	if (timer_expired(&mytimer)) {
		if (discharge) {
			led_off(LED_R | LED_L);

			DDRC |= (1 << PORTC0) | (1 << PORTC1);	//set LED Ports  to output:
			/*discharge*/
			PORTC = (PORTC & 0b11110000);
			/*set C0 and C2 to input (disable pullups)*/
			DDRC &= ~(1 << PORTC0);	//set Led Ports to input
			/*pull ups off*/
			PORTC &= ~(1 << PORTC0);
			discharge = false;

			timer_set(&mytimer, 1);
		} else {
			/*single measurement*/
			ADMUX = (ADMUX & ~(0x1F)) | 0; // select channel 0
			ADCSRA |= (1 << ADSC);         // start single conversion
			while (ADCSRA & (1 << ADSC)) ; // wait for conversion to end
			led1 = ADCW;                   // read result

			DDRC |= (1 << PORTC0) | (1 << PORTC1);	//set LED Ports  to output:

			set_motor(biased_random(led1) > 0x50 ? MOTOR_ON : MOTOR_OFF);

			if(biased_random(led1) > 0x20) {
				led_on(LED_R);
			} else {
				led_off(LED_R);
			}

			if(biased_random(led1) > 0x20) {
				led_on(LED_L);
			} else {
				led_off(LED_L);
			}

			if(biased_random(led1) > 0x90) {
				uint16_t tone = (biased_random(led1) * 2) + 500;
				music_setNote(tone, 0);
			} else {
				music_setNote(NOTE_PAUSE, 0);
			}

			discharge = true;

			timer_set(&mytimer, 20);
		}
	}			//end if timer_expired
}				//end mode5

REGISTER(led_sound)
