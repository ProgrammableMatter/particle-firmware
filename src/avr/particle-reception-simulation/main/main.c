/**
 * @author Raoul Rubien 2016
 */
#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/ParticleTypes.h>
#include "uc-core/IoDefinitions.h"
#include "uc-core/interrupts/Interrupts.c"
#include "uc-core/interrupts/Reception.h"
#include "uc-core/Particle.h"

#define FUNC_ATTRS inline

/**
 * A mocked up particle loop. It puts the particle in an initialized reception state.
 */
FUNC_ATTRS int particleLoop(void) {
    SREG setBit bit(SREG_I);
    forever {
        particleTick();
        if (ParticleAttributes.node.state == STATE_TYPE_ERRONEOUS) {
            return 1;
        }
    }
}


int main(void) {
    IO_PORTS_SETUP; // configure input/output pins
    // setup and enable reception (on pin change) interrupts
    RX_INTERRUPTS_SETUP;
    RX_INTERRUPTS_ENABLE;
    // setup and enable reception counter interrupt
    TIMER_TX_RX_SETUP;
    TIMER_TX_RX_ENABLE;
    TIMER_TX_RX_TIMEOUT_ENABLE;

    ParticleAttributes.discoveryPulseCounters.loopCount = UINT8_MAX;
    constructParticleState(&ParticleAttributes);

    ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
    ParticleAttributes.node.state = STATE_TYPE_IDLE;

    return particleLoop();
}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif

