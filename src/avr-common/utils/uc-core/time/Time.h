/**
 * @author Raoul Rubien  12.07.2016
 */

#pragma once

#include "TimeTypes.h"
#include "uc-core/particle/Globals.h"

/**
 * Enables the local time interrupt using current adjustment argument
 * {@link ParticleAttributes.localTime.timePeriodInterruptDelay}.
 * Write to LOCAL_TIME_INTERRUPT_COMPARE_VALUE is not atomic.
 */
void enableLocalTimeInterrupt(void) {
    LOCAL_TIME_INTERRUPT_COMPARE_DISABLE;
    MEMORY_BARRIER;
    LOCAL_TIME_INTERRUPT_COMPARE_VALUE = ParticleAttributes.localTime.timePeriodInterruptDelay;
    MEMORY_BARRIER;
    LOCAL_TIME_INTERRUPT_COMPARE_ENABLE;
}

#ifdef LOCAL_TIME_EXPERIMENTAL_IN_PHASE_APPROXIMATION_SHIFT

#include "uc-core/stdout/stdio.h"

/**
 * Calculates a total amount of lag in units to be lowered down the local time tracking timer/counter.
 * If units are still consumed this function has no effect and returns immediately.
 */
void updateConsumableLagBetweenLocalAndRemoteTimerCounter(uint16_t localTimerCounterValue,
                                                          uint16_t remoteTimerCounterValue) {
    if (ParticleAttributes.localTime.totalCounterShiftToBeConsumed > 0) {
        // still consume-able units left
        return;
    }

    uint16_t difference;
    // @pre: the local timer counter speed is always lowered but never advanced
    // until it is again in phase with the remote counter
    if (localTimerCounterValue <= remoteTimerCounterValue) {
        // on local timer counter lower, slower down
        // for [localValue, remoteValue] ticks
        difference = remoteTimerCounterValue - localTimerCounterValue;
    } else {
        // on local timer is more advanced than the remote,
        // slower down for [0, remoteValue] plus [localValue, UINT16_MAX] ticks
        difference = remoteTimerCounterValue + (UINT16_MAX - localTimerCounterValue);
    }

    ParticleAttributes.localTime.totalCounterShiftToBeConsumed = difference;
    MEMORY_BARRIER;
    ParticleAttributes.localTime.isTimerCounterShiftConsumable = true;

}

/**
 * Takes a portion of units to be consumed and exposes it to the corresponding ISR.
 */
void shiftConsumableLocalTimeTrackingClockLagUnitsToIsr(void) {
    if (ParticleAttributes.timeSynchronization.timeIntervalSamples.numSamples <
        SAMPLE_FIFO_NUM_BUFFER_ELEMENTS) {
        return;
    }
    if (ParticleAttributes.localTime.isTimerCounterShiftConsumable) {
        // ISR has still not fetched the next portion to consume
        return;
    }

    uint16_t portion = 16;
    if (ParticleAttributes.localTime.totalCounterShiftToBeConsumed > 0) {
        // on still units to be consumed available
        if (ParticleAttributes.localTime.totalCounterShiftToBeConsumed < portion) {
            portion = ParticleAttributes.localTime.totalCounterShiftToBeConsumed;
        }
        ParticleAttributes.localTime.totalCounterShiftToBeConsumed -= portion;
//    ParticleAttributes.localTime.portionCounterShiftToBeConsumed = 0;
        ParticleAttributes.localTime.portionCounterShiftToBeConsumed = portion;
        MEMORY_BARRIER;
        // expose order to ISR
        ParticleAttributes.localTime.isTimerCounterShiftConsumable = true;
    }
//    printf("t%u p%u\n", ParticleAttributes.localTime.totalCounterShiftToBeConsumed,
//           ParticleAttributes.localTime.portionCounterShiftToBeConsumed);
}

#endif