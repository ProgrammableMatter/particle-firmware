/**
 * @author Raoul Rubien 2015
 */
#ifndef DEFAULT_ISR_H
#define DEFAULT_ISR_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <uc-core/ParticleTypes.h>
#include <uc-core/communication/CommunicationTypes.h>

#include "Globals.h"
#include "IoDefinitions.h"
#include "Interrupts.h"
#include "simulation/SimulationUtils.h"
#include "discovery/Discovery.c"
#include "communication/Communication.c"
#include "simulation/SimulationMacros.h"

extern volatile ParticleState ParticleAttributes;


/**
 * south RX pin change interrupt on logical pin change
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
    switch (ParticleAttributes.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (SOUTH_RX_IS_LO) {
                TIMER_NEIGHBOUR_SENSE_DISABLE;
                dispatchFallingDiscoveryFlank(&ParticleAttributes.rxDiscoveryPulseCounters.south);
                TIMER_NEIGHBOUR_SENSE_ENABLE;
            }
            break;

            // on data received
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            dispatchReceivedDataFlank(&ParticleAttributes.rxBitBuffer.south,
                                      &ParticleAttributes.rxCounterArguments.south, SOUTH_RX_IS_LO);
            break;

        default:
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
            break;
    }
}


/**
 * east RX pin change interrupt on logical pin change
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
    switch (ParticleAttributes.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (EAST_RX_IS_LO) {
                TIMER_NEIGHBOUR_SENSE_DISABLE;
                dispatchFallingDiscoveryFlank(&ParticleAttributes.rxDiscoveryPulseCounters.east);
                TIMER_NEIGHBOUR_SENSE_ENABLE;
            }
            break;

            // on data received
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            dispatchReceivedDataFlank(
                    &ParticleAttributes.rxBitBuffer.east,
                    &ParticleAttributes.rxCounterArguments.east, EAST_RX_IS_LO);
            break;
        default:
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
            break;
    }
}


/**
 * north RX pin change interrupt on logical pin change
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
    switch (ParticleAttributes.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (NORTH_RX_IS_LO) {
                TIMER_NEIGHBOUR_SENSE_DISABLE;
                dispatchFallingDiscoveryFlank(&ParticleAttributes.rxDiscoveryPulseCounters.north);
                TIMER_NEIGHBOUR_SENSE_ENABLE;
            }
            break;

            // on data received
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            dispatchReceivedDataFlank(
                    &ParticleAttributes.rxBitBuffer.north, &ParticleAttributes.rxCounterArguments.north,
                    NORTH_RX_IS_LO);
            break;

        default:
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
            break;
    }
}


/**
 * On timer 0 compare interrupt.
 */
#ifdef __AVR_ATtiny1634__

ISR(TIM0_COMPA_vect)
#else
#  if defined(__AVR_ATmega16__)
ISR(TIMER0_COMP_vect)
#  else
#    error
#  endif
#endif
{
}


#if defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)
/**
 * On timer_counter match with compare register A.
 */
ISR(TIMER1_COMPA_vect) {
    switch (ParticleAttributes.state) {
        // on generate discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
        case STATE_TYPE_DISCOVERY_PULSING:
        TIMER_NEIGHBOUR_SENSE_DISABLE;
            TIMER_NEIGHBOUR_SENSE_COUNTER = 0;

            NORTH_TX_TOGGLE;
            SOUTH_TX_TOGGLE;
            EAST_TX_TOGGLE;

            TIMER_NEIGHBOUR_SENSE_ENABLE;
            break;

            // on receive data counter compare
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            TCNT1 = 0;
            break;

        default:
            break;
    }
}

/**
 * On timer_counter compare register B match.
 */
ISR(TIMER1_COMPB_vect) {
    switch (ParticleAttributes.state) {
        // on receive data counter compare
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            if (ParticleAttributes.rxCounterArguments.north.isReceiving != 0) {
                --ParticleAttributes.rxCounterArguments.north.isReceiving;
            }
            if (ParticleAttributes.rxCounterArguments.east.isReceiving != 0) {
                --ParticleAttributes.rxCounterArguments.east.isReceiving;
            }
            if (ParticleAttributes.rxCounterArguments.south.isReceiving != 0) {
                --ParticleAttributes.rxCounterArguments.south.isReceiving;
            }
            break;
        default:
            break;
    }
}

#endif

#ifdef IS_SIMULATION

const char isrVector0Msg[] PROGMEM = "ISR(_VECTOR(0))";
/**
 * On external pin, power-on reset, brown-out reset, watchdog reset.
 */
ISR(_VECTOR(0)) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
}

#endif

#endif
