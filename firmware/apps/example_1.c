#include <avr/io.h>
#include <pentabug/app.h>

// was bedeuten die einzelnen Worte?
// was passiert?
// geht das einfacher?

static void example(void) {
	if(!(PINB & 1)) {
		PORTD &= ~(1 << 4);
	}


	if(!(PINB & 2)) {
		PORTC &= ~(1 << 2);
	}
}

REG(example);
