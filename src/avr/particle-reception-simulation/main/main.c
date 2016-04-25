/**
 * @author Raoul Rubien 2016
 */


#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/ParticleTypes.h>
#include "uc-core/IoDefinitions.h"
#include "uc-core/interrupts/Interrupts.c"
#include "uc-core/Particle.h"

unsigned char __stuff __attribute__((section(".noinit")));
#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

/**
 * A mocked up particle loop. It puts the particle in an initialized reception state.
 */
FUNC_ATTRS int particleLoop(void) {
    forever {
        particleTick();
        if (ParticleAttributes.node.state == STATE_TYPE_ERRONEOUS) {
            return 1;
        }
    }
}


int main(void) {
    IF_SIMULATION_CHAR_OUT('0');
    IO_PORTS_SETUP; // configure input/output pins
    // setup and enable reception (on pin change) interrupts
    RX_INTERRUPTS_SETUP;
    RX_INTERRUPTS_ENABLE;
    // setup and enable reception counter interrupt
    TIMER_TX_RX_SETUP;
    TIMER_TX_RX_ENABLE;
    //TIMER_TX_RX_TIMEOUT_ENABLE;

    ParticleAttributes.discoveryPulseCounters.loopCount = UINT8_MAX;
    constructParticleState(&ParticleAttributes);

    SREG setBit bit(SREG_I);
    IF_SIMULATION_CHAR_OUT('1');
    ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
    ParticleAttributes.node.state = STATE_TYPE_IDLE;

    return particleLoop();
}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif
