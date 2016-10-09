/**
 * @author Raoul Rubien  12.07.2016
 */

#pragma once

#include <stdint.h>

/**
 * Structure to Keep track of number of intervals passed since time tracking was activated.
 * A time interval can be adjusted at runtime.
 */
typedef struct LocalTimeTracking {
    volatile uint16_t numTimePeriodsPassed;
    /**
     * The current delay for local time tracking. The value is updated by the corresponding ISR only.
     */
    volatile uint16_t timePeriodInterruptDelay;
    /**
     * The new value for local time tracking period interrupt delay.
     */
    volatile uint16_t newTimePeriodInterruptDelay;
    /**
     * Flag indicating new time tracking period interrupt delay available.
     * If true a new value is available which the ISR has to consider
     * and no new value is calculated until the ISR clears the flag.
     * If false a new value can be calculated.
     */
    volatile uint8_t isTimePeriodInterruptDelayUpdateable : 1;
    uint8_t __pad : 7;
} LocalTimeTracking;
