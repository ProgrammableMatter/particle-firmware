/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo related implementation.
 */

#pragma once

#include "SynchronizationTypes.h"

/**
 * circular-increment an index with respect to the fifo buffer boundaries
 */
static void __samplesFifoBufferIncrementIndex(uint8_t *const index) {
    if (*index < (TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE - 1)) {
        (*index)++;
    } else {
        *index = 0;
    }
}

/**
 * Increments the iterator. If the position equals TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END
 * the buffer's end as been reached.
 */
void samplesFifoBufferFiFoBufferIteratorNext(SamplesFifoBuffer *const samplesBuffer) {
    __samplesFifoBufferIncrementIndex(&samplesBuffer->iterator);
    if (samplesBuffer->iterator == samplesBuffer->__startIdx) {
        samplesBuffer->iterator = TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END;
    }
}

/**
 * Sets the iterator to start position.
 */
void samplesFifoBufferIteratorStart(SamplesFifoBuffer *const samplesBuffer) {
    samplesBuffer->iterator = samplesBuffer->__startIdx;
}

/**
 * Increments the end index for the next value to be inserted (First-in).
 * Also updates the start index for next value to be removed (First-out).
 */
static void __samplesFifoBufferIncrementInsertIndex(SamplesFifoBuffer *const samplesBuffer) {
    if (samplesBuffer->numSamples <= 0) {
        samplesBuffer->__insertIndex++;
    } else {
        __samplesFifoBufferIncrementIndex(&samplesBuffer->__insertIndex);
        if (samplesBuffer->__insertIndex == samplesBuffer->__startIdx) {
            __samplesFifoBufferIncrementIndex(&samplesBuffer->__startIdx);
        }
    }
}

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType sample, SamplesFifoBuffer *const samplesBuffer) {
    // TODO: if enough samples, enable outlier detection using µ +/- 2*sigma
    if (samplesBuffer->numSamples < TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {
        samplesBuffer->numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(samplesBuffer);
    samplesBuffer->samples[samplesBuffer->__insertIndex] = sample;
}
