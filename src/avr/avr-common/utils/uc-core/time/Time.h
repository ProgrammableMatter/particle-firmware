/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once

#include "TimeTypes.h"
#include "uc-core/particle/Globals.h"

extern FUNC_ATTRS void advanceLocalTime(void);
/**
 * Increments the local time period counter which, if correctly adjusted,
 * overflows each 390 seconds or 6 minutes and 30 seconds.
 */
FUNC_ATTRS void advanceLocalTime(void) {
    ParticleAttributes.localTime.numTimeIntervalPassed++;
    if (ParticleAttributes.localTime.numTimeIntervalPassed >= ParticleAttributes.localTime.interruptDelay) {
        ParticleAttributes.localTime.numTimeIntervalPassed = 0;
    }
}

extern FUNC_ATTRS void scheduleNextLocalTimeInterrupt(void);

/**
 * schedule the next time interrupt
 */
extern FUNC_ATTRS void scheduleNextLocalTimeInterrupt(void) {
    LOCAL_TIME_INTERRUPT_COMPARE_VALUE += ParticleAttributes.localTime.interruptDelay;
}