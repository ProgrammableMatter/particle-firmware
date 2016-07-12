/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once

#include "TimeTypes.h"
#include "uc-core/particle/Globals.h"

extern FUNC_ATTRS void advanceLocalTime(void);
/**
 * Increments the local time periode counter but overflows at 65000.
 */
FUNC_ATTRS void advanceLocalTime(void) {
    ParticleAttributes.localTime.numTimeIntervalPassed++;
    if (ParticleAttributes.localTime.numTimeIntervalPassed >= 65000) {
        ParticleAttributes.localTime.numTimeIntervalPassed = 0;
    }
}