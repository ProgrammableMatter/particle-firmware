/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once

#include "TimeTypes.h"
#include "uc-core/configuration/Time.h"

extern CTOR_ATTRS void constructLocalTimeTracking(volatile LocalTimeTracking *o);

/**
 * constructor function
 */
extern CTOR_ATTRS void constructLocalTimeTracking(volatile LocalTimeTracking *o) {
    o->numTimeIntervalPassed = 0;
    o->interruptDelay = LOCAL_TIME_DEFAULT_INTERRUPT_DELAY;
}