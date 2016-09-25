/**
 * @author Raoul Rubien  12.07.2016
 */

#pragma once

#include "TimeTypes.h"
#include "uc-core/particle/Globals.h"

/**
 * Schedules the next time interrupt.
 */
void scheduleNextLocalTimeInterrupt(void) {
    if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable) {
        ParticleAttributes.localTime.timePeriodInterruptDelay = ParticleAttributes.localTime.newTimePeriodInterruptDelay;
        MEMORY_BARRIER;
        ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = false;
    }
    LOCAL_TIME_INTERRUPT_COMPARE_VALUE += ParticleAttributes.localTime.timePeriodInterruptDelay;
}

/**
 * enables the local time interrupt using current adjustment argument
 * {@link ParticleAttributes.localTime.timePeriodInterruptDelay}.
 */
void enableLocalTimeInterrupt(void) {
    LOCAL_TIME_INTERRUPT_COMPARE_VALUE = ParticleAttributes.localTime.timePeriodInterruptDelay;
    MEMORY_BARRIER;
    LOCAL_TIME_INTERRUPT_COMPARE_ENABLE;
}
