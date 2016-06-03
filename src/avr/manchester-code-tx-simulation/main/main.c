/**
 * @author Raoul Rubien 2016
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/IoDefinitions.h>
#include <uc-core/Globals.h>
#include <uc-core/delay/delay.h>

//unsigned char__pad __attribute__((section(".noinit")));

#define COUNTER_1_SETTINGS_TOP_INTERRUPT_LATENCY 18
#define COUNTER_1_SETTINGS_TOP (DEFAULT_TX_RX_COMPARE_TOP_VALUE - COUNTER_1_SETTINGS_TOP_INTERRUPT_LATENCY)
#define COUNTER_1_SETTINGS_CENTER (((COUNTER_1_SETTINGS_TOP+COUNTER_1_SETTINGS_TOP_INTERRUPT_LATENCY) / 2) - COUNTER_1_SETTINGS_TOP_INTERRUPT_LATENCY)
#define COUNTER_1_SETTINGS_PRESCALER ((0 << CS12) | (0 << CS11) | (1 << CS10))
#define COUNTER_1_SETTINGS_PRESCALER_DISCONNECT ((1 << CS12) | (1 << CS11) | (1 << CS10))


// triggers on signal clock (signal rectification)
#define TX_COUNTER_TOP TIMER1_COMPA_vect

ISR(TX_COUNTER_TOP) {
    TCNT1 = 0;

    // rectify (invert according to next data bit) signal before upcoming data transition
    //if (ParticleAttributes.ports.tx.south.buffer.pointer.bitMask & ParticleAttributes.ports.tx.south.buffer.bytes[0]) {
    if (ParticleAttributes.ports.tx.south.buffer.pointer.bitMask &
        ParticleAttributes.ports.tx.south.buffer.bytes[ParticleAttributes.ports.tx.
                south.buffer.pointer.byteNumber]) {
        SOUTH_TX_HI;
//            IF_SIMULATION_INT16_OUT(TCNT1);
    } else {
        SOUTH_TX_LO;
//            IF_SIMULATION_INT16_OUT(TCNT1);
    }
}

// triggers on bit transmission:
// reception at the receiver side of this transmission is inverted
#define TX_COUNTER_CENTER TIMER1_COMPB_vect

ISR(TX_COUNTER_CENTER) {

    if (isDataEndPosition(&ParticleAttributes.ports.tx.south)) {
        // stop after one transmission
//        TIMSK unsetBit(1 << OCIE1B);
        TCCR1B unsetBit COUNTER_1_SETTINGS_PRESCALER_DISCONNECT;
        // return signal to default
        SOUTH_TX_LO; // inverted on receiver side
//        IF_SIMULATION_INT16_OUT(TCNT1);
        ParticleAttributes.node.state = STATE_TYPE_TX_DONE;
    } else {
        // write data bit to output (inverted)
        if (ParticleAttributes.ports.tx.south.buffer.pointer.bitMask &
            ParticleAttributes.ports.tx.south.buffer.bytes[ParticleAttributes.ports.tx.south.buffer.pointer.byteNumber]) {
            SOUTH_TX_LO;
//            IF_SIMULATION_INT16_OUT(TCNT1);
        } else {
            SOUTH_TX_HI;
//            IF_SIMULATION_INT16_OUT(TCNT1);
        }
        bufferBitPointerIncrement(&ParticleAttributes.ports.tx.south.buffer.pointer);
    }
}

inline void initTransmission(void) {
    constructParticleState(&ParticleAttributes);
    ParticleAttributes.node.state = STATE_TYPE_START;
    ParticleAttributes.node.type = NODE_TYPE_MASTER;

    bufferBitPointerStart(&ParticleAttributes.ports.tx.south.buffer.pointer);
    ParticleAttributes.ports.tx.south.dataEndPos.byteNumber = 6;
    ParticleAttributes.ports.tx.south.dataEndPos.bitMask = 0x80;


//    // the byte to transmit
//    ParticleAttributes.ports.tx.south.buffer.bytes[6] = 0b10100111; // == 0xa7 => 0xe5
//    ParticleAttributes.ports.tx.south.buffer.bytes[5] = 0b10101010; // == 0xaa => 0x55
//    ParticleAttributes.ports.tx.south.buffer.bytes[4] = 0b10101010; // == 0xaa => 0x55
//    ParticleAttributes.ports.tx.south.buffer.bytes[3] = 0b10100111; // == 0xa7 => 0xe5
//    ParticleAttributes.ports.tx.south.buffer.bytes[2] = 0b10101010; // == 0xaa => 0x55
//    ParticleAttributes.ports.tx.south.buffer.bytes[1] = 0b10101010; // == 0xaa => 0x55
//    ParticleAttributes.ports.tx.south.buffer.bytes[0] = 0b10100111; // == 0xa7 => 0xe5

    // the byte to transmit
    ParticleAttributes.ports.tx.south.buffer.bytes[6] = 0b00100110;
    ParticleAttributes.ports.tx.south.buffer.bytes[5] = 0b01111110;
    ParticleAttributes.ports.tx.south.buffer.bytes[4] = 0b10101010;
    ParticleAttributes.ports.tx.south.buffer.bytes[3] = 0b01010101;
    ParticleAttributes.ports.tx.south.buffer.bytes[2] = 0b10101010;
    ParticleAttributes.ports.tx.south.buffer.bytes[1] = 0b10101010;
    ParticleAttributes.ports.tx.south.buffer.bytes[0] = 0b10100111;

    SOUTH_TX_SETUP;
    // return signal to default (high on receiver side)
    SOUTH_TX_LO;

    DDir setOut Pin0;
    DOut setHi Pin0;
    ADir setOut Pin0;
    ADir setOut Pin1;
    DDir setOut Pin1;
    DDir setOut Pin2;

    TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0);
    TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << CS12) | (0 << CS11) | (0 << CS10);

    TCCR1A |= (0 << WGM11) | (0 << WGM10);
    TCCR1B |= (0 << WGM13) | (0 << WGM12);

    TIMSK |= (1 << OCIE1B) | (1 << OCIE1A) | (0 << TOIE1);

    if ((TIFR & (1 << OCF1A)) != 0)
        TIFR = (1 << OCF1A);
    if ((TIFR & (1 << OCF1B)) != 0)
        TIFR = (1 << OCF1B);

    OCR1A = COUNTER_1_SETTINGS_TOP;
    OCR1B = COUNTER_1_SETTINGS_CENTER;
    // start transmission with compare A interrupt
    TCNT1 = COUNTER_1_SETTINGS_CENTER + 1;
}

extern inline void initTransmission(void);

int main(void) {
    initTransmission();
    // wait until receiver is ready
    DELAY_US_15;

    ParticleAttributes.node.state = STATE_TYPE_TX_START;

    SREG setBit bit(SREG_I);

    TCCR1B |= COUNTER_1_SETTINGS_PRESCALER; // start transmission
    while (ParticleAttributes.node.state != STATE_TYPE_TX_DONE);

    SREG unsetBit bit(SREG_I);
    ParticleAttributes.node.state = STATE_TYPE_STALE;
    return 0;
}
