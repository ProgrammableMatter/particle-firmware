/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once

#include "ActuationTypes.h"

extern CTOR_ATTRS void constructActuators(volatile Actuators *o);
/**
 * constructor function
 */
CTOR_ATTRS void constructActuators(volatile Actuators *o) {
    o->northLeft = false;
    o->northRight = false;
    o->eastLeft = false;
    o->eastRight = false;
}

extern CTOR_ATTRS void constructHeatingMode(volatile HeatingMode *o);

/**
 * constructor function
 */
CTOR_ATTRS void constructHeatingMode(volatile HeatingMode *o) {
    o->dutyCycleLevel = HEATING_LEVEL_TYPE_MAXIMUM;
}

extern CTOR_ATTRS void constructActuationCommand(volatile struct ActuationCommand *o);

/**
 * constructor function
 */
CTOR_ATTRS void constructActuationCommand(volatile struct ActuationCommand *o) {
    constructActuators(&o->actuators);
    constructHeatingMode(&o->actuationPower);
    o->isScheduled = false;
    o->isToBeTerminated = true;
    o->executionState = ACTUATION_STATE_TYPE_IDLE;
}
