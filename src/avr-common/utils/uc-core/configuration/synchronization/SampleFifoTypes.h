/**
 * @author Raoul Rubien 24.09.2016
 *
 * Sample capturing related arguments.
 */

#pragma once

/**
 * Amount of uint16 samples the synchronization buffers for clock skew approximation.
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE 30

/**
 * The minimum amount of samples needed to perform an approximation.
 */
#define TIME_SYNCHRONIZATION_MINIMUM_SAMPLES 10

/**
 * default numeric value indicating buffer's end position
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END ((uint8_t)(TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE + 1))
