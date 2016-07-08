/**
 * @author Raoul Rubien 2016
 */


#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/particle/ParticleStateTypes.h>
#include "uc-core/configuration/IoPins.h"
#include "uc-core/particle/ParticleCore.h"

//unsigned char __stuff __attribute__((section(".noinit")));

extern inline int particleLoop(void);

/**
 * A mocked up particle loop. It puts the particle in an initialized reception state.
 */
inline int particleLoop(void) {
    forever {
        PARTICLE_MAIN_LOOP_DELAY;
        particleTick();
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
    constructParticleState(&ParticleAttributes);

    DEBUG_CHAR_OUT('1');
    RX_INTERRUPTS_CLEAR_PENDING;
    ParticleAttributes.discoveryPulseCounters.north.isConnected = true;
    ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
    ParticleAttributes.node.state = STATE_TYPE_IDLE;

    clearReceptionBuffers();
    // setup and enable reception and counter interrupts
    __enableReception();

    SEI;
    return particleLoop();
}

