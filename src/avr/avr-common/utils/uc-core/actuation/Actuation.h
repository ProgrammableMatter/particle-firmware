/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once

#include "ActuationTypes.h"
#include "uc-core/particle/Globals.h"


extern FUNC_ATTRS void handleExecuteActuation(void (actuationDoneCallback)(void));
/**
 * handles actuation command states
 */
FUNC_ATTRS void handleExecuteActuation(void (actuationDoneCallback)(void)) {
    volatile ActuationCommand *actuationCommand = &ParticleAttributes.actuationCommand;
    switch (actuationCommand->executionState) {
        case ACTUATION_STATE_TYPE_IDLE:
            if (actuationCommand->isScheduled) {
                actuationCommand->isScheduled = false;
                actuationCommand->executionState = ACTUATION_STATE_TYPE_START;
            } else {
                return;
            }
            break;

        case ACTUATION_STATE_TYPE_START:
//            DEBUG_CHAR_OUT('y');
            // enable pwm interrupt
            actuationCommand->executionState = ACTUATION_STATE_TYPE_WORKING;
            break;

        case ACTUATION_STATE_TYPE_WORKING:
            if (ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp <
                ParticleAttributes.localTime.numTimePeriodsPassed) {
                actuationCommand->executionState = ACTUATION_STATE_TYPE_DONE;
            }
            return;
            break;


        case ACTUATION_STATE_TYPE_DONE:
            // disable pwm interrupt
            ParticleAttributes.actuationCommand.isScheduled = false;
            ParticleAttributes.actuationCommand.actuators.northLeft = false;
            ParticleAttributes.actuationCommand.actuators.northRight = false;
            ParticleAttributes.actuationCommand.actuators.eastLeft = false;
            ParticleAttributes.actuationCommand.actuators.eastRight = false;
            ParticleAttributes.actuationCommand.actuators.southLeft = false;
            ParticleAttributes.actuationCommand.actuators.southRight = false;
            actuationCommand->executionState = ACTUATION_STATE_TYPE_IDLE;
            actuationDoneCallback();
//            DEBUG_CHAR_OUT('Y');
            break;
    }
};