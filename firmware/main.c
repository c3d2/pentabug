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



void __attribute__((noreturn)) 
main(void)
{
	/* hardware initialisation: */
	init_leds();
	init_buzzr();
	//USART0_Init();
	// init_motor();
	
	/* here the show begins:*/
	//sei();
	//set_motor(MOTOR_ON);

	for(;;) /* ever */  {
	//do something
		//synth_poll();
		//USART0_put_uint16(0xA09F);
		//USART0_crlf();
	}

	/* never  return 0; */
}



