/**
 * @author Raoul Rubien 2015
 */
#ifndef DEFAULT_ISR_H
#define DEFAULT_ISR_H

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Globals.h"
#include "ParticleIoDefinitions.h"
#include "ParticleInterruptDefinitions.h"

extern volatile ParticleState GlobalState;

#define TIMER0_ON_INTERRUPT_SHIFT_BACK 2


/**
 * north RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__

ISR(PCINT2_vect) {
#else
#  ifdef __AVR_ATmega16__
    ISR(INT0_vect) {
#  else
#    error
#  endif
#endif
}


/**
 * south RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__

ISR(PCINT0_vect) {
#else
#  ifdef __AVR_ATmega16__
    ISR(INT1_vect) {
#  else
#    error
#  endif
#endif
}


/**
 * east RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__

ISR(PCINT1_vect) {
#else
#  ifdef __AVR_ATmega16__
    ISR(INT2_vect) {
#  else
#    error
#  endif
#endif
}


#if defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)
/**
 * timer/counter 1 match on compare with register A - TCNT1 compare with OCR1A
 * This interrupt routine is triggered when the counter equals to OCR1A value.
 */
ISR(TIMER1_COMPA_vect) {
    TIMER0_NEIGHBOUR_SENSE_DISABLE;
    TCNT0 = 0;

    NORTH_TX_TOGGLE;
    SOUTH_TX_TOGGLE;

    //    TIFR setBit (bit(OCF0B) | bit (OCF0A) | bit(TOV0));
    TIMER0_NEIGHBOUR_SENSE_ENABLE;
}


/**
 * Timer/Counter1 Overflow
 */
ISR(TIMER1_OVF_vect) {
    asm("BREAK");
}

/**
 * Timer/Counter1 Input Capture
 */
ISR(TIMER1_CAPT_vect) {
    asm("BREAK");
}


/**
 * Timer/Counter1 Compare Match B
 */
ISR(TIMER1_COMPB_vect) {
    asm("BREAK");
}


/**
 * External Pin, Power-on Reset, Brown-Out Reset, Watchdog Reset
 */
ISR(_VECTOR(0)) {
    asm("BREAK");
}

#endif

#endif
