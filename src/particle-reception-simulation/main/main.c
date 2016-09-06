/**
 * @author Raoul Rubien 2016
 */

#include "uc-core/particle/ParticleCore.h"

//unsigned char __stuff __attribute__((section(".noinit")));

/**
 * A mocked up particle loop. It puts the particle in an initialized reception state.
 */
extern inline void processLoop(void);

inline void processLoop(void) {
    forever {
        process();
    }
}

/**
 * starts
 */
int main(void) {
    DELAY_US_150;

    DEBUG_CHAR_OUT('0');
    // configure input/output pins
    IO_PORTS_SETUP;

    ParticleAttributes.discoveryPulseCounters.loopCount = UINT8_MAX;
    constructParticle(&ParticleAttributes);

    DEBUG_CHAR_OUT('1');
    RX_INTERRUPTS_CLEAR_PENDING;
    ParticleAttributes.discoveryPulseCounters.north.isConnected = true;
    ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
    ParticleAttributes.node.state = STATE_TYPE_IDLE;

    clearReceptionBuffers();
    // setup and enable reception and counter interrupts
    __enableReception();

    SEI;
    processLoop();
    return 0;
}

