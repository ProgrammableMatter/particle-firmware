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