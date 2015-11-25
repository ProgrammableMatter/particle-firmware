/**
 * @author Raoul Rubien 2015
 */

#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/ParticleIoDefinitions.h>
#include <uc-core/Particle.h>
#include <uc-core/ParticleInterruptDefinitions.h>

/**
 * set "void init()" as initialization function
 */
//void init(void) __attribute__ ((naked)) __attribute__ ((section (".init3")));
void init(void);


int main(void) {
    init();

    forever {
        particle_tick();
    }
}

/**
 * initialize ports and interrupts before {@link #main(void)}
 */
void init(void) {
    SETUP_PORT_DIRS; // configure all pin directions
    SETUP_PORT_PINS; // set all IO pins to default

    SETUP_RX_INTERRUPTS; // configure reception interrupts
    ENABLE_RX_INTERRUPTS;

    SETUP_TIMER0_NEIGHBOUR_SENSE_INTERRUPT; // configure timer 0 interrupt for neighbour sensing
    ENABLE_TIMER0_INTERRUPT;

    SREG setBit bit(SREG_I); // finally enable interrupts
}
