/**
 * @author Raoul Rubien 2016
 */


#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/particle/ParticleStateTypesCtors.h>
#include <uc-core/delay/delay.h>
#include "uc-core/configuration/IoPins.h"
#include "uc-core/particle/ParticleCore.h"

//unsigned char __stuff __attribute__((section(".noinit")));

extern FUNC_ATTRS int particleLoop(void);
/**
 * A mocked up particle loop. It puts the particle in an initialized reception state.
 */
FUNC_ATTRS int particleLoop(void) {
    forever {
        PARTICLE_LOOP_DELAY;
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
//    ParticleAttributes.node.state = STATE_TYPE_TX_START;
    ParticleAttributes.node.type = NODE_TYPE_MASTER;
    ParticleAttributes.ports.xmissionState = STATE_TYPE_XMISSION_TYPE_ENABLED_TX;

    clearTransmissionPortBuffer(&ParticleAttributes.ports.tx.south);
    ParticleAttributes.ports.tx.south.dataEndPos.bitMask = 1;
    ParticleAttributes.ports.tx.south.dataEndPos.byteNumber = 7;

    // least significant bit (start bit) must be set due tu the physically underlying protocol
    ParticleAttributes.ports.tx.south.buffer.bytes[0] = 0b00100110 | 0x01;
    ParticleAttributes.ports.tx.south.buffer.bytes[1] = 0b10101010;
    ParticleAttributes.ports.tx.south.buffer.bytes[2] = 0b10101010;
    ParticleAttributes.ports.tx.south.buffer.bytes[3] = 0b01010101;
    ParticleAttributes.ports.tx.south.buffer.bytes[4] = 0b10101010;
    ParticleAttributes.ports.tx.south.buffer.bytes[5] = 0b01111110;
    ParticleAttributes.ports.tx.south.buffer.bytes[6] = 0b00100110;


    // configure input/output pins
    IO_PORTS_SETUP;
    SOUTH_TX_LO;
    // setup and enable transmission/reception counter interrupt
    __enableTxRxTimer();
    enableTransmission(&ParticleAttributes.ports.tx.south);

    DELAY_US_15;
    DELAY_US_15;
    DELAY_US_15;
    DELAY_US_15;
    DELAY_US_15;

    DELAY_US_15;
    DELAY_US_15;
    DELAY_US_15;
    DELAY_US_15;
    DELAY_US_15;

    SREG setBit bit(SREG_I);

//    particleLoop();
//    while (ParticleAttributes.node.state != STATE_TYPE_TX_DONE);
//    TIMER_TX_RX_DISABLE_COMPARE_INTERRUPT;
//    ParticleAttributes.node.state = STATE_TYPE_STALE;
    forever;
}


