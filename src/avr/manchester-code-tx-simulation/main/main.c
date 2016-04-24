/**
 * @author Raoul Rubien 2016
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/IoDefinitions.h>
#include <uc-core/Globals.h>
#include <util/delay.h>

extern volatile ParticleState ParticleAttributes;
uint8_t bitMask  __attribute__((section(".noinit")));
//unsigned char__stuff __attribute__((section(".noinit")));

#define COUNTER_1_SETTINGS_TOP 500
#define COUNTER_1_SETTINGS_CENTER (500 / 2)
#define COUNTER_1_SETTINGS_PRESCALER ((0 << CS12) | (0 << CS11) | (1 << CS10))
#define COUNTER_1_SETTINGS_PRESCALER_DISCONNECT ((1 << CS12) | (1 << CS11) | (1 << CS10))


// triggers on signal clock (signal rectification)
ISR(TIMER1_COMPA_vect) {
    TCNT1 = 0;
    if (bitMask == 0) {
        // stop after one transmission
        TIMSK unsetBit(1 << OCIE1A);
    } else {
        // rectify (invert accordint to next data bit) signal before upcoming data transition
        if (!(bitMask & ParticleAttributes.ports.tx.south.buffer.bytes[0])) {
            SOUTH_TX_LO;
        } else {
            SOUTH_TX_HI;
        }
    }
}

// triggers on bit transmission
// reception on receptor of this transmission is inverted
ISR(TIMER1_COMPB_vect) {

    if (bitMask == 0) {
        // stop after one transmission
        TIMSK unsetBit(1 << OCIE1B);
        TCCR1B &= ~COUNTER_1_SETTINGS_PRESCALER_DISCONNECT;
        // return signal to default
        SOUTH_TX_LO; // inverted on receiver side
        ParticleAttributes.node.state = STATE_TYPE_TX_DONE;
    } else {
        // write data bit to output (inverted)
        if ((bitMask & ParticleAttributes.ports.tx.south.buffer.bytes[0])) {
            SOUTH_TX_LO;
        } else {
            SOUTH_TX_HI;
        }
        bitMask <<= 1;
        TIMSK setBit (1 << OCIE1A);
    }
}


inline void initTransmission(void) {
    bitMask = 0;
    constructParticleState(&ParticleAttributes);
    ParticleAttributes.node.state = STATE_TYPE_START;
    ParticleAttributes.node.type = NODE_TYPE_MASTER;
    // the byte to transmit
    ParticleAttributes.ports.tx.south.buffer.bytes[0] = 0b11100101;

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

    TIMSK |= (1 << OCIE1B) | (0 << OCIE1A) | (0 << TOIE1);

    OCR1A = COUNTER_1_SETTINGS_TOP;
    OCR1B = COUNTER_1_SETTINGS_CENTER;
    // start transmission with compare A interrupt
    TCNT1 = COUNTER_1_SETTINGS_CENTER + 1;

    bitMask = 1;
}


int main(void) {
    initTransmission();
    // wait until receiver is ready
    _delay_us(37.0);

    ParticleAttributes.node.state = STATE_TYPE_TX_START;
    SREG setBit bit(SREG_I);
    TCCR1B |= COUNTER_1_SETTINGS_PRESCALER;

    while (ParticleAttributes.node.state != STATE_TYPE_TX_DONE);
    SREG unsetBit bit(SREG_I);
    ParticleAttributes.node.state = STATE_TYPE_STALE;
    return 0;
}
