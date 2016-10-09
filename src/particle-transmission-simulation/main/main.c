/**
 * @author Raoul Rubien 2016
 */

#include "uc-core/particle/Particle.h"

//unsigned char __stuff __attribute__((section(".noinit")));

/**
 * A mocked up particle loop. It puts the particle in an initialized reception state.
 */
void processLoop(void) {
    forever {
        process();
    }
}


/**
 * starts a configured node in transmission state with preset transmission buffer
 */
int main(void) {
    constructParticle(&ParticleAttributes);
    ParticleAttributes.discoveryPulseCounters.loopCount = UINT8_MAX;
    ParticleAttributes.node.type = NODE_TYPE_MASTER;

    clearTransmissionPortBuffer(&ParticleAttributes.communication.ports.tx.south);
    ParticleAttributes.communication.ports.tx.south.dataEndPos.bitMask = 1;
    ParticleAttributes.communication.ports.tx.south.dataEndPos.byteNumber = 8;

    // least significant bit (start bit) must be set due tu the physically underlying protocol
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[0] = 0b00100110 | 0x01;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[1] = 0b10101010;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[2] = 0b10101010;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[3] = 0b01010101;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[4] = 0b10101010;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[5] = 0b01111110;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[6] = 0b00100110;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[7] = 0b01010110;


    // configure input/output pins
    IO_PORTS_SETUP;
    SOUTH_TX_LO;
    // setup and enable transmission/reception counter interrupt
    __enableTxRxTimer();
    enableTransmission(&ParticleAttributes.communication.ports.tx.south);

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

//    processLoop();
//    while (ParticleAttributes.node.state != STATE_TYPE_TX_DONE);
//    TIMER_TX_RX_DISABLE_COMPARE_INTERRUPT;
//    ParticleAttributes.node.state = STATE_TYPE_STALE;
    forever;
    return 0;
}


