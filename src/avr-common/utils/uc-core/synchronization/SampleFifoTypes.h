/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo related types.
 */

#pragma once

#include <stdint.h>
#include "uc-core/configuration/synchronization/SampleFifoTypes.h"
#include "LeastSquareRegressionTypes.h"

#ifdef C_STRUCTS_TO_JSON_PARSER_TYPEDEF_NOT_SUPPORTED_SUPPRESS_REGULAR_TYPEDEFS
# define CumulationType uint32_t
# define SampleValueType uint16_t
# define CalculationType float
#define IndexType uint8_t
#else
typedef uint32_t CumulationType;
typedef uint16_t SampleValueType;
typedef float CalculationType;
typedef uint8_t IndexType;
#endif

/**
 * Simple first-in first-out buffer for 1-dimensional samples (y-values).
 * X-values the samples' index.
 */
typedef struct SamplesFifoBuffer {
    /**
     * samples buffer
     */
    SampleValueType samples[TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE];
    /**
     * index of 1st valid element in buffer
     */
    IndexType __startIdx;
    /**
     * index for next insert
     */
    IndexType __insertIndex;
    /**
     * number of buffered samples
     */
    IndexType numSamples;
    /**
    * index of iterator for outer access
    */
    IndexType iterator;
} SamplesFifoBuffer;
