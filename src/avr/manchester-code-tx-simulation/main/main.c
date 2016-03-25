/**
 * @author Raoul Rubien 2016
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <common/common.h>
#include <uc-core/IoDefinitions.h>
#include <uc-core/Globals.h>

extern volatile ParticleState ParticleAttributes;

/**
 * PORTD2 - pin 19 - compare B interrupt handler duration
 * PORTD1 - pin 20 - compare A interrupt handler duration
 * PORTD0 - pin 21 - signal output pin - manchester code
 * PORTA0 - pin 22 - clock out pin
 * PORTA1 - pin 23 - data out bit pin
 */

#define COUNTER_1_SETTINGS_TOP 500
#define COUNTER_1_SETTINGS_CENTER (500 / 2)
#define COUNTER_1_SETTINGS_PRESCALER (0 << CS12) | (0 << CS11) | (1 << CS10)
#define COUNTER_1_SETTINGS_PRESCALER_DISCONNECT (1 << CS12) | (1 << CS11) | (1 << CS10)
#define COUNTER_1_SETTINGS_TRANSMISSION_PAUSE_DELAY 10

uint8_t bitMask = 0;
uint8_t data;
uint8_t transmissionPauseCounter;


// trigger clock
ISR(TIMER1_COMPA_vect) {
    PORTD |= (1 << PORTD1);
    TCNT1 = 0;

    if (bitMask == 0) {
        PORTA &= ~(1 << PORTA0);
        PORTA &= ~(1 << PORTA1);
        PORTD |= (1 << PORTD0);
        transmissionPauseCounter = COUNTER_1_SETTINGS_TRANSMISSION_PAUSE_DELAY;
    } else {
        // clock out high
        PORTA |= (1 << PORTA0);

        // bit value out
        uint8_t bitValue = ((bitMask & data));
        if (bitValue) {
            PORTA |= (1 << PORTA1);
        } else {
            PORTA &= ~(1 << PORTA1);
        }

        // generated signal out
        if (!(bitMask & data)) {
//            PORTD |= (1 << PORTD0);
            SOUTH_TX_LO;
        } else {
//            PORTD &= ~(1 << PORTD0);
            SOUTH_TX_HI;
        }
    }
    PORTD &= ~(1 << PORTD1);
}

// trigger transmission
ISR(TIMER1_COMPB_vect) {
    PORTD |= (1 << PORTD2);

    if (transmissionPauseCounter == 0) {
        if (bitMask != 0) {
            // clock out low
            PORTA &= ~(1 << PORTA0);

            // bit value out
//            uint8_t bitValue = ((bitMask & data));
            if (bitMask & data) {
                PORTA |= (1 << PORTA1);
            } else {
                PORTA &= ~(1 << PORTA1);
            }

            // generated signal out
            if ((bitMask & data)) {
//                PORTD |= (1 << PORTD0);
                SOUTH_TX_LO;

            } else {
//                PORTD &= ~(1 << PORTD0);
                SOUTH_TX_HI;
            }
            bitMask <<= 1;
        }
    } else {
        // pause transmission
        if (--transmissionPauseCounter == 0) {
            bitMask = 1;
            TCNT1 = COUNTER_1_SETTINGS_CENTER + 1;
            TIMSK |= (1 << OCIE1A);
        } else { // start transmission
            TCNT1 = 0;
            TIMSK &= ~(1 << OCIE1A);
        }

    }
    PORTD &= ~(1 << PORTD2);
}


void init(void) {

    ParticleAttributes.node.type = NODE_TYPE_MASTER;
    data = 0b11100101;
    transmissionPauseCounter = COUNTER_1_SETTINGS_TRANSMISSION_PAUSE_DELAY;

    SOUTH_TX_SETUP;
    SOUTH_TX_HI;

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

    OCR1B = COUNTER_1_SETTINGS_CENTER;
    OCR1A = COUNTER_1_SETTINGS_TOP;
}

#include <util/delay.h>

int main(void) {
    init();
    _delay_ms(4.5);

    SREG setBit bit(SREG_I);
    TCCR1B |= COUNTER_1_SETTINGS_PRESCALER;

// TODO:
// 1) transmit one byte
// 2) disable interrupt:
// SREG unsetBit bit(SREG_I);
// TCCR1B &= ~COUNTER_1_SETTINGS_PRESCALER_DISCONNECT;
    while (1);
    return 0;
}
