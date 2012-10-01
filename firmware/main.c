#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"

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
	timer_t t;
	uint8_t ledstate_l =0;
	uint8_t ledstate_r =0;
	uint8_t motorstate =0;
	timer_set(&t, 100);
	for(;;) /* ever */  {
	//do something
	//main polling loop;
		button_poll();
		if (timer_expired(&t)){
			//while left button is pressed switch left led on
			if (btn_state(BTNST_SDN,BTN_LEFT)|btn_state(BTNST_LDN,BTN_LEFT)){
			   led_on(LED_L);
                        } else {
                           led_off(LED_L);
                        };
			//when right button has been pressed short, toggle right led
			if (btn_state(BTNST_SUP,BTN_RIGHT)){
			   if (ledstate_r ==0){
				ledstate_r = 1;
				led_on(LED_R);
			   } else {
				ledstate_r = 0;
				led_off(LED_R);
			   };
		           button_clear(BTN_RIGHT); //this is important, to show the event has been processed
			};	
			//when right button has been pressed long, toggle motor
			if (btn_state(BTNST_LUP,BTN_RIGHT)){
			   if (motorstate ==0){
				motorstate = 1;
				set_motor(MOTOR_ON);
			   } else {
				motorstate = 0;
				set_motor(MOTOR_OFF);
			   };
		           button_clear(BTN_RIGHT); //this is important, to show the event has been processed
			};				
			
			timer_set(&t, 5);
		}; //end if timer expired
		//USART0_put_uint16(0xA09F);
		//USART0_crlf();


	}

	/* never  return 0; */
}



