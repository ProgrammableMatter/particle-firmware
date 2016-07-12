/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once

#include "TimeTypes.h"

extern CTOR_ATTRS void constructLocalTime(volatile LocalTime *o);

/**
 * constructor function
 */
extern CTOR_ATTRS void constructLocalTime(volatile LocalTime *o) {
    o->numTimeIntervalPassed = 0;
}