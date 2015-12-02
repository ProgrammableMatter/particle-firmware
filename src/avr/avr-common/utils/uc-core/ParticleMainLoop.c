/**
 * @author Raoul Rubien 2015
 */
#include "ParticleMainLoop.h"

#include <avr/interrupt.h>
#include <common/common.h>
#include "ParticleIoDefinitions.h"
#include "ParticleInterruptDefinitions.h"
#include "Globals.h"
#include "DefaultIsr.h" // registers ISR vectors
#include "Particle.h"

#ifndef __AVR_ATtmega16__
#endif

void init(void);

int particleLoop(void) {
    init();
#ifndef __AVR_ATmega16__
    _delay_ms(0.5); // wait for all nodes to be ready
#endif

    GlobalState.state = STATE_TYPE_ACTIVE;
    forever {
        particle_tick();
        if (GlobalState.state == STATE_TYPE_ERRONEOUS) {
            return 1;
        }
    }
}


/**
 * Sets up ports and interrupts but does not enable the global interrupt (I-flag in SREG)
 */
void init(void) {
    SETUP_PORT_DIRS; // configure all pin directions
    SETUP_PORT_PINS; // set all IO pins to default

    SETUP_RX_INTERRUPTS; // configure reception interrupts
    ENABLE_RX_INTERRUPTS;

    SETUP_TIMER0_NEIGHBOUR_SENSE_INTERRUPT; // configure timer 0 interrupt for neighbour sensing
}