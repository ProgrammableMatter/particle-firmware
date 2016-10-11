/**
 * @author Raoul Rubien  12.07.2016
 */

#pragma once

#include <stdint.h>
#include "uc-core/configuration/Time.h"

/**
 * Structure to Keep track of number of intervals passed since time tracking was activated.
 * A time interval can be adjusted at runtime.
 */
typedef struct LocalTimeTracking {
    /**
     * The current local time.
     */
    volatile uint16_t numTimePeriodsPassed;
    /**
     * The new local time to be considered in the next local time tracking ISR.
     */
    volatile uint16_t newNumTimePeriodsPassed;
    /**
     * The current delay for local time tracking. The value is updated by the corresponding ISR only.
     * TODO: volatile declaration for this field may be unnecessary, since it is only by the ISR.
     */
    volatile uint16_t timePeriodInterruptDelay;
    /**
     * The new value for local time tracking period interrupt delay.
     */
    volatile uint16_t newTimePeriodInterruptDelay;
    /**
     * Flag indicating new time tracking period interrupt delay is available.
     * If true a new value is available which the ISR has to consider
     * and no new value is calculated until the ISR clears the flag.
     * If false a new value can be calculated.
     */
    volatile uint8_t isTimePeriodInterruptDelayUpdateable : 1;
#ifdef LOCAL_TIME_EXPERIMENTAL_IN_PHASE_APPROXIMATION_SHIFT
    /**
     * In case the local time tracking interval has to be shifted forward/backward to be in phase with
     * a remote MCU, the total amount of units to decelerate the clock once can be set.
     * The units are decremented and consumed subsequently.
     */
    uint16_t totalCounterShiftToBeConsumed;
    /**
     * The current portion of the total clock counter shift to be observed by the next ISR.
     */
    volatile uint16_t portionCounterShiftToBeConsumed;
#endif
    /**
     * Flag indicating new time period value is available.
     */
    volatile uint8_t isNumTimePeriodsPassedUpdateable : 1;
#ifdef LOCAL_TIME_EXPERIMENTAL_IN_PHASE_APPROXIMATION_SHIFT
    /**
     * Flag indicating the ISR can safely consume the shift portion.
     * Cleared by the same ISR after consumption.
     */
    volatile uint8_t isTimerCounterShiftConsumable: 1;
    uint8_t __pad : 5;
#else
    uint8_t __pad : 6;
#endif
} LocalTimeTracking;
