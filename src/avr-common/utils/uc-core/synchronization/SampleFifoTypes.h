/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo related types.
 */

#pragma once

#include <stdint.h>
#include "BasicCalculationTypes.h"
#include "uc-core/configuration/synchronization/SampleFifoTypes.h"
#include "LeastSquareRegressionTypes.h"

typedef struct FifoElement {
    SampleValueType value;
    uint8_t isRejected : 1;
    uint8_t __pad : 7;
} FifoElement;

/**
 * Simple first-in first-out buffer for 1-dimensional samples (y-values).
 * X-values the samples' index.
 */
typedef struct SamplesFifoBuffer {
    /**
     * samples buffer
     */
    FifoElement samples[SAMPLE_FIFO_NUM_BUFFER_ELEMENTS];
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
    /**
     * the last FiFo value that has been dropped out of the queue
     */
    FifoElement dropOut;
    /**
     * indicates whether the dropOut value is valid or not
     */
    uint8_t isDropOutValid :1;
    uint8_t __isPreDropOutValid :1;

    uint8_t __pad :6;

} SamplesFifoBuffer;
