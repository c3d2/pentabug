#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>

#include "../lib/apps.h"
#include "../lib/bughal.h"
#include "../lib/util.h"
#include "../lib/music.h"
#include "../lib/synth.h"
#include "../lib/usart.h"

static void led_sound(void)
{
	static bool discharge;
	static timer_t mytimer;
	uint16_t led1;
	uint16_t led2;

	if (mode_uninitialized) {
		mode_uninitialized = false;
		set_motor(MOTOR_OFF);
		ADMUX   = (1 << REFS0);                // use VCC reference
		ADCSRA  = (1 << ADPS1) | (1 << ADPS0); // prescaler F_CPU/8
		ADCSRA |= (1 << ADEN);                 // ADC enable - turn it on
		/*do one conversion*/
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC)) {
		/*wait for conversion to end */
		}
		uint16_t __attribute__ ((unused)) dummy = ADCW;	//read once
		timer_set(&mytimer, 10);
		discharge = true;
	}
	if (timer_expired(&mytimer)) {
		if (discharge) {
			DDRC |= (1 << PORTC0) | (1 << PORTC1) |	//set LED Ports  to output:
			    (1 << PORTC2) | (1 << PORTC3);
			/*discharge*/
			PORTC = (PORTC & 0b11110000);
			/*set C0 and C2 to input (disable pullups)*/
			DDRC &= ~((1 << PORTC0) | (1 << PORTC2));	//set Led Ports to input
			/*pull ups off*/
			PORTC &= ~((1 << PORTC0) | (1 << PORTC2));
			discharge = false;
		} else {
			/*single measurement*/
			ADMUX = (ADMUX & ~(0x1F)) | 0; // select channel 0
			ADCSRA |= (1 << ADSC);         // start single conversion
			while (ADCSRA & (1 << ADSC)) ; // wait for conversion to end
			led1 = ADCW;                   // read result

			ADMUX = (ADMUX & ~(0x1F)) | 2; // select channel 2
			ADCSRA |= (1 << ADSC);         // start single conversion
			while (ADCSRA & (1 << ADSC)) ; // wait for conversion to end
			led2 = ADCW;                   // read result
#if 0
			USART0_putc('1');
			USART0_putc(':');
			USART0_put_uint16(led1);
			USART0_crlf();
			USART0_putc('2');
			USART0_putc(':');
			USART0_put_uint16(led2);
			USART0_crlf();
#endif
			music_setNote(400 +
				      ((0x1ff - led1) + (0x1ff - led2)) * 5, 0);
			discharge = true;
		}
		timer_set(&mytimer, 2);	//relaunch timer
	}			//end if timer_expired
}				//end mode5

REGISTER(led_sound)
