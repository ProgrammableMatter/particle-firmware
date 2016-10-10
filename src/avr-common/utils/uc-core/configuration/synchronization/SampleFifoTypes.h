/**
 * @author Raoul Rubien 24.09.2016
 *
 * Sample capturing related arguments.
 */

#pragma once

/**
 * Amount of uint16 samples the synchronization buffers for clock skew approximation.
 */
//#define SAMPLE_FIFO_NUM_BUFFER_ELEMENTS 120
#define SAMPLE_FIFO_NUM_BUFFER_ELEMENTS 80

/**
 * default numeric value indicating buffer's end position
 */
#define TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END ((uint8_t)(SAMPLE_FIFO_NUM_BUFFER_ELEMENTS + 1))

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