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
#define TIME_SYNCHRONIZATION_SAMPLE_OFFSET ((uint16_t) 0x7fff)

/**
 * mean calculation on-line vs off-line
 */
#define SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION

/**
 * Synchronization strategy.
 */
//#define SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
#define SYNCHRONIZATION_STRATEGY_MEAN
//#define SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING

/**
 *  Manual clock synchronization adjustment to decelerate the local time tracking.
 *  Increases the timer/counter compare value.
 *  As observed the approximated duration mean needs to be decelerated by ~1-2µs,
 *  which is ~8-16clocks.
 *  A weight for this constant value should be correlated to the current MCU clock or in other words
 *  correlated to the difference of the current mean and the initial local time tracking timer/counter
 *  compare value.
 */
#define __SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_SLOW_DOWN + ((uint8_t) 12)
#define __SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_NO_ACCELERATION
#define __SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_SPEED_UP - ((uint8_t) 12)
#define SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION __SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_SLOW_DOWN
