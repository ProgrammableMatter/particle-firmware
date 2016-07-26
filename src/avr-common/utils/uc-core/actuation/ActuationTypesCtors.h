/**
 * @author Raoul Rubien  12.07.2016
 *
 * Actuation types constructor implementation.
 */

#pragma once

#include "ActuationTypes.h"

extern CTOR_ATTRS void constructActuators(volatile Actuators *o);
/**
 * constructor function
 * @param o reference to the object to construct
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
 * @param o reference to the object to construct
 */
CTOR_ATTRS void constructHeatingMode(volatile HeatingMode *o) {
    o->dutyCycleLevel = HEATING_LEVEL_TYPE_MEDIUM;
}

extern FUNC_ATTRS void constructLocalTime(volatile LocalTime *o);
/**
 * constructor function
 * @param o reference to the object to construct
 */
FUNC_ATTRS void constructLocalTime(volatile LocalTime *o) {
    o->periodTimeStamp = 0;
};

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructActuationCommand(volatile struct ActuationCommand *o);

CTOR_ATTRS void constructActuationCommand(volatile struct ActuationCommand *o) {
    constructActuators(&o->actuators);
    constructHeatingMode(&o->actuationPower);
    constructLocalTime(&o->actuationStart);
    constructLocalTime(&o->actuationEnd);
    o->isScheduled = false;
    o->executionState = ACTUATION_STATE_TYPE_IDLE;
}
