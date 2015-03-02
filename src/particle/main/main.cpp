/**
 * @author Raoul Rubien 2015
 */

#include <common/common.h>
#include <common/PortInteraction.h>
#include <uc-core/OutputCore.h>

#include "/usr/lib/avr/include/avr/io.h"
#include "InterruptFlags.h"

extern "C" {
#include <avr/interrupt.h>
#include <avr/io.h>
}
;

#include "uc-core/Particle.h"
#include "uc-core/Indicator.h"

#include "fuses.h"

/**
 * free function declarations
 */
void init(void);
void initCounter1Compare();

/**
 * globals
 */
InterruptFlags IRCollector;

int main(void) {
	Indicator id;
	id.init();
	OutputCore *indicator = &id;

	forever {
		indicator->tick();
	}
}

void initCounter1Compare() {
// disconnect interrupt from port
	TCCR0A unsetBit bit (COM0A1) | bit(COM0A0);

// set waveform generation to normal
	TCCR0A unsetBit bit (WGM01) | bit(WGM00);
	TCCR0B unsetBit bit (WGM02);

// clear counter
	TCNT0 = 0xff;

// set compare value
	OCR0A = 0xff;

// enable interrupt
	TIMSK setBit bit(OCIE0A);
// enable global interrupt
}

/**
 * initialization before main is called
 */
void init() {
	initCounter1Compare();
	sei();
}

