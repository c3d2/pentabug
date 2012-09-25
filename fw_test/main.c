#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>


#define LED_L (1 << PORTC0)
#define LED_R (1 << PORTC2)
#define MOTOR_ON (0<<PORTB1)
#define MOTOR_OFF (1<<PORTB1)



static void init_leds(void){
	//enable LED channels as output
	DDRC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);
	// both LEDs off
	PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3));
 	return;
}

static void inline led_on(int leds){
 	PORTC |= leds;
}

static void inline led_off(int leds){
	PORTC &= ~leds;
	return;
}

static void inline buzzr_off(void){
	PORTC &= ~(1 << PORTC5);
	PORTB &= ~(1 << PORTB2);
}

static void init_buzzr(void){
	//its on B2 and C5, for reasons
	DDRC |= (1 << PORTC5);
	DDRB |= (1 << PORTB2);
	//switch it off
	buzzr_off();
	return;
}

static void buzzr_up(void){
	PORTC &= ~(1 << PORTC5);
	PORTB |= (1 << PORTB2);
	return;
}

static void buzzr_down(void){
	PORTC |= (1 << PORTC5);
	PORTB &= ~(1 << PORTB2);
}



static void buzzr_inv(void){
	PORTC ^= (1 << PORTC5);
	PORTB ^= (1 << PORTB2);
}


static void init_motor(void)
{
	/* vibration motor on B1, initially off: */
	DDRB  |= (1 << PORTB1);
	PORTB &= ~( 1<<PORTB1);
	return;
}

static void set_motor(int val){
	PORTB = ~(val);
	PORTB = PORTB;
	return;
}


static void init_switch(void){
	DDRD &= ~( (1 << PORTD1) | (1<<PORTD0));
	return;
}

static void blinkrattlebeep(void){
	led_off(LED_L|LED_R);
	_delay_ms(100);	
	led_on(LED_L);
	_delay_ms(100);	
	led_off(LED_L);
	led_on(LED_R);
	_delay_ms(100);
	led_off(LED_R);	
	set_motor(MOTOR_ON);
	_delay_ms(200);
	set_motor(MOTOR_OFF);
	buzzr_up();
	for (int i=0; i<100; i++){
		_delay_ms(2);
		buzzr_inv();
	};
	buzzr_off();
}

static void mode_motortest(void)
{
	set_motor(MOTOR_ON);
	_delay_ms(400);
	set_motor(MOTOR_OFF);
	_delay_ms(500);
	return;
}


static void mode_blinktest(void)
{
	led_off(LED_L|LED_R);
	_delay_ms(100);	
	led_on(LED_L);
	_delay_ms(100);	
	led_off(LED_L);
	led_on(LED_R);
	_delay_ms(100);
	led_off(LED_R);	
	return;
}

static void mode_beeptest(void)
{
	buzzr_up();
	for (int i=0; i<100; i++){
		_delay_ms(2);
		buzzr_inv();
	};
	buzzr_off();
	return;
}

int main(void)
{

	/* hardware initialisation: */
	init_motor();
	init_buzzr();
	init_leds();
	init_switch();
	uint8_t mode = 0;

	for(;;) /* ever */  {
		//do something

		//TODO: check switches and change mode

		switch(mode){
			case 1: 
				mode_blinktest();
				break;
			case 2:
				mode_motortest();
				break;
			case 3: 
				mode_beeptest();
				break;
			case 4:
			default:
				mode=0;
			case 0:
				blinkrattlebeep();

		}
		
		switch( PIND & 3){
			case 1:
				mode++;
				break;
			case 2:
				mode --;
				break;
			case 3:
			default:
				;
		}

	}
	/* never  return 0; */
	return 0;
}


