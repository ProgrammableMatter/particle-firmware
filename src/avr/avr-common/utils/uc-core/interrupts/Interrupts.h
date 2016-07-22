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

/**
 * Handles input pins interrupts according to the particle state.
 * @param port the designated port
 * @param isRxHigh the logic signal level
 */
extern FUNC_ATTRS void __handleInputInterrupt(volatile DirectionOrientedPort *port,
                                              const bool isRxHigh);

FUNC_ATTRS void __handleInputInterrupt(volatile DirectionOrientedPort *port,
                                       const bool isRxHigh) {
    uint16_t timerCounterValue = TIMER_TX_RX_COUNTER_VALUE;
    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            // on discovery pulse
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

/**
 * Schedules the next transmission interrupt if transmission data is buffered.
 */
extern FUNC_ATTRS void scheduleNextTransmission(void);

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
 * Interrupt routine on logical north pin change (reception).
 * simulator int. #19
 */
ISR(NORTH_PIN_CHANGE_INTERRUPT_VECT) {
    // DEBUG_INT16_OUT(TIMER_TX_RX_COUNTER_VALUE);
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
 * Interrupt routine on logical east pin change (reception).
 * simulator int. #3
 */
ISR(EAST_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.directionOrientedPorts.east,
                           EAST_RX_IS_HI);
    RX_EAST_INTERRUPT_CLEAR_PENDING;
}

/**
 * Interrupt routine on logical south pin change (reception).
 * simulator int. #2
 */
ISR(SOUTH_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.directionOrientedPorts.south,
                           SOUTH_RX_IS_HI);
    RX_SOUTH_INTERRUPT_CLEAR_PENDING;
}

/**
 * On transmission/discovery timer compare match.
 * simulator int. #7
 */
ISR(TX_TIMER_INTERRUPT_VECT) {

    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_START:
        case STATE_TYPE_ACTIVE:
            break;

        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
        case STATE_TYPE_DISCOVERY_PULSING:
            // on generate discovery pulse
            NORTH_TX_TOGGLE;
            SOUTH_TX_TOGGLE;
            EAST_TX_TOGGLE;
            break;

        default:
            // otherwise process transmission
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
 * On local time period passed interrupt.
 * simulator int. #8
 */
ISR(LOCAL_TIME_INTERRUPT_VECT) {
    ADVANCE_LOCAL_TIME;
    SCHEDULE_NEXT_LOCAL_TIME_INTERUPT;
}


/**
 * Actuator PWM interrupt routine: On compare match toggle wires.
 * simulator int. #20
 */
ISR(ACTUATOR_PWM_INTERRUPT_VECT) {
    if (ParticleAttributes.actuationCommand.actuators.northLeft) {
        // on actuate north transmissoin wire
        // @pre deactivated: NORTH_TX_LO;
        NORTH_TX_TOGGLE;
    }
    if (ParticleAttributes.actuationCommand.actuators.northRight) {
        // on actuate north recepton wire
        // @pre deactivated: NORTH_RX_SWITCH_HI;
        NORTH_RX_SWITCH_TOGGLE;
    }
}

EMPTY_INTERRUPT(__UNUSED_TIMER0_OVERFLOW_INTERRUPT_VECT)

EMPTY_INTERRUPT(__UNUSED_TIMER1_OVERFLOW_INTERRUPT_VECT)

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

