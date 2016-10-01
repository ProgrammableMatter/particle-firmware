/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related arguments.
 */

#pragma once

/**
 * The difference of measured synchronization package durations are shifted by the synthetic offset UINT16_MAX/2=0x7fff
 * to overcome the need of signed data types.
 */
#define TIME_SYNCHRONIZATION_SAMPLE_OFFSET 0x7fff

/**
 * Synchronization strategy.
 */
#define SYNCHRONIZATION_STRATEGY_SIMPLE_MEAN_AND_STDDEVIANCE
//#define SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING

/**
 *  Manual clock synchronization adjustment to decelerate the local time tracking.
 *  Increases the timer/counter compare value.
 *  As observed the approximated duration mean needs to be decelerated by ~1.1µs,
 *  which is 9*(1/800000).
 *  TODO: Investigate why this constant value is necessary (rounding error, float inaccuracy or conceptual error?).
 *  TODO: Enhancement of the SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_DECELERATION value.
 *  A weight for this constant value should be correlated to the current MCU clock or in other words
 *  correlated to the difference of the current mean and the initial local time tracking timer/counter
 *  compare value.
 */
#define SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_DECELERATION ((uint8_t) 10) +
//#define SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_DECELERATION


/**
 * Manual clock synchronization adjustment to accelerate the local time tracking.
 * Decreases the timer/counter compare value.
 * TODO: Investigate why this constant value is necessary (rounding error, float inaccuracy or conceptual error?).
 * TODO: Enhancement of the SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION value.
 */
//#define SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION - ((uint8_t) 0)
#define SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION