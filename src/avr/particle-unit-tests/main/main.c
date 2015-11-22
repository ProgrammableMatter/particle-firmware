/**
 * @author Raoul Rubien 2015
 */

#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/ParticleIoDefinitions.h>
#include <uc-core/Particle.h>

/**
 * set "void init()" as initialization function
 */
void init(void) __attribute__ ((naked)) __attribute__ ((section (".init3")));

int main(void) {
    forever {
        particle_tick();
    }
}

/**
 * initialize ports and interrupts before {@link #main(void)}
 */
void init(void) {
    cli();
    LED_SETUP;
    TESTPOINT_SETUP;
    NORTH_TX_SETUP;
    NORTH_RX_SETUP;
    NORTH_RX_SWITCH_SETUP;
    SOUTH_TX_SETUP;
    SOUTH_RX_SETUP;
    SOUTH_RX_SWITCH_SETUP;
    sei();
}
