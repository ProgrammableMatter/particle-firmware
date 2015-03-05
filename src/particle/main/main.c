/**
 * @author Raoul Rubien 2015
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <common/common.h>
#include <common/PortInteraction.h>

#include "uc-core/Particle.h"
#include "uc-core/Indicator.h"

void init(void) __attribute__ ((naked)) __attribute__ ((section (".init3")));
void initCounter0Compare(void);

int main(void) {
	init();
	init();
	forever {
		Indicator_tick();
	}
}

/**
 * initialization before main is called
 */
void init(void) {
	cli();
	Indicator();
	initCounter0Compare();
	sei();
}

void initCounter0Compare(void) {
//#ifdef __AVR_ATtiny20__
// disconnect interrupt from port
	TCCR0A unsetBit bit (COM0A1);
	TCCR0A unsetBit bit (COM0A0);

// set waveform generation to normal
	TCCR0A unsetBit bit (WGM00);
	TCCR0A unsetBit bit (WGM01);
	TCCR0B unsetBit bit (WGM02);

// clear counter
	TCNT0 = 0x00;

// set compare value
	OCR0A = 0x10;

// enable interrupt
	TIMSK setBit bit(OCIE0A);
//#endif
}
