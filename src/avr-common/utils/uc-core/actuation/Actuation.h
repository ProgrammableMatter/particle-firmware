/**
 * @author Raoul Rubien  12.07.2016
 *
 * Actuation related implementation.
 */

#pragma once

#include "ActuationTypes.h"
#include "uc-core/particle/Globals.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/configuration/interrupts/ReceptionPCI.h"
#include "uc-core/configuration/interrupts/ActuationTimer.h"
#include "uc-core/delay/delay.h"
#include "uc-core/communication/ManchesterDecodingTypesCtors.h"

/**
 * Prepares affected wires and enables the actuation interrupt (PWM),
 * otherwise, on maximum power output, configures the corresponding wires.
 */
extern FUNC_ATTRS void __startActuation(void);

FUNC_ATTRS void __startActuation(void) {

    // active actuation - north left
    if (ParticleAttributes.actuationCommand.actuators.northLeft) {
        NORTH_TX_LO; // do not drive MOSFET
    }
    // active actuation - north right
    if (ParticleAttributes.actuationCommand.actuators.northRight) {
        RX_NORTH_INTERRUPT_DISABLE;
        NORTH_RX_SWITCH_HI; // do not drive MOSFET
    }
    // passive actuation - east left
    if (ParticleAttributes.actuationCommand.actuators.eastLeft) {
        EAST_TX_HI; // drive MOSFET
    }
    // passive actuation - east right
    if (ParticleAttributes.actuationCommand.actuators.eastRight) {
        RX_EAST_INTERRUPT_DISABLE;
        EAST_RX_SWITCH_LO; // drive MOSFET
    }
    // passive actuation - south left
    if (ParticleAttributes.actuationCommand.actuators.southLeft) {
        SOUTH_TX_HI; // drive MOSFET
    }
    // passive actuation - south right
    if (ParticleAttributes.actuationCommand.actuators.southRight) {
        RX_SOUTH_INTERRUPT_DISABLE;
        SOUTH_RX_SWITCH_LO; // drive MOSFET
    }

    // set up PWM duty cycle according to actuation command setting
    switch (ParticleAttributes.actuationCommand.actuationPower.dutyCycleLevel) {
        case HEATING_LEVEL_TYPE_MAXIMUM:

            if (ParticleAttributes.actuationCommand.actuators.northLeft) {
                NORTH_TX_HI; // drive MOSFET
            }
            if (ParticleAttributes.actuationCommand.actuators.northRight) {
                RX_NORTH_INTERRUPT_DISABLE;
                NORTH_RX_SWITCH_LO; // drive MOSFET
            }

            break;
        case HEATING_LEVEL_TYPE_STRONG:
            ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_STRONG;
            ACTUATOR_INTERRUPT_ENABLE;
            break;
        case HEATING_LEVEL_TYPE_MEDIUM:
            ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_MEDIUM;
            ACTUATOR_INTERRUPT_ENABLE;
            break;
        case HEATING_LEVEL_TYPE_WEAK:
        default:
            ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_WEAK;
            ACTUATOR_INTERRUPT_ENABLE;
            break;
    }
}


/**
 * disables the actuation interrupt
 */
extern FUNC_ATTRS void __stopActuationPWM(void);

FUNC_ATTRS void __stopActuationPWM(void) {
    ACTUATOR_INTERRUPT_DISABLE;
}

/**
 * return wires of affected ports to their default working states (reception state)
 */
extern FUNC_ATTRS void __setDefaultWiresStates(void);

FUNC_ATTRS void __setDefaultWiresStates(void) {

    if (ParticleAttributes.actuationCommand.actuators.northLeft) {
        NORTH_TX_LO;
    }
    if (ParticleAttributes.actuationCommand.actuators.northRight) {
        NORTH_RX_SWITCH_HI;
    }
    if (ParticleAttributes.actuationCommand.actuators.eastLeft) {
        EAST_TX_LO;
    }
    if (ParticleAttributes.actuationCommand.actuators.eastRight) {
        EAST_RX_SWITCH_HI;
    }
    if (ParticleAttributes.actuationCommand.actuators.southLeft) {
        SOUTH_TX_LO;
    }
    if (ParticleAttributes.actuationCommand.actuators.southRight) {
        SOUTH_RX_SWITCH_HI;
    }
}

/**
 * truncates snapshot buffers of affected ports
 */
extern FUNC_ATTRS void __truncateReceptionBuffers(void);

FUNC_ATTRS void __truncateReceptionBuffers(void) {

    if (ParticleAttributes.actuationCommand.actuators.northRight) {
        constructRxSnapshotBuffer(&ParticleAttributes.directionOrientedPorts.north.rxPort->snapshotsBuffer);
    }
    if (ParticleAttributes.actuationCommand.actuators.eastRight) {
        constructRxSnapshotBuffer(&ParticleAttributes.directionOrientedPorts.east.rxPort->snapshotsBuffer);
    }
    if (ParticleAttributes.actuationCommand.actuators.southRight) {
        constructRxSnapshotBuffer(&ParticleAttributes.directionOrientedPorts.south.rxPort->snapshotsBuffer);
    }
}

/**
 * clears pending interrupts and enables reception interrupts of affected ports
 */
extern FUNC_ATTRS void __enableReceptionInterrupts(void);

FUNC_ATTRS void __enableReceptionInterrupts(void) {

    if (ParticleAttributes.actuationCommand.actuators.northRight) {
        RX_NORTH_INTERRUPT_CLEAR_PENDING;
        RX_NORTH_INTERRUPT_ENABLE;
    }
    if (ParticleAttributes.actuationCommand.actuators.eastRight) {
        RX_EAST_INTERRUPT_CLEAR_PENDING;
        RX_EAST_INTERRUPT_ENABLE;
    }
    if (ParticleAttributes.actuationCommand.actuators.southRight) {
        RX_SOUTH_INTERRUPT_CLEAR_PENDING;
        RX_SOUTH_INTERRUPT_ENABLE;
    }
}

/**
 * handles actuation command states
 */
extern FUNC_ATTRS void handleExecuteActuation(void (actuationDoneCallback)(void));

FUNC_ATTRS void handleExecuteActuation(void (actuationDoneCallback)(void)) {
    volatile ActuationCommand *actuationCommand = &ParticleAttributes.actuationCommand;
    switch (actuationCommand->executionState) {
        case ACTUATION_STATE_TYPE_IDLE:
            if (actuationCommand->isScheduled) {
                actuationCommand->isScheduled = false;
                actuationCommand->executionState = ACTUATION_STATE_TYPE_START;
                goto __ACTUATION_STATE_TYPE_START;
            } else {
                // in case of race; actuator interrupt vs main loop
                actuationCommand->executionState = ACTUATION_STATE_TYPE_DONE;
                goto __ACTUATION_STATE_TYPE_DONE;
                return;
            }
            break;

        __ACTUATION_STATE_TYPE_START:
        case ACTUATION_STATE_TYPE_START:
//            DEBUG_CHAR_OUT('y');
            __startActuation();
            actuationCommand->executionState = ACTUATION_STATE_TYPE_WORKING;
            goto __ACTUATION_STATE_TYPE_WORKING;
            break;

        __ACTUATION_STATE_TYPE_WORKING:
        case ACTUATION_STATE_TYPE_WORKING:
            if (ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp <
                ParticleAttributes.localTime.numTimePeriodsPassed) {
                actuationCommand->executionState = ACTUATION_STATE_TYPE_RELAXATION_PAUSE;
                goto __ACTUATION_STATE_TYPE_RELAXATION_PAUSE;
            }
            return;
            break;

        __ACTUATION_STATE_TYPE_RELAXATION_PAUSE:
        case ACTUATION_STATE_TYPE_RELAXATION_PAUSE:
            __stopActuationPWM();
            __setDefaultWiresStates();
            __truncateReceptionBuffers();
            // let bouncing signals pass
            DELAY_US_150;
            actuationCommand->executionState = ACTUATION_STATE_TYPE_DONE;
            goto __ACTUATION_STATE_TYPE_DONE;
            break;

        __ACTUATION_STATE_TYPE_DONE:
        case ACTUATION_STATE_TYPE_DONE:
            __enableReceptionInterrupts();
            actuationCommand->executionState = ACTUATION_STATE_TYPE_IDLE;
            actuationDoneCallback();
            *((uint8_t *) &ParticleAttributes.actuationCommand.actuators) = 0;
//            DEBUG_CHAR_OUT('Y');
            break;
    }
};