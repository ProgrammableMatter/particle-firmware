/**
 * @author Raoul Rubien 2016
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/configuration/IoPins.h>
#include <uc-core/particle/Globals.h>
#include <uc-core/particle/ParticleStateTypesCtors.h>

#define TIMER_TX_ENABLE_COMPARE_TOP_INTERRUPT \
    __TIMER1_INTERRUPT_CLEAR_PENDING_COMPARE_A; \
    __TIMER1_COMPARE_A_INTERRUPT_ENABLE

#define TIMER_TX_ENABLE_COMPARE_CENTER_INTERRUPT \
    __TIMER1_INTERRUPT_CLEAR_PENDING_COMPARE_B; \
    __TIMER1_COMPARE_B_INTERRUPT_ENABLE

//unsigned char__pad __attribute__((section(".noinit")));

#define TIMER_TX_COUNTER_TOP_VECTOR TIMER1_COMPA_vect
// int #7
ISR(TIMER_TX_COUNTER_TOP_VECTOR) {
    OCR1A += DEFAULT_TX_RX_CLOCK_DELAY;
    if (ParticleAttributes.communication.ports.tx.south.buffer.pointer.bitMask &
        ParticleAttributes.communication.ports.tx.south.buffer.bytes[ParticleAttributes.communication.ports.tx.
                south.buffer.pointer.byteNumber]) {
        SOUTH_TX_HI;
    } else {
        SOUTH_TX_LO;
    }
}

// triggers on bit transmission:
// reception at the receiver side of this transmission is inverted
#define TIMER_TX_COUNTER_CENTER_VECTOR TIMER1_COMPB_vect
// int #8
ISR(TIMER_TX_COUNTER_CENTER_VECTOR) {
    OCR1B += DEFAULT_TX_RX_CLOCK_DELAY;
    if (isDataEndPosition(&ParticleAttributes.communication.ports.tx.south)) {
        // return signal to default
        SOUTH_TX_LO; // inverted on receiver side
        TIMER_TX_RX_COUNTER_DISABLE;
        ParticleAttributes.node.state = STATE_TYPE_IDLE;
    } else {
        // write data bit to output (inverted)
        if (ParticleAttributes.communication.ports.tx.south.buffer.pointer.bitMask &
            ParticleAttributes.communication.ports.tx.south.buffer.bytes[ParticleAttributes.communication.ports.tx.south.buffer.pointer.byteNumber]) {
            SOUTH_TX_LO;
        } else {
            SOUTH_TX_HI;
        }
        bufferBitPointerIncrement(&ParticleAttributes.communication.ports.tx.south.buffer.pointer);
    }
}

inline void initTransmission(void) {
    constructParticle(&ParticleAttributes);
    ParticleAttributes.node.state = STATE_TYPE_START;
    ParticleAttributes.node.type = NODE_TYPE_MASTER;

    bufferBitPointerStart(&ParticleAttributes.communication.ports.tx.south.buffer.pointer);
    ParticleAttributes.communication.ports.tx.south.dataEndPos.byteNumber = 7;
    ParticleAttributes.communication.ports.tx.south.dataEndPos.bitMask = 0x1;

    // the bytes to transmit
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[0] = 0b10100110 | 0x1;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[1] = 0b10101010;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[2] = 0b10101010;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[3] = 0b01010101;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[4] = 0b10101010;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[5] = 0b01111110;
    ParticleAttributes.communication.ports.tx.south.buffer.bytes[6] = 0b00100110;

    SOUTH_TX_SETUP;
    // return signal to default (locally low means high at receiver side)
    SOUTH_TX_LO;

    TIMER_TX_RX_COUNTER_SETUP;

    OCR1A = DEFAULT_TX_RX_CLOCK_DELAY;
    OCR1B = DEFAULT_TX_RX_CLOCK_DELAY + (DEFAULT_TX_RX_CLOCK_DELAY / 2);
    // start transmission with TIMER_TX_COUNTER_TOP_VECTOR
    TCNT1 = DEFAULT_TX_RX_CLOCK_DELAY / 2;

    TIMER_TX_ENABLE_COMPARE_TOP_INTERRUPT;
    TIMER_TX_ENABLE_COMPARE_CENTER_INTERRUPT;
}

EMPTY_INTERRUPT(TIMER1_OVF_vect)

extern inline void initTransmission(void);

int main(void) {
    initTransmission();
    // wait until receiver is ready
    DELAY_US_150;

    ParticleAttributes.node.state = STATE_TYPE_UNDEFINED;

    SEI;
    TIMER_TX_RX_COUNTER_ENABLE;
    while (ParticleAttributes.node.state != STATE_TYPE_IDLE);
    CLI;

    ParticleAttributes.node.state = STATE_TYPE_STALE;
    return 0;
}
