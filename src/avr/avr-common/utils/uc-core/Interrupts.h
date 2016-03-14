/**
 * @author Raoul Rubien 2015
 */
#ifndef DEFAULT_ISR_H
#define DEFAULT_ISR_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Globals.h"
#include "IoDefinitions.h"
#include "InterruptDefinitions.h"
#include "simulation/SimulationUtils.h"

extern volatile ParticleState ParticleAttributes;

/**
 * north RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__

ISR(PCINT2_vect)
#else
#  if defined(__AVR_ATmega16__)

ISR(INT0_vect)
#  else
#    error
#  endif
#endif
{
    TIMER_NEIGHBOUR_SENSE_DISABLE;
    if (SOUTH_RX_IS_LO) {
        switch (ParticleAttributes.state) {
            case STATE_TYPE_NEIGHBOURS_DISCOVERY:
                if (ParticleAttributes.rxDiscoveryPulseCounters.south < RX_PULSE_COUNTER_MAX) {
                    ParticleAttributes.rxDiscoveryPulseCounters.south++;
                }
                break;

            default:
                break;
        }
    }
    TIMER_NEIGHBOUR_SENSE_ENABLE;
}


/**
 * south RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__
ISR(PCINT0_vect)
#else
#  if defined(__AVR_ATmega16__)

ISR(INT1_vect)
#  else
#    error
#  endif
#endif
{
    TIMER_NEIGHBOUR_SENSE_DISABLE;
    if (EAST_RX_IS_LO) {
        switch (ParticleAttributes.state) {
            case STATE_TYPE_NEIGHBOURS_DISCOVERY:
                if (ParticleAttributes.rxDiscoveryPulseCounters.east < RX_PULSE_COUNTER_MAX) {
                    ParticleAttributes.rxDiscoveryPulseCounters.east++;
                }
                break;

            default:
                break;
        }
    }
    TIMER_NEIGHBOUR_SENSE_ENABLE;
}


/**
 * east RX pin change interrupt on logical pin change
 */
#ifdef __AVR_ATtiny1634__

ISR(PCINT1_vect)
#else
#  if defined(__AVR_ATmega16__)

ISR(INT2_vect)
#  else
#    error
#  endif
#endif
{
    TIMER_NEIGHBOUR_SENSE_DISABLE;
    if (NORTH_RX_IS_LO) {
        switch (ParticleAttributes.state) {
            case STATE_TYPE_NEIGHBOURS_DISCOVERY:
                if (ParticleAttributes.rxDiscoveryPulseCounters.north < RX_PULSE_COUNTER_MAX) {
                    ParticleAttributes.rxDiscoveryPulseCounters.north++;
                }
                break;

            default:
                break;
        }
    }
    TIMER_NEIGHBOUR_SENSE_ENABLE;
}


#if defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)
/**
 * On timer_counter match with compare register A.
 */
ISR(TIMER1_COMPA_vect) {
    TIMER_NEIGHBOUR_SENSE_DISABLE;
    TIMER_NEIGHBOUR_SENSE_COUNTER = 0;

    NORTH_TX_TOGGLE;
    SOUTH_TX_TOGGLE;
    EAST_TX_TOGGLE;

    TIMER_NEIGHBOUR_SENSE_ENABLE;
}

#endif

#if defined(__AVR_ATmega16__)

const char isrTimer1OvfMsg[] PROGMEM = "ISR(TIMER1_CAPT_vect)";
/**
 * On timer_counter overflow.
 */
ISR(TIMER1_OVF_vect) {
    writeToUart((PGM_P) pgm_read_word(&(isrTimer1OvfMsg)));
}

const char isrTimer1CaptMsg[] PROGMEM = "ISR(TIMER1_CAPT_vect)";
/**
 * On timer_counter input capture.
 */
ISR(TIMER1_CAPT_vect) {
    writeToUart((PGM_P) pgm_read_word(&(isrTimer1CaptMsg)));
}

const char isrTimer1CompBMsg[] PROGMEM = "ISR(TIMER1_COMPB_vect)";
/**
 * On timer_counter compare register B match.
 */
ISR(TIMER1_COMPB_vect) {
    writeToUart((PGM_P) pgm_read_word(&(isrTimer1CompBMsg)));
}


const char isrVector0Msg[] PROGMEM = "ISR(_VECTOR(0))";
/**
 * On external pin, power-on reset, brown-out reset, watchdog reset.
 */
ISR(_VECTOR(0)) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
}

#endif

#endif
