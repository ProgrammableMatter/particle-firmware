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
void initClock(void);

int main(void) {
    initClock();
    Indicator();
    forever {
        Indicator_tick();
    }
}

/**
 * initialization before main is called
 */
void init(void) {
	cli();
    initClock();
	Indicator();
	initCounter0Compare();
	sei();
}

void initClock(void) {
    CCP=0xD8;           // unlock CLKSMR
    CLKMSR = 0b00;      // configure internal clock, 8MHz
    CCP=0xD8;           // unlock CLKPSR
    CLKPSR = 0b0000;    // set clock prescale 1
}

void initCounter0Compare(void) {
//#ifdef __AVR_ATtiny20__
// disconnect interrupt from port
	//TCCR0A unsetBit bit (COM0A1);
	//TCCR0A unsetBit bit (COM0A0);

// set waveform generation to normal
	//TCCR0A unsetBit bit (WGM00);
	//TCCR0A unsetBit bit (WGM01);
	//TCCR0B unsetBit bit (WGM02);

// set timer prescaler
    TCCR0B setBit (bit(CS01) | bit(CS00));

	//TCNT0 = 0x00; // clear counter
	//OCR0A = 0x10; // set compare value
// enable interrupt
	//TIMSK setBit bit(OCIE0A);
    TIMSK setBit bit(TOIE0);
//#endif
}
