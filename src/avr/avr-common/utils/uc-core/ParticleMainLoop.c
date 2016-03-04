/**
* @author Raoul Rubien 2015
*/
#include "ParticleMainLoop.h"

#include <avr/interrupt.h>
#include <common/common.h>
#include "ParticleIoDefinitions.h"
#include "ParticleInterruptDefinitions.h"
#include "DefaultIsr.h"
#include "Particle.h"

#ifdef __AVR_ATtiny1634__
#  include <util/delay.h>
#endif

void init(void);

int particleLoop(void) {
    init();

#ifdef __AVR_ATtiny1634__
    _delay_ms(0.5); // wait for all nodes to be ready
#endif

    ParticleAttributes.state = STATE_TYPE_ACTIVE;
    forever {
        particleTick();
        if (ParticleAttributes.state == STATE_TYPE_ERRONEOUS) {
            return 1;
        }
    }
}


/**
 * Sets up ports and interrupts but does not enable the global interrupt (I-flag in SREG)
 */
void init(void) {
    IO_PORTS_SETUP; // configure input/output pins
    particleSnapshotRxFlanks(); // take rx input signal snapshot for later flank sense control
    RX_INTERRUPTS_SETUP; // configure input pins interrupts
    RX_INTERRUPTS_ENABLE; // enable input pin interrupts
    TIMER0_NEIGHBOUR_SENSE_SETUP; // configure timer interrupt for neighbour sensing
}