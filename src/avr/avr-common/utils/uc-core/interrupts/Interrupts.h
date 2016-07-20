/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include "uc-core/particle/Globals.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/configuration/interrupts/Vectors.h"
#include "uc-core/configuration/interrupts/TxRxTimer.h"
#include "uc-core/configuration/interrupts/LocalTime.h"
#include "uc-core/configuration/interrupts/Reception.h"
#include "uc-core/discovery/Discovery.h"
#include "uc-core/communication/Transmission.h"
#include "uc-core/communication/ManchesterCoding.h"
#include "uc-core/communication/ManchesterDecoding.h"
#include "uc-core/time/Time.h"

#ifdef SIMULATION

#  include <avr/pgmspace.h>
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
            // on data received
            captureSnapshot(&timerCounterValue, isRxHigh, &port->rxPort->snapshotsBuffer);
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
 * interrupt routine on north RX pin change interrupt on logical pin change
 * simulator int. #19
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
 * interrupt routine on east RX pin change interrupt on logical pin change
 * simulator int. #3
 */
ISR(EAST_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.directionOrientedPorts.east,
                           EAST_RX_IS_HI);
    RX_EAST_INTERRUPT_CLEAR_PENDING;
}

/**
 * south RX pin change interrupt on logical pin change
 * simulator int. #2
 */
ISR(SOUTH_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.directionOrientedPorts.south,
                           SOUTH_RX_IS_HI);
    RX_SOUTH_INTERRUPT_CLEAR_PENDING;
}

/**
 * on transmission/discovery timer compare match interrupt
 * simulator int. #7
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
            if (ParticleAttributes.protocol.isSimultaneousTransmissionEnabled) {
                transmit(&ParticleAttributes.directionOrientedPorts.simultaneous);
            } else {
                transmit(&ParticleAttributes.directionOrientedPorts.north);
                transmit(&ParticleAttributes.directionOrientedPorts.east);
                transmit(&ParticleAttributes.directionOrientedPorts.south);
            }
            scheduleNextTransmission();
            break;
    }
}

/**
 * on local time period passed interrupt
 * simulator int. #8
 */
ISR(LOCAL_TIME_INTERRUPT_VECT) {
    ADVANCE_LOCAL_TIME;
    SCHEDULE_NEXT_LOCAL_TIME_INTERUPT;
}

EMPTY_INTERRUPT(__UNUSED_TIMER1_OVERFLOW_INTERRUPT_VECT)

/**
 * actuator pwm interrupt routine: on actuator pwm compare match interrupt
 * simulator int. #20
 */
ISR(ACTUATOR_PWM_INTERRUPT_VECT) {
    if (ParticleAttributes.actuationCommand.actuators.northLeft) {
        // on actuate north tx wire
        // @pre deactivated: NORTH_TX_LO;
        NORTH_TX_TOGGLE;
    }
    if (ParticleAttributes.actuationCommand.actuators.northRight) {
        // on actuate north rx wire
        // @pre deactivated: == NORTH_RX_SWITCH_HI;
        NORTH_RX_SWITCH_TOGGLE;
    }
}

/**
 * on timer 0 overflow
 * simulator int. #10
 */
EMPTY_INTERRUPT(__UNUSED_TIMER0_OVERFLOW_INTERRUPT_VECT)

# ifdef SIMULATION
const char isrVector0Msg[] PROGMEM = "BAD ISR";

ISR(RESET_VECT) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}

ISR(BADISR_vect) {
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}
#  else
EMPTY_INTERRUPT(RESET_VECT)
EMPTY_INTERRUPT(BADISR_vect)
#  endif

