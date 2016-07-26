/**
 * @author Raoul Rubien  12.07.2016
 */

#pragma once

#include "TimeTypes.h"
#include "uc-core/particle/Globals.h"

/**
 * Increments the local time period counter which, if correctly adjusted,
 * overflows each (1/F_CPU * LOCAL_TIME_INTERRUPT_COMPARE_VALUE * 2^16)=
 * 532.48 seconds
 * or 8 minutes and 52.48 seconds.
 */
#define ADVANCE_LOCAL_TIME \
    ( \
        (ParticleAttributes.localTime.numTimePeriodsPassed >= ParticleAttributes.localTime.timePeriodInterruptDelay) \
        ? (ParticleAttributes.localTime.numTimePeriodsPassed = 0) \
        : (ParticleAttributes.localTime.numTimePeriodsPassed++) \
    )

/**
 * Schedules the next time interrupt.
 */
#define SCHEDULE_NEXT_LOCAL_TIME_INTERUPT \
    (LOCAL_TIME_INTERRUPT_COMPARE_VALUE += ParticleAttributes.localTime.timePeriodInterruptDelay)


/**
 * enables the local time interrupt using current adjustment argument
 * {@link ParticleAttributes.localTime.timePeriodInterruptDelay}.
 */
extern FUNC_ATTRS void enableLocalTimeInterrupt(void);

FUNC_ATTRS void enableLocalTimeInterrupt(void) {
    LOCAL_TIME_INTERRUPT_COMPARE_VALUE = ParticleAttributes.localTime.timePeriodInterruptDelay;
    LOCAL_TIME_INTERRUPT_COMPARE_ENABLE;
}
