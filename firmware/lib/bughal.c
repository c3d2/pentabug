#include <inttypes.h>
#include <avr/io.h>
#include "bughal.h"
#include "util.h" //for timer

/* Hardware abstraction layer for Pentabug hardware */

/*
 * initialize LEDs on C0-C3
 */ 


static uint8_t oldinput; // button readings from last poll cycle
static uint8_t curinput; // button readings from current poll cycle
//each switch has its own state machine
static uint8_t btnstates[BTN_BUTTONS]; //array for current button states
static uint8_t btncounters[BTN_BUTTONS]; //individual counter for button state machine
static timer_t btntimers[BTN_BUTTONS];  //individiual timer for for button state machine



void init_leds(void){
	//enable LED channels as output
	DDRC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);
	// both LEDs off
	PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3));


	 //TCCR2A = (1 << WGM21);
	  //TCCR2B = (1 << CS20)|(1 << CS21);
	  //OCR2A = 255; /* TOP */
	  //	TCNT2 = 0;
	  //	/*enable interrupt*/
	  //	TIMSK2 |=  (1<<OCIE2A);


  	return;
}

inline void led_on(int leds){
 	PORTC |= leds;
}

inline void led_off(int leds){
	PORTC &= ~leds;
}



void init_buzzr(void){
	//its on B2 and C5, for reasons
	DDRC |= (1 << PORTC5);
	DDRB |= (1 << PORTB2);
	//switch it off
	buzzr_off();
	return;
}


void init_mic(void){
	//its on B2 and C5, for reasons
	// 
	DDRC |= (1 << PORTC5);
	DDRB |= (1 << PORTB2);
	//switch it off
	buzzr_off();
	return;
}



void buzzr_up(void){
	PORTC &= ~(1 << PORTC5);
	PORTB |= (1 << PORTB2);
	return;
}

void buzzr_down(void){
	PORTC |= (1 << PORTC5);
	PORTB &= ~(1 << PORTB2);
}

inline void buzzr_off(void){
	PORTC &= ~(1 << PORTC5);
	PORTB &= ~(1 << PORTB2);
}
void buzzr_inv(void){
	PORTC ^= (1 << PORTC5);
	PORTB ^= (1 << PORTB2);
}

void init_switch(void){
	DDRD &= ~( (1 << PORTD1) | (1<<PORTD0));
	PORTD |= (1 << PORTD1) | (1<<PORTD0); //Pullups FTW
	// set predefined buttonstates
	oldinput = 0;
	curinput = 0;
	for(uint8_t i=0; i<BTN_BUTTONS; i++){
		btnstates[i] = BTNST_NTRL; //init button states
		btncounters[i] = 0;        //init button counters
		timer_set(&btntimers[i], 0x05); //50ms - init button timers
	}

	return;
}

bool switch_l(void){
	return !(PIND & 0b00000001);
};

bool switch_r(void){
	return !(PIND & 0b00000010);
};


void init_motor(void)
{
	/* vibration motor on B1, initially off: */
	DDRB  |= (1 << PORTB1);
	PORTB &= ~( 1<<PORTB1);
	return;
}

void set_motor(int val){
	PORTB = ~(val);
	return;
}

// reset button to neutral state
void button_clear(uint8_t button){
	btnstates[button] = BTNST_NTRL;
}


void stateswitch(uint8_t i ){
	switch(btnstates[i])
	{
		case BTNST_NTRL: //NEUTRAL
			if (curinput & (1<<i)){ //button down
				btncounters[i] = 0;
				btnstates[i] = BTNST_DBNC;
			}
			break;
		//intermediate state, check if button is still pressed to debounce
		case BTNST_DBNC: 
			btnstates[i] = (curinput & (1<<i))? BTNST_SDN: BTNST_NTRL;
			(btncounters[i])++;
			break;

		case BTNST_SDN: //is shortpressed and still held down
			if (curinput & (1<<i)){
				btncounters[i]++;
				if (btncounters[i] > BTN_T_LONGFACT){ //500ms held
					btnstates[i] = BTNST_LDN;
				}
			} else { //button was released
				btnstates[i] = BTNST_SUP;
				//signal shortclick
			}
			break;
		case BTNST_LDN: //is longpressed and still held down
			if (!(curinput & (1<<i))){
				//button was released
				btnstates[i] = BTNST_LUP; //signal longpress
			}
			break;
		case BTNST_SUP: //Button came up after being pressed shortly
			if ((curinput & (1<<i))){
				//button was pressed again or is bouncing after release
				btnstates[i] = BTNST_SUPDBNC; //going in special debounce
				btncounters[i] = 0;
			}
			break;
		case BTNST_LUP: //Button came up after being pressed for a long time
			if ((curinput & (1<<i))){
				//button was pressed again or is bouncing after release
				btnstates[i] = BTNST_LUPDBNC; //going in special debounce
				btncounters[i] = 0;
			}
			break;
		case BTNST_SUPDBNC: //Button was pressed again after beeing short pressed(or is bouncing)
			if ((curinput & (1<<i))){
				//button is still pressed --> going to shortpress
				btncounters[i]++;
				btnstates[i] = BTNST_SDN; //starting over from short pressed
			} else {
				btnstates[i] = BTNST_SUP; //nope, it was bouncing, back to old state
			}
			break;
		case BTNST_LUPDBNC: //Button was pressed again after beeing short pressed(or is bouncing)
			if ((curinput & (1<<i))){
				//button is still pressed --> going to shortpress
				btncounters[i]++;
				btnstates[i] = BTNST_SDN; //starting over from short pressed
			} else {
				btnstates[i] = BTNST_LUP; //nope, it was bouncing, back to old state
			}
			break;
		default: //curently catches nothing
			// do nothing yet
			;
	} //end switch
	timer_set(&btntimers[i], BTN_T_DEBOUNCE);
}



void button_poll(){
	curinput = ~(PIND & 0b00000011);
	for(uint8_t i=0; i<BTN_BUTTONS; i++){
		if (timer_expired(&btntimers[i])){
			stateswitch(i);
		} //end if timer expired
	} //end for
	oldinput = curinput;
	return;
}

bool btn_state(uint8_t btnstate, uint8_t btn){
	return (btnstates[btn]==btnstate);
}



