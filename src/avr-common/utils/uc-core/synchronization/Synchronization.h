/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related implementation.
 */

#pragma once

#include "SynchronizationTypes.h"

/**
 * Increments the iterator. If the position equals TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END
 * the buffer's end as been reached.
 */
void samplesFifoBufferFiFoBufferIteratorNext(SamplesFifoBuffer *const samplesBuffer) {

    if (samplesBuffer->iterator >= (TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE - 1)) {
        samplesBuffer->iterator = 0;
    } else {
        samplesBuffer->iterator++;
    }

    if (samplesBuffer->iterator == samplesBuffer->endIdx) {
        samplesBuffer->iterator = TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END;
    }
}

/**
 * Sets the iterator to start position.
 */
void samplesFifoBufferIteratorStart(SamplesFifoBuffer *const samplesBuffer) {
    samplesBuffer->iterator = samplesBuffer->startIdx;
}

/**
 * Increments the end index for the next value to be inserted (First-in).
 * Also updates the start index for next value to be removed (First-out).
 */
static void __samplesFifoBufferIncrementEndIndex(SamplesFifoBuffer *const samplesBuffer) {

    bool isBufferFull = false;
    if (samplesBuffer->endIdx == samplesBuffer->startIdx) {
        isBufferFull = true;
    }

    if (samplesBuffer->endIdx >= (TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE - 1)) {
        samplesBuffer->endIdx = 0;
    } else {
        samplesBuffer->endIdx++;
    }

    if (isBufferFull) {
        samplesBuffer->startIdx = samplesBuffer->endIdx;
    }
}

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType sample, SamplesFifoBuffer *const samplesBuffer) {
    __samplesFifoBufferIncrementEndIndex(samplesBuffer);
    samplesBuffer->samples[samplesBuffer->endIdx] = sample;
}
