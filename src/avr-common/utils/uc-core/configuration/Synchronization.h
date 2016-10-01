/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related arguments.
 */

#pragma once

/**
 * Amount of uint16 samples the synchronization buffers for clock skew approximation.
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE 10

/**
 * The minimum amount of samples needed to perform an approximation.
 */
#define TIME_SYNCHRONIZATION_MINIMUM_SAMPLES 3

/**
 * default numeric value indicating buffer's end position
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END ((uint8_t)(TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE + 1))


/**
 * The difference of measured synchronization package durations are shifted by the synthetic offset UINT16_MAX/2=0x7fff
 * to overcome the need of signed data types.
 */
#define TIME_SYNCHRONIZATION_SAMPLE_OFFSET 0x7fff