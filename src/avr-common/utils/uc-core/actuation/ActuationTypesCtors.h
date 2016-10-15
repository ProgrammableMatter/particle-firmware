/**
 * @author Raoul Rubien  12.07.2016
 *
 * Actuation types constructor implementation.
 */

#pragma once

#include "ActuationTypes.h"

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructActuators(Actuators *const o) {
    o->northLeft = false;
    o->northRight = false;
    o->eastLeft = false;
    o->eastRight = false;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructHeatingMode(HeatingMode *const o) {
    // TODO: pwm driven pwer levels seems not to work correctly - don't see the pwm
//    o->dutyCycleLevel = HEATING_LEVEL_TYPE_MEDIUM;
    o->dutyCycleLevel = HEATING_LEVEL_TYPE_MAXIMUM;
//    o->dutyCycleLevel = HEATING_LEVEL_TYPE_STRONG;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructLocalTime(LocalTime *o) {
    o->periodTimeStamp = 0;
};

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructActuationCommand(ActuationCommand *const o) {
    constructActuators(&o->actuators);
    constructHeatingMode(&o->actuationPower);
    constructLocalTime(&o->actuationStart);
    constructLocalTime(&o->actuationEnd);
    o->isScheduled = false;
    o->executionState = ACTUATION_STATE_TYPE_IDLE;
}
