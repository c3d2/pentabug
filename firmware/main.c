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
#include "lib/music.h"
#include "lib/synth.h"

//operartion modes
#define MODE0 0
#define MODE1 1
#define MODE2 2
#define MODE3 3
#define NUM_MODES 4



uint8_t OpMode = MODE0; //Operation mode
bool 	ModeChanged = true;

void modeswitch_poll(void){
   if (btn_state(BTNST_LUP,BTN_LEFT)){
     //opmode -
     OpMode = (0 == OpMode) ? (NUM_MODES-1) : (OpMode-1);
     ModeChanged = true;
     button_clear(BTN_LEFT);
   };
   if (btn_state(BTNST_LUP,BTN_RIGHT)){
     //opmode +
     ModeChanged = true;
     OpMode = ((NUM_MODES-1) == OpMode) ? 0 : (OpMode+1);
     button_clear(BTN_RIGHT);
   };
   return;
};


void do_mode0(void){
 uint8_t max = 200;
 uint8_t min = 10;

 static timer_t mytimer;
 static bool blink;
   if (ModeChanged){
    music_setNote(NOTE_PAUSE,0);
    ModeChanged = false;
    timer_set(&mytimer, 10);
    blink = false;
   };
   if(timer_expired(&mytimer)){
     if (!blink) {
       //lets blink
	int i = (rand() % 3);
	switch(i) {
		case 0  : led_on(LED_L); break;
		case 1  : led_on(LED_R); break;
		default : led_on(LED_L|LED_R);
	};
	if (rand()%10>8) set_motor(MOTOR_ON);
       music_setNote(NOTE_C,5);
       timer_set(&mytimer, 2);
       blink=true;
     } else {
       //stop blink
       led_off(LED_L|LED_R);
	set_motor(MOTOR_OFF);
       music_setNote(NOTE_PAUSE,0);
       timer_set(&mytimer, (rand() % (max-min)) + min);

	blink=false;
     }

   } //end if timer_expired




};


void do_mode1(void){
  static uint16_t tone;
  if (ModeChanged){
    ModeChanged = false;
    tone = 1000;
    music_setNote(tone,0);
    led_off(LED_L|LED_R);  
  };
  if (btn_state(BTNST_SUP,BTN_LEFT))  {
    button_clear(BTN_LEFT);
    tone += 10;
    music_setNote(tone,0);
  };
  if (btn_state(BTNST_SUP,BTN_RIGHT))  {
    button_clear(BTN_RIGHT);
    tone -= 10;
    music_setNote(tone,0);
  };
};

void do_mode2(void){
 static timer_t mytimer;
 uint8_t max = 50;
 uint8_t min = 5;
 uint8_t maxfreq = 6000;
 uint8_t minfreq = 2000;

  if (ModeChanged){
    ModeChanged = false;
    music_setNote(NOTE_PAUSE,4); //mute
    timer_set(&mytimer, 10);
    led_off(LED_L|LED_R);
    set_motor(MOTOR_OFF);
  }

   if(timer_expired(&mytimer)){
      set_motor(MOTOR_OFF);
      music_setNote(NOTE_PAUSE,0); //mute
     // set random led
	switch(rand() % 4) {
		case 0  : led_on(LED_L); break;
		case 1  : led_on(LED_R); break;
		case 2  : led_on(LED_L|LED_R); break;
		default : led_off(LED_L|LED_R);
	};
     // decide if to switch motor on (40% chance)
	if (rand()%5>2) set_motor(MOTOR_ON);

     //decide if to play sound (70% chance)
	if (rand()%10>2) {
         music_setNote((rand() % (maxfreq-minfreq)) + minfreq,0);
      
   }

	timer_set(&mytimer, (rand() % (max-min)) + min);
   }; //end if timer_expired

    


};

void do_mode3(void){
 static timer_t mytimer;
 static bool blink;
  if (ModeChanged){
    ModeChanged = false;
    music_setNote(NOTE_PAUSE,4); //mute
    set_motor(MOTOR_OFF);
    timer_set(&mytimer, 10);
    blink = false;
  };
  
   if(timer_expired(&mytimer)){
     if (!blink) {
       //lets blink
	led_on(LED_L|LED_R);
       timer_set(&mytimer, 1);
       blink=true;
     } else {
       //stop blink
       led_off(LED_L|LED_R);
       timer_set(&mytimer, 123);
	blink=false;
     }


   } //end if timer_expired

  if (btn_state(BTNST_SUP,BTN_LEFT))  {
    button_clear(BTN_LEFT);
    set_motor(MOTOR_OFF);

  };
  if (btn_state(BTNST_SUP,BTN_RIGHT))  {
    button_clear(BTN_RIGHT);
    set_motor(MOTOR_ON);  };



};


int main(void)
{
	/* hardware initialisation: */
	init_leds();
	init_buzzr();
	init_switch();
	USART0_Init();
	init_motor();

	synth_init(); /* synth debug */
	for(;;);

	/* software initialisation */
	timer_init();
	music_init();
	
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
	  modeswitch_poll();
	  switch(OpMode){
  	     case MODE1 : do_mode1(); break; 
	     case MODE2 : do_mode2(); break;
	     case MODE3 : do_mode3(); break;
	     default : do_mode0();
	  }
/*

		if (timer_expired(&t)){
			//while left button is pressed switch left led on
			if (btn_state(BTNST_SDN,BTN_LEFT)|btn_state(BTNST_LDN,BTN_LEFT)){
			   led_on(LED_L);
                        } else {
                           led_off(LED_L);
                        };
			if (btn_state(BTNST_SUP,BTN_LEFT)){
			   music_setNote(NOTE_PAUSE,0); //mute
			   button_clear(BTN_LEFT);
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
			   music_setNote(500,0); //50Hz sound
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
			music_setNote(NOTE_B,5); //1000Hz sound
		           button_clear(BTN_RIGHT); //this is important, to show the event has been processed
			};				
			
			timer_set(&t, 5);
		}; //end if timer expired
		//USART0_put_uint16(0xA09F);
		//USART0_crlf();
*/

	}

	return 0;
}



