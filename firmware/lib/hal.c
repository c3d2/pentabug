#include <pentabug/hal.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <pentabug/lifecycle.h>

#include <pentabug/timer.h>

static volatile uint8_t ir_active = 0;
static int int_skip = 0;
static volatile int16_t wait_time = 0;

static uint16_t button_count[2];
static uint8_t button_pressed[2];

// major interrupt for button handling, every 5ms
inline static void major_interrupt(void) {
	uint8_t i = 0;

	for(i = 0; i < 2; ++i) {
		// button pressed?
		if(PINB & (1 << i)) {
			// pressed for more than 50ms is a click
			if(button_count[i] > 10 && button_count[i] < 200) {
				button_pressed[i] = 1;
			}

			// not pressed, reset
			button_count[i] = 0;
		} else {
			//.count time pressed
			++button_count[i];
		}

		// 1s pressed, request next app
		if(button_count[i] == 200) {
			next_app(i ? 1 : -1);
		}
	}
}

// WARNING: this interrupt is already way too big. extend only in case of emergency!
ISR(TIMER0_COMPA_vect) {
	// generate 38kHz signal

	if(ir_active) {
		PORTD ^= 1 << 2;
	}

	// call button handling less often

	++int_skip;

	if(int_skip >= 64 * 5) {
		int_skip = 0;
		major_interrupt();
	}

	// tell wait_ms() that 1/38 ms has passed

	--wait_time;
}

void init_hw(void) {
	// we need to get real fast (8MHz) to handle 38kHz IR frequency ...

	CLKPR = 0b10000000;
	CLKPR = 0b00000000;

	// initialize timer

	TIMSK0 |= (1 << OCIE0A);

	// calculated and works, but frequency is a little bit off?
	OCR0A = 105;

	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00);

	// activate interrupts

	sei();
}

void reset_hw(void) {
	stop_timer();

	// 0: S1
	// 1: S2
	// 6: MOTOR
	// 7: BUZZR
	PORTB = (1 << 0) | (1 << 1) | (1 << 7);
	DDRB = (1 << 6) | (1 << 7);

	// 0: BUZGND
	// 2: LED2
	// 3: LED2 (+)
	PORTC = (1 << 2) | (1 << 3);
	DDRC = (1 << 0) | (1 << 2) | (1 << 3);

	// 2: IRSEND
	// 3: IRRECV
	// 4: LED
	PORTD = (1 << 4);
	DDRD = (1 << 2) | (1 << 4);

	// do not carry button state

	button_pressed[0] = 0;
	button_pressed[1] = 0;

	// turn ir off

	ir_off();
}

uint8_t button_state(uint8_t btn) {
	return !(PINB & (1 << btn));
}

uint8_t button_clicked(uint8_t btn) {
	uint8_t clicked = button_pressed[btn];
	button_pressed[btn] = 0;
	return clicked;
}

void button_reset(uint8_t btn) {
	button_pressed[btn] = 0;
}

void led_set(uint8_t led, uint8_t state) {
	if(state) {
		led_on(led);
	} else {
		led_off(led);
	}
}

void led_on(uint8_t led) {
	if(led == RIGHT) {
		PORTC &= ~(1 << 2);
	} else {
		PORTD &= ~(1 << 4);
	}
}

void led_off(uint8_t led) {
	if(led == RIGHT) {
		PORTC |= 1 << 2;
	} else {
		PORTD |= 1 << 4;
	}
}

void led_inv(uint8_t led) {
	if(led == RIGHT) {
		PORTC ^= 1 << 2;
	} else {
		PORTD ^= 1 << 4;
	}
}

void motor_set(uint8_t state) {
	if(state) {
		motor_on();
	} else {
		motor_off();
	}
}

void motor_on(void) {
	PORTB |= 1 << 6;
}

void motor_off(void) {
	PORTB &= ~(1 << 6);
}

void motor_inv(void) {
	PORTB ^= 1 << 6;
}

void buzzer_up(void) {
	PORTB |= 1 << 7;
	PORTC &= ~(1 << 0);
}

void buzzer_down(void) {
	PORTB &= ~(1 << 7);
	PORTC |= 1 << 0;
}

void buzzer_inv(void) {
	PORTB ^= 1 << 7;
	PORTC ^= 1 << 0;
}

void buzzer_off(void) {
	PORTB &= ~(1 << 7);
	PORTC &= ~(1 << 0);
}

void wait_ms(uint16_t ms) {
	// TODO: this function seems to be ~10% too fast
	int32_t cycles = ms * (int32_t)64;

	// wait_time is int16_t for performance reasons, so we have to wait multiple times

	while(cycles >= INT16_MAX) {
		cycles -= INT16_MAX;
		wait_time = INT16_MAX;

		while(wait_time > 0) {
			test_stop_app();
		}
	}

	// wait the odd time left

	wait_time = cycles;

	while(wait_time > 0) {
		test_stop_app();
	}
}

void wait_ticks(int16_t ticks) {
	wait_time = ticks;

	while(wait_time > 0) {
		test_stop_app();
	}
}

void ir_on(void) {
	ir_active = 1;
}

void ir_off(void) {
	ir_active = 0;
	PORTD &= ~(1 << 2);
}

void ir_inv(void) {
	if(ir_active) {
		ir_off();
	} else {
		ir_on();
	}
}

void ir_set(uint8_t state) {
	if(state) {
		ir_on();
	} else {
		ir_off();
	}
}

uint8_t ir_recv(void) {
	return !(PIND & (1 << 3));
}

