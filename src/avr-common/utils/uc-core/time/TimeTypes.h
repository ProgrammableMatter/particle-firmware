/**
 * @author Raoul Rubien  12.07.2016
 */

#pragma once
/**
 * Structure to Keep track of number of intervals passed since time tracking was activated.
 * A time interval can be adjusted at runtime.
 */
typedef struct {
    volatile uint16_t numTimePeriodsPassed;
    /**
     * adjustable delay for fine tuning
     */
    volatile uint16_t timePeriodInterruptDelay;
    /**
     * new value for local time tracking period int. delay
     */
    volatile uint16_t newTimePeriodInterruptDelay;
    /**
     * flag indicating new time tracking period int. delay available
     */
    volatile uint8_t isTimePeriodInterruptDelayUpdateable : 1;
    uint8_t __pad : 7;
} LocalTimeTracking;
