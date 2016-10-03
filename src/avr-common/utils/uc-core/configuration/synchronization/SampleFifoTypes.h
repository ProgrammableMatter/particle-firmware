/**
 * @author Raoul Rubien 24.09.2016
 *
 * Sample capturing related arguments.
 */

#pragma once

/**
 * Amount of uint16 samples the synchronization buffers for clock skew approximation.
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE 120
//#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE 5

/**
 * The minimum amount of samples needed to perform an approximation.
 */
#define TIME_SYNCHRONIZATION_MINIMUM_SAMPLES 40
//#define TIME_SYNCHRONIZATION_MINIMUM_SAMPLES 2

/**
 * default numeric value indicating buffer's end position
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END ((uint8_t)(TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE + 1))

/**
 * en- or disable outlier rejection
 */
//#define SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION
//#define SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION

#ifdef SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION
/**
 * Defines the factor f for outlier detection. Samples having values not within
 * [µ - f * σ, µ + f * σ] are rejected.
 */
#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 48.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 24.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 12.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 9.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 6.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 4.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 2.8)
#endif
