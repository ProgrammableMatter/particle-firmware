/**
 * @author Raoul Rubien 2016
 */


#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/ParticleTypes.h>
#include <uc-core/delay/delay.h>
#include "uc-core/IoDefinitions.h"
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
    constructParticleState(&ParticleAttributes);
    ParticleAttributes.discoveryPulseCounters.loopCount = UINT8_MAX;
    ParticleAttributes.node.state = STATE_TYPE_TX_START;
    ParticleAttributes.node.type = NODE_TYPE_MASTER;

    bufferBitPointerStart(&ParticleAttributes.ports.tx.south.buffer.pointer);
    ParticleAttributes.ports.tx.south.dataEndPos.byteNumber = 6;
    ParticleAttributes.ports.tx.south.dataEndPos.bitMask = 0x80;

    ParticleAttributes.ports.tx.south.buffer.bytes[6] = 0b10100111; // == 0xa7 => 0xe5
    ParticleAttributes.ports.tx.south.buffer.bytes[5] = 0b10101010; // == 0xaa => 0x55
    ParticleAttributes.ports.tx.south.buffer.bytes[4] = 0b10101010; // == 0xaa => 0x55
    ParticleAttributes.ports.tx.south.buffer.bytes[3] = 0b10100111; // == 0xa7 => 0xe5
    ParticleAttributes.ports.tx.south.buffer.bytes[2] = 0b10101010; // == 0xaa => 0x55
    ParticleAttributes.ports.tx.south.buffer.bytes[1] = 0b10101010; // == 0xaa => 0x55
    ParticleAttributes.ports.tx.south.buffer.bytes[0] = 0b10100111; // == 0xa7 => 0xe5

    // configure input/output pins
    IO_PORTS_SETUP;
    SOUTH_TX_LO;
    // setup and enable transmission/reception counter interrupt
    TIMER_TX_RX_SETUP;
    RX_INTERRUPTS_CLEAR_PENDING;
    TIMER_TX_RX_ENABLE;

    DELAY_US_15
    DELAY_US_15
    DELAY_US_15
    DELAY_US_15
    DELAY_US_15

    DELAY_US_15
    DELAY_US_15
    DELAY_US_15
    DELAY_US_15
    DELAY_US_15

    ParticleAttributes.ports.tx.south.retainTransmission = true;
    ParticleAttributes.ports.tx.south.enableTransmission = true;

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

