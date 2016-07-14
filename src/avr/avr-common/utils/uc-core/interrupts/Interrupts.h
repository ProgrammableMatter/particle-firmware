/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <uc-core/particle/ParticleStateTypes.h>

#include "uc-core/configuration/interrupts/Vectors.h"
#include "uc-core/configuration/interrupts/TimerCounter.h"
#include "uc-core/configuration/interrupts/Reception.h"
#include "uc-core/communication/Transmission.h"
#include "uc-core/particle/Globals.h"
#include "uc-core/discovery/Discovery.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/communication/ManchesterDecoding.h"
#include "uc-core/communication/ManchesterCoding.h"

#ifdef SIMULATION

#  include "simulation/SimulationMacros.h"

#endif

#define TIMER_NEIGHBOUR_SENSE_COUNTER_ON_INTERRUPT_ROLLBACK 12

extern FUNC_ATTRS void __handleInputInterrupt(volatile DirectionOrientedPort *port,
                                              const bool isRxHigh);
/**
 * Handles interrupt in input pins according to the particle state.
 */
FUNC_ATTRS void __handleInputInterrupt(volatile DirectionOrientedPort *port,
                                       const bool isRxHigh) {
    uint16_t timerCounterValue = TIMER_TX_RX_COUNTER_VALUE;
    switch (ParticleAttributes.node.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (!isRxHigh) {
                dispatchFallingDiscoveryEdge(port->discoveryPulseCounter);
            }
            break;

        default:
            switch (ParticleAttributes.communication.xmissionState) {
                // on data received
                case STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX:
                case STATE_TYPE_XMISSION_TYPE_ENABLED_RX:
                    captureSnapshot(&timerCounterValue, isRxHigh, &port->rxPort->snapshotsBuffer);
                    break;

                default:
                    break;
            }
            break;
    }
}

extern FUNC_ATTRS void scheduleNextTransmission(void);
/**
 * schedules the next transmission interrupt on available data to be sent.
 */
FUNC_ATTRS void scheduleNextTransmission(void) {
    if (ParticleAttributes.communication.ports.tx.north.isTransmitting ||
        ParticleAttributes.communication.ports.tx.east.isTransmitting ||
        ParticleAttributes.communication.ports.tx.south.isTransmitting) {
        scheduleNextTxInterrupt();
    } else {
        TIMER_TX_RX_DISABLE_COMPARE_INTERRUPT;
    }
}


/**
 * north RX pin change interrupt on logical pin change
 * int. #19
 */
ISR(NORTH_PIN_CHANGE_INTERRUPT_VECT) {
//    DEBUG_INT16_OUT(TIMER_TX_RX_COUNTER_VALUE);
    if (ParticleAttributes.protocol.isBroadcastEnabled) {
        if (NORTH_RX_IS_HI) {
            simultaneousTxLoImpl();
        } else {
            simultaneousTxHiImpl();
        }
    }
    __handleInputInterrupt(&ParticleAttributes.directionOrientedPorts.north,
                           NORTH_RX_IS_HI);
    RX_NORTH_INTERRUPT_CLEAR_PENDING;
}

/**
 * east RX pin change interrupt on logical pin change
 * int. #3
 */
ISR(EAST_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.directionOrientedPorts.east,
                           EAST_RX_IS_HI);
    RX_EAST_INTERRUPT_CLEAR_PENDING;
}

/**
 * south RX pin change interrupt on logical pin change
 * int. #2
 */
ISR(SOUTH_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.directionOrientedPorts.south,
                           SOUTH_RX_IS_HI);
    RX_SOUTH_INTERRUPT_CLEAR_PENDING;
}

/**
 * int. #7
 */
ISR(TX_TIMER_INTERRUPT_VECT) {

    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_START:
        case STATE_TYPE_ACTIVE:
            break;

            // on generate discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
        case STATE_TYPE_DISCOVERY_PULSING:
            NORTH_TX_TOGGLE;
            SOUTH_TX_TOGGLE;
            EAST_TX_TOGGLE;
            break;

            // otherwise process transmission
        default:
            switch (ParticleAttributes.communication.xmissionState) {
                case STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX:
                case STATE_TYPE_XMISSION_TYPE_ENABLED_TX:
                    if (ParticleAttributes.protocol.isSimultaneousTransmissionEnabled) {
                        transmit(&ParticleAttributes.directionOrientedPorts.simultaneous);
                    } else {
                        transmit(&ParticleAttributes.directionOrientedPorts.north);
                        transmit(&ParticleAttributes.directionOrientedPorts.east);
                        transmit(&ParticleAttributes.directionOrientedPorts.south);
                    }
                    scheduleNextTransmission();
                    break;

                default:
                    break;
            }
            break;
    }
}

#include "uc-core/time/Time.h"

/**
 * On local time period passed interrupt.
 * int. #8
 */
ISR(LOCAL_TIME_INTERRUPT_VECT) {
    advanceLocalTime();
    scheduleNextLocalTimeInterrupt();

    if (ParticleAttributes.actuationCommand.isScheduled) {
        if (ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp <=
            ParticleAttributes.localTime.numTimePeriodsPassed) {
            ParticleAttributes.node.state = STATE_TYPE_EXECUTE_ACTUATION_COMMAND;
        }
    }
}

EMPTY_INTERRUPT(TIMER1_OVERFLOW_INTERRUPT_VECT)
//{
//    EAST_TX_TOGGLE;
//    SOUTH_TX_TOGGLE;
//}

# ifdef SIMULATION

const char isrVector0Msg[] PROGMEM = "BAD ISR";
/**
 * On external pin, power-on reset, brown-out reset, watchdog reset.
 */
ISR(_VECTOR(0)) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}

/**
 * on timer 0 compare
 * int #19
 */
ISR(__UNUSED_TIMER0_COMPARE_INTERRUPT_VECT) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}

/**
 * on timer 0 overflow
 * int #9
 */
ISR(__UNUSED_TIMER0_OVERFLOW_INTERRUPT_VECT) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}

ISR(BADISR_vect) {
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}

#  else

EMPTY_INTERRUPT(_VECTOR(0))

EMPTY_INTERRUPT(__UNUSED_TIMER0_COMPARE_INTERRUPT_VECT)

EMPTY_INTERRUPT(__UNUSED_TIMER0_OVERFLOW_INTERRUPT_VECT)

EMPTY_INTERRUPT(BADISR_vect)

#  endif

