/**
 * @author Raoul Rubien  12.07.2016
 */

#pragma once
/**
 * Structure to Keep track of number of intervals passed since time tracking was activated.
 * A time interval can be adjusted at runtime and lasts depending on
 * <br/>TIMER_TX_RX_COUNTER_ENABLE and
 * <br/>LOCAL_TIME_DEFAULT_INTERRUPT_DELAY or
 * <br/>{@link LocalTimeTracking.timePeriodInterruptDelay}
 * <br/>approximately
 * (timer_counter_prescaler/F_CPU * LocalTimeTracking.timePeriodInterruptDelay), thus ~8.125ms.
 */
typedef struct LocalTimeTracking {
    uint16_t numTimePeriodsPassed;
    /**
     * adjustable delay for fine tuning
     */
    uint16_t timePeriodInterruptDelay;
} LocalTimeTracking;
