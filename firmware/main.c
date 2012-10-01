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
			if (0==ledmode){
				ledmode =1;
				led_on(LED_L);
	
			} else {
			
				ledmode=0;
				led_off(LED_L);

			};
			timer_set(&t, 1);
			USART0_crlf();
			USART0_put_uint16(ct);
			ct++;
		}; //end if timer expired

		//USART0_put_uint16(0xA09F);
		//USART0_crlf();


	}

	/* never  return 0; */
}



