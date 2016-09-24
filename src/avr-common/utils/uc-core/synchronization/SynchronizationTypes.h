/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related types.
 */

#pragma once

#include <stdint.h>
#include "uc-core/configuration/Synchronization.h"

#ifdef C_STRUCTS_TO_JSON_PARSER_TYPEDEF_NOT_SUPPORTED_SUPPRESS_REGULAR_TYPEDEFS
# define SampleValueType uint16_t
# define CalculationType float
# define DeviationType float
#else
typedef uint16_t SampleValueType;
typedef float CalculationType;
typedef float DeviationType;
#endif

/**
 * Simple first-in first-out buffer for 1-dimensional samples (y-values).
 * X-values the samples' index.
 */
typedef struct {
    /**
     * samples buffer
     */
    SampleValueType samples[TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE];
    /**
     * number of buffered samples
     */
    uint8_t numSamples;
    /**
     * index of 1st valid element in buffer
     */
    uint8_t startIdx;
    /**
     * index of 1st invalid element in buffer
     */
    uint8_t endIdx;
    /**
    * index of current iterator
    */
    uint8_t iterator;
    /**
    * k as known of f(x)=k*x+d
    */
    CalculationType k;
    /**
    * d as known of f(x)=k*x+d
    */
    CalculationType d;
    /**
    * sample's variance
    */
    DeviationType variance;
} SamplesFifoBuffer;


typedef struct {
    SamplesFifoBuffer timeIntervalSamples;
} TimeSynchronization;