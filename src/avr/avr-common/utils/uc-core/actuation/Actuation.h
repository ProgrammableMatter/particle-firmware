/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once

#include "ActuationTypes.h"
#include "uc-core/particle/Globals.h"

/**
 * prepares affected wires and enables the actuation interrupt
 */
extern FUNC_ATTRS void __enableActuationInterrupt(void);

FUNC_ATTRS void __enableActuationInterrupt(void) {

    // setup wires, disable reception interrupt,
    if (ParticleAttributes.actuationCommand.actuators.northLeft) {
        // on north left wire actuation
        NORTH_TX_LO; // do not drive MOSFET
    }
    if (ParticleAttributes.actuationCommand.actuators.northRight) {
        // on north right wire actuation
        RX_NORTH_INTERRUPT_DISABLE; // do not feed reception with actuation PWM
        // do not drain current when actuating
        // NORTH_RX_PULL_UP; // already pulled up
        NORTH_RX_SWITCH_HI; // do not drive MOSFET
    }

    // set up PWM duty cycle according to actuation command setting
    switch (ParticleAttributes.actuationCommand.actuationPower.dutyCycleLevel) {
        case HEATING_LEVEL_TYPE_MAXIMUM:
            ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_MAXIMUM;
            break;
        case HEATING_LEVEL_TYPE_STRONG:
            ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_STRONG;
            break;
        case HEATING_LEVEL_TYPE_MEDIUM:
            ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_MEDIUM;
            break;
        case HEATING_LEVEL_TYPE_WEAK:
        default:
            ACTUATOR_INTERRUPT_SET_PWM_DUTY_CYCLE_WEAK;
            break;
    }

    // enable PWM interrupt
    ACTUATOR_INTERRUPT_ENABLE;
}


/**
 * disables actuation interrupt
 */
extern FUNC_ATTRS void __disableActuationInterrupt(void);

FUNC_ATTRS void __disableActuationInterrupt(void) {
    ACTUATOR_INTERRUPT_DISABLE;
    ACTUATOR_INTERRUPT_RESET_STATE;
}

/**
 * put wires back in charge: set wires to default state,
 * enable reception interrupt,
 * clear actuation command flags
 */
extern FUNC_ATTRS void __reEnableWiresAfterActuationInterrupt(void);

FUNC_ATTRS void __reEnableWiresAfterActuationInterrupt(void) {

    if (ParticleAttributes.actuationCommand.actuators.northLeft) {
        NORTH_TX_LO; // return tx wire to default state
    }
    if (ParticleAttributes.actuationCommand.actuators.northRight) {
        // NORTH_RX_PULL_UP; // already pulled up
        NORTH_RX_SWITCH_HI; // do not drive MOSFET, let rx signal pass through
        RX_NORTH_INTERRUPT_ENABLE;
    }
    if (ParticleAttributes.actuationCommand.actuators.eastLeft) {
        EAST_TX_LO; // return tx wire to default state
    }
    if (ParticleAttributes.actuationCommand.actuators.eastRight) {
        // EAST_RX_PULL_UP; // already pulled up
        EAST_RX_SWITCH_HI;
        RX_EAST_INTERRUPT_ENABLE;
    }
    if (ParticleAttributes.actuationCommand.actuators.southLeft) {
        SOUTH_TX_LO; // return tx wire to default state
    }
    if (ParticleAttributes.actuationCommand.actuators.southRight) {
        // SOUTH_RX_PULL_UP; // already pulled up
        SOUTH_RX_SWITCH_HI;
        RX_SOUTH_INTERRUPT_ENABLE;
    }

    *((uint8_t *) &ParticleAttributes.actuationCommand.actuators) = 0;
    ParticleAttributes.actuationCommand.isScheduled = false;

    RX_INTERRUPTS_ENABLE;
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
            __enableActuationInterrupt();
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
            __disableActuationInterrupt();
            // wait until probably bouncing signal is finished
            DELAY_US_15;
            actuationCommand->executionState = ACTUATION_STATE_TYPE_DONE;
            goto __ACTUATION_STATE_TYPE_DONE;
            break;

        __ACTUATION_STATE_TYPE_DONE:
        case ACTUATION_STATE_TYPE_DONE:
            __reEnableWiresAfterActuationInterrupt();
            actuationCommand->executionState = ACTUATION_STATE_TYPE_IDLE;
            actuationDoneCallback();
//            DEBUG_CHAR_OUT('Y');
            break;
    }
};