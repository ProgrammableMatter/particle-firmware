/**
 * @author Raoul Rubien  12.07.2016
 *
 * Time types constructor implementation.
 */

#pragma once

#include "TimeTypes.h"
#include "uc-core/configuration/Time.h"

/**
 * constructor function
 * @param o the object to construct
 */
void constructLocalTimeTracking(LocalTimeTracking *const o) {
    o->numTimePeriodsPassed = 0;
    o->newNumTimePeriodsPassed = 0;
    o->timePeriodInterruptDelay = LOCAL_TIME_DEFAULT_INTERRUPT_DELAY;
    o->newTimePeriodInterruptDelay = LOCAL_TIME_DEFAULT_INTERRUPT_DELAY;
    o->isTimePeriodInterruptDelayUpdateable = false;
    o->isNumTimePeriodsPassedUpdateable = false;
#ifdef LOCAL_TIME_EXPERIMENTAL_IN_PHASE_APPROXIMATION_SHIFT
    o->totalCounterShiftToBeConsumed = 0;
    o->portionCounterShiftToBeConsumed = 0;
    o->isTimerCounterShiftConsumable = false;
#endif
}
