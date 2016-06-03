/**
 * @author Raoul Rubien 2016
 */


#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/ParticleTypes.h>
#include "uc-core/IoDefinitions.h"
#include "uc-core/ParticleCore.h"

//unsigned char __stuff __attribute__((section(".noinit")));
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


/**
 * starts
 */
int main(void) {
    DEBUG_CHAR_OUT('0');
    // configure input/output pins
    IO_PORTS_SETUP;

    ParticleAttributes.discoveryPulseCounters.loopCount = UINT8_MAX;
    constructParticleState(&ParticleAttributes);

    DEBUG_CHAR_OUT('1');
    RX_INTERRUPTS_CLEAR_PENDING;
    ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
    ParticleAttributes.node.state = STATE_TYPE_IDLE;

    clearReceptionBuffers();
    // setup and enable reception and counter interrupts
    __enableTxRx();

    SREG setBit bit(SREG_I);
    return particleLoop();
}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif

