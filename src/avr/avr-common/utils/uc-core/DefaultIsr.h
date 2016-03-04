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

extern volatile ParticleState ParticleAttributes;

#define TIMER0_ON_INTERRUPT_SHIFT_BACK 2


/**
 * north RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__

ISR(PCINT2_vect)
#else
#  ifdef __AVR_ATmega16__
ISR(INT0_vect)
#  else
#    error
#  endif
#endif
{
    unsigned char isRxSignalLow = NORTH_RX;

    if (isRxSignalLow && ParticleAttributes.rxInterruptFlankStates.north != isRxSignalLow) { // on falling edge
        // count incoming discovery pulse events
        if (ParticleAttributes.state == STATE_TYPE_NEIGHBOURS_DISCOVERY) {
            if (ParticleAttributes.rxDiscoveryPulseCounter.north < RX_PULSE_COUNTER_MAX) {
                ParticleAttributes.rxDiscoveryPulseCounter.north++;
            }
        }
    }
    ParticleAttributes.rxInterruptFlankStates.north = isRxSignalLow;
}


/**
 * south RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__

ISR(PCINT0_vect)
#else
#  ifdef __AVR_ATmega16__
    ISR(INT1_vect)
#  else
#    error
#  endif
#endif
{
    unsigned char isRxSignalLow = SOUTH_RX;

    if (isRxSignalLow && ParticleAttributes.rxInterruptFlankStates.south != isRxSignalLow) { // on falling edge
        // count incoming discovery pulse events
        if (ParticleAttributes.state == STATE_TYPE_NEIGHBOURS_DISCOVERY) {
            if (ParticleAttributes.rxDiscoveryPulseCounter.south < RX_PULSE_COUNTER_MAX) {
                ParticleAttributes.rxDiscoveryPulseCounter.south++;
            }
        }
    }
    ParticleAttributes.rxInterruptFlankStates.south = isRxSignalLow;
}


/**
 * east RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__

ISR(PCINT1_vect)
#else
#  ifdef __AVR_ATmega16__
    ISR(INT2_vect)
#  else
#    error
#  endif
#endif
{
    unsigned char isRxSignalLow = EAST_RX;

    if (isRxSignalLow && ParticleAttributes.rxInterruptFlankStates.east != isRxSignalLow) { // on falling edge
        // count incoming discovery pulse events
        if (ParticleAttributes.state == STATE_TYPE_NEIGHBOURS_DISCOVERY) {
            if (ParticleAttributes.rxDiscoveryPulseCounter.east < RX_PULSE_COUNTER_MAX) {
                ParticleAttributes.rxDiscoveryPulseCounter.east++;
            }
        }
    }
    ParticleAttributes.rxInterruptFlankStates.east = isRxSignalLow;
}


#if defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)
/**
 * On timer_counter match with compare register A.
 */
ISR(TIMER1_COMPA_vect) {
    TIMER0_NEIGHBOUR_SENSE_DISABLE;
    TCNT0 = 0;

    NORTH_TX_TOGGLE;
    SOUTH_TX_TOGGLE;
    EAST_TX_TOGGLE;

    TIMER0_NEIGHBOUR_SENSE_ENABLE;
}


/**
 * On timer_counter overflow.
 */
ISR(TIMER1_OVF_vect) {
    asm("BREAK");
}

/**
 * On timer_counter input capture.
 */
ISR(TIMER1_CAPT_vect) {
    asm("BREAK");
}


/**
 * On timer_counter compare register B match.
 */
ISR(TIMER1_COMPB_vect) {
    asm("BREAK");
}


/**
 * On external pin, power-on reset, brown-out reset, watchdog reset.
 */
ISR(_VECTOR(0)) {
    asm("BREAK");
}

#endif

#endif
