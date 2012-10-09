static void init_sampletimer(void)
{
	/* 
	 * Timer 0
	 */

	/* set timer0 to CTC & prescaler 64 → 125kHz increment */
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << CS01);

	OCR0A = 6;		/* TOP */
	TCNT0 = 0;
	/*enable interrupt */
	TIMSK0 |= (1 << OCIE0A);
}

static inline void init_pwm(void)
{
	/* set PB2 as output (buzzer pwm port): */
	DDRB |= (1 << PORTB2);

	/* analog value preselection : */
	OCR1B = 0x007F;

	/* Top value. Timer overflows here. Thus we have a resulting 8bit pwm */
	ICR1 = 0x00FF;

	/* only b-chan , fastpwm (mode 14), no prescale */
	TCCR1A = (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);

	return;
}

inline void synth_init(){
	cli();
	init_sampletimer();
	init_pwm();
	sei();
	return;
}

inline void synth_deinit(){
	return;
}

uint8_t  amp;

static uint8_t synth_mix()
{
	return	amp++;
}

ISR(TIMER0_COMPA_vect)
{
	/* calculate next analog sample value in synth mixer: */
	OCR1B = synth_mix();
}



