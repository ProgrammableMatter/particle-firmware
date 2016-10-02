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
#define __SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_SLOW_DOWN + ((uint8_t) 12)
//#define __SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_SPEED_UP - ((uint8_t) 0)
#define SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION __SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_SLOW_DOWN

/**
 * en- or disable outlier rejection
 */
#define SYNCHRONIZATION_ENABLE_OUTLIER_REJECTION
//#define SYNCHRONIZATION_DISABLE_OUTLIER_REJECTION

/**
 * Defines the factor f for outlier detection. Samples having values not within
 * [µ - f * σ, µ + f * σ] are rejected.
 */
#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((float) 12.0)
//#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((float) 9.0)
//#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((float) 6.0)
//#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((float) 4.0)
//#define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((float) 2.8)