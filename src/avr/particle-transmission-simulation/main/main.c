/**
 * @author Raoul Rubien 2016
 */


#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/ParticleTypes.h>
#include <util/delay.h>
#include "uc-core/IoDefinitions.h"
#include "uc-core/interrupts/Interrupts.c"
#include "uc-core/Particle.h"

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
 * starts a configured node in transmission state with preset transmission buffer
 */
int main(void) {
    _delay_us(15.0);
    _delay_us(15.0);
    _delay_us(15.0);
    _delay_us(15.0);

    ParticleAttributes.discoveryPulseCounters.loopCount = UINT8_MAX;
    constructParticleState(&ParticleAttributes);
    txBufferBitPointerStart(&ParticleAttributes.ports.tx.south.buffer.pointer);
    ParticleAttributes.ports.tx.south.buffer.bytes[3] = 0b10100111;
    ParticleAttributes.ports.tx.south.buffer.bytes[2] = 0xaa;
    ParticleAttributes.ports.tx.south.buffer.bytes[1] = 0xaa;
    ParticleAttributes.ports.tx.south.buffer.bytes[0] = 0b10100111;

    ParticleAttributes.node.type = NODE_TYPE_MASTER;
    ParticleAttributes.node.state = STATE_TYPE_TX_START;

    IO_PORTS_SETUP; // configure input/output pins

    // setup and enable transmission/reception counter interrupt
    TIMER_TX_RX_SETUP;
    TIMER_TX_RX_ENABLE;
    RX_INTERRUPTS_CLEAR_PENDING;
    SREG setBit bit(SREG_I);

    particleLoop();
    while (ParticleAttributes.node.state != STATE_TYPE_TX_DONE);
    TIMER_TX_RX_DISABLE;
    ParticleAttributes.node.state = STATE_TYPE_STALE;

    forever;
    return 0;
}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif

