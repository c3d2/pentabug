#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"
#include "lib/synth.h"
#include "lib/usart.h"
#include "lib/bughal.h"
#include "lib/util.h"



void __attribute__((noreturn)) 
main(void)
{
	/* hardware initialisation: */
	init_leds();
	init_buzzr();
	init_switch();
	USART0_Init();
	init_motor();
	/* software initialisation */
	timer_init();
	
	/* here the show begins:*/
	sei();
	//set_motor(MOTOR_ON);
	timer_t t;
	uint8_t ledmode =0;
	uint16_t ct =0;
	led_on(LED_R);
	timer_set(&t, 100);
	for(;;) /* ever */  {
	//do something
	//main polling loop;
		if (timer_expired(&t)){
			if (switch_l()) {led_on(LED_L);} else {led_off(LED_L);};
			if (switch_r()) {led_on(LED_R);} else {led_off(LED_R);};

			timer_set(&t, 50);

		}; //end if timer expired

		//USART0_put_uint16(0xA09F);
		//USART0_crlf();


	}

	/* never  return 0; */
}



