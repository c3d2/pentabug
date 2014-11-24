#include <stdlib.h>

#include <pentabug/app.h>
#include <pentabug/hal.h>
#include <pentabug/pentatonic.h>
#include <pentabug/music.h>


// number of voices available
#define VOICEMAX	4

#define MAXTALK		50
#define MINNOTALK	6
#define MAXSILENCE	15



static uint8_t voice;
static uint8_t voice_this_time;
static uint8_t cycle_counter;
static uint8_t my_talk_time;
static uint8_t no_talk;
static enum {nothing, listening, talking} status;


static void init(void) {
	pentatonic_direction(ALL_OUT);

	voice = 1;
	cycle_counter = 0;
	my_talk_time = 2;		// don't start talking befor having a chance for listening
							// but don't wait to long, so something is happening
							// almost immediately
	no_talk = 0;
	status = nothing;
}


// prevent counter from overflow (or underflow)
static void inc_cycle_counter(void){
	if (cycle_counter < 0xFF) {
		cycle_counter += 1;
	}
}
static void dec_no_talk(void){
	if (no_talk > 0) {
		no_talk -= 1;
	}
}

static uint8_t really_no_ir(void) {			// be shure not to miss an ir event
	uint8_t i = 0;
	for (i = 0; i < 100; i++) {
		if (ir_recv()) {
			return 0;
		}
		wait_ms(1);
	}
	return 1;
}



static void waiting(void){
	ir_off();
	motor_off();
	stop_note();

	led_on(RIGHT);
	led_on(LEFT);
	wait_ms(50);
	led_off(LEFT);
	led_off(RIGHT);
	wait_ms(450);

	inc_cycle_counter();
	dec_no_talk();
}


static void listen(void){
	ir_off();
	motor_off();
	stop_note();

	led_on(RIGHT);
	led_on(LEFT);

	inc_cycle_counter();
	dec_no_talk();
	my_talk_time = 0;

	wait_ms(500);
}


static void talk(void){
		ir_on();

		uint8_t i = 0;
		uint8_t r = 0;
		static uint8_t note = 6;
		static int16_t tone = 250;
		int16_t new_tone;
		// TODO: test this. I have no pentatonic board :-(
		pentatonic_all_led_set((rand()& 255) >> 3);

		switch(voice_this_time) {
		case 1:
			led_set(RIGHT, (rand() & 15) > 3);
			led_set(LEFT, (rand() & 15) > 3);

			if((rand() & 15) > 7) {
				tone = ( (rand() & 2047) * 2) + 500;
				set_note(tone, 0);
			} else {
				stop_note();
			}
			wait_ms(200);
			break;


		case 2:
			for (i = 1; i < 4; i++) {
				led_set(RIGHT, (rand() & 15) > 3);
				led_set(LEFT, (rand() & 15) > 3);

				if((rand() & 15) > 7) {
					tone = ( (rand() & 1023) * 2) + 100;
					set_note(tone, 0);
				} else {
					stop_note();
				}
				wait_ms(50);
			}
			break;


		case 3:
			led_inv(RIGHT);
			led_set(LEFT, !led_state(RIGHT));
			r = rand() % 0xFF;
			if (r < 100) {
				note += 1;
			} else if  (r < 200) {
				note -= 1;
			} else if (r < 228) {
				note -= 2;
			} else {
				note += 2;
			}

			switch (note) {
			case 0:	note = 4; break;
			case 1: note = 3; break;
			case 12: note = 10; break;
			case 13: note = 9; break;
			}

			switch (note) {
			case 2:
				set_note(NOTE_E, 4);
				break;
			case 3:
				set_note(NOTE_G, 4);
				break;
			case 4:
				set_note(NOTE_A, 4);
				break;
			case 5:
				set_note(NOTE_C, 5);
				break;
			case 6:
				set_note(NOTE_D, 5);
				break;
			case 7:
				set_note(NOTE_E, 5);
				break;
			case 8:
				set_note(NOTE_G, 5);
				break;
			case 9:
				set_note(NOTE_A, 5);
				break;
			case 10:
				set_note(NOTE_C, 6);
				break;
			case 11:
				set_note(NOTE_D, 6);
				break;
			}
			wait_ms(200);
			if ( !(r % 4) ) {
				wait_ms(200);
			}

			// keep it shorter
			inc_cycle_counter();
			break;


		case 4:
			new_tone = ( (rand() & 1023) * 2) + 600;


			uint8_t steps = rand() % 50 + 10;

			for (i = 0; i < steps; i++) {
				if (! (i%3) ) {
					led_inv(RIGHT);
					led_set(LEFT, !led_state(RIGHT));
				}
				set_note(tone + i * (int32_t)(new_tone - tone) / steps, 1);
				wait_ms(10);
			}

			wait_ms(500);
			tone = new_tone;
			inc_cycle_counter();		// keep it shorter
			inc_cycle_counter();
			break;
		}

		// you don't HAVE TO talk the maximum time. Give a chance to stop earlier
		if ((rand() & 63) > 60) {
			cycle_counter = MAXTALK;
		}

		inc_cycle_counter();
		if (cycle_counter >= MAXTALK) {
			motor_off();
			stop_note();
			led_off(RIGHT);
			led_off(LEFT);

			wait_ms(200);	// no one shall start talking IMMEDIATELY after i finished
			ir_off();
			wait_ms(20);	// otherwise i will see my own ir in the next cycle


			status = nothing;
			cycle_counter = 0;
			my_talk_time = 0;
			no_talk = MINNOTALK;
		}
}





static void run(void) {

	if(status != talking) {
		if (really_no_ir()) {
			status = nothing;
		} else {
			// received an ir signal, now listen
			status = listening;
		}
	}

	switch(status) {
	case nothing:
		if (my_talk_time > 0 && cycle_counter >= my_talk_time) {
			status = talking;
			if (voice == 0) {
				voice_this_time = (rand() % VOICEMAX) + 1;
			} else {
				voice_this_time = voice;
			}
			break;
		}

		if (my_talk_time == 0 && no_talk == 0) {
			my_talk_time = rand() % MAXSILENCE;
			cycle_counter = 0;
		}

		waiting();
		break;

	case listening:
		listen();
		break;

	case talking:
		talk();
		break;
	}


	// change the voice with the buttons
	if(button_clicked(RIGHT)) {
		motor_off();
		stop_note();
		led_off(LEFT);
		led_off(RIGHT);

		set_note(NOTE_F, 5);
		wait_ms(200);
		stop_note();

		if (voice == VOICEMAX) {
			voice = 0;
		} else {
			voice += 1;
		}

		if (voice == 0) {
			wait_ms(200);
			set_note(NOTE_F, 5);
			wait_ms(400);
			stop_note();
			wait_ms(200);
			set_note(NOTE_F, 5);
			wait_ms(400);
			stop_note();
			wait_ms(200);
		} else {
			voice_this_time = voice;
		}
	}

	if(button_clicked(LEFT)) {
		motor_off();
		stop_note();
		led_off(LEFT);
		led_off(RIGHT);

		set_note(NOTE_F, 5);
		wait_ms(200);
		stop_note();

		if (voice == 0) {
			voice = VOICEMAX;
		} else {
			voice -= 1;
		}

		if (voice == 0) {
			wait_ms(200);
			set_note(NOTE_F, 5);
			wait_ms(400);
			stop_note();
			wait_ms(200);
			set_note(NOTE_F, 5);
			wait_ms(400);
			stop_note();
			wait_ms(200);
		} else {
			voice_this_time = voice;
		}
	}

}

REGISTER(run, init, NULL);
