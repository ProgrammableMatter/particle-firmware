/**
 * @author Raoul Rubien 24.09.2016
 *
 * Sample capturing related arguments.
 */

#pragma once

#include <stdint.h>
/**
 * Amount of uint16 samples the synchronization buffers for clock skew approximation.
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE 120

/**
 * The minimum amount of samples needed to perform an approximation.
 */
#define TIME_SYNCHRONIZATION_MINIMUM_SAMPLES 90

/**
 * default numeric value indicating buffer's end position
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END ((uint8_t)(TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE + 1))

/**
 * en- or disable outlier rejection
 */
//#define SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION
#define SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION

#ifdef SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION
/**
 * Defines the factor f for outlier detection. Samples having values not within
 * [µ - f * σ, µ + f * σ] are rejected.
 */
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 48.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 24.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 12.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 9.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 6.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 4.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 3.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 2.8)
#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 2.0)
//#  define SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR ((CalculationType) 1.0)
#endif

#ifdef SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION
/**
 * if rejected or accepted counter >= SAMPLE_FIFO_ADAPTIVE_REJECTION_REDUCE_COUNTERS_LIMIT
 * both counters are reduced by factor 2.0
 */
#  define SAMPLE_FIFO_ADAPTIVE_REJECTION_REDUCE_COUNTERS_LIMIT ((uint16_t) 2000)
/**
 * threshold where no interval updates are performed
 */
#  define SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_INTERVAL_THRESHOLD ((uint8_t) 25)
/**
 * step to increase/decrease the acceptance interval
 */
#  define SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_STEP ((uint8_t) 10)
/**
 * minimum acceptance interval:
 * mean +/- SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MIN_INTERVAL
 */
#  define SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MIN_INTERVAL ((int8_t) 10)
/**
 * maximum acceptance interval:
 * mean +/- SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MAX_INTERVAL
 */
#  define SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MAX_INTERVAL ((int16_t) 20000)
/**
 * the factor of accepted versus rejected:
 * accpeted ~ SAMPLE_FIFO_ADAPTIVE_REJECTION_ACCEPTANCE_RATIO * rejected
 */
#  define SAMPLE_FIFO_ADAPTIVE_REJECTION_ACCEPTANCE_RATIO ((uint16_t) 9)
#endif