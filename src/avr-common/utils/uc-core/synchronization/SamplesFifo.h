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
            samplesBuffer->dropOut = samplesBuffer->samples[samplesBuffer->__startIdx];
            samplesBuffer->isDropOutValid = true;
            __samplesFifoBufferIncrementIndex(&samplesBuffer->__startIdx);
        }
    }
}

/**
 * @return true if enough data is queued for calculations
 */
static bool isEnoughFifoDataAvailable(const TimeSynchronization *const timeSynchronization) {
    // on too less samples calculation is not performed
    if (timeSynchronization->timeIntervalSamples.numSamples < (TIME_SYNCHRONIZATION_MINIMUM_SAMPLES - 1)) {
        return false;
    } else {
        return true;
    }
}

/**
 * Updates the mean of all available samples using only observations of incoming and outgoing FiFo values.
 * One call to this function does not iterate the whole FiFo.
 * An updated is performed if the TIME_SYNCHRONIZATION_MINIMUM_SAMPLES limit is exceeded.
 */
static void __calculateMeanUsingFifoInOutObservations(TimeSynchronization *const timeSynchronization,
                                                      const SampleValueType fifoIn,
                                                      const SampleValueType fifoOut) {
    timeSynchronization->__unnormalizedCumulativeMean += fifoIn;
    timeSynchronization->__unnormalizedCumulativeMean -= fifoOut;

    if (isEnoughFifoDataAvailable(timeSynchronization)) {
        // on enough data stored: update mean
        timeSynchronization->mean = timeSynchronization->__unnormalizedCumulativeMean /
                                    (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;
    }
}

/**
 * Updates the mean of all available samples using only observations of incoming and outgoing FiFo values.
 * One call to this function does not iterate the whole FiFo.
 * An updated is performed if the TIME_SYNCHRONIZATION_MINIMUM_SAMPLES limit is exceeded.
 */
static void __calculateMeanUsingFifoInObservations(TimeSynchronization *const timeSynchronization,
                                                   const SampleValueType fifoIn) {
    timeSynchronization->__unnormalizedCumulativeMean += fifoIn;
    if (isEnoughFifoDataAvailable(timeSynchronization)) {
        timeSynchronization->mean = timeSynchronization->__unnormalizedCumulativeMean /
                                    (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;
    }
}

/**
 * Calculates a quick and simple approximated mean value.
 * This method is not very stable against outlier.
 */
static void __calculateProgressiveMean(const CumulationType sample,
                                       TimeSynchronization *const timeSynchronization) {
    if (timeSynchronization->progressiveMean == 0) {
        timeSynchronization->progressiveMean = sample;
    } else {
        timeSynchronization->progressiveMean += sample;
        timeSynchronization->progressiveMean /= 2.0;
    }
}

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType sample,
                                TimeSynchronization *const timeSynchronization) {
//    if (timeSynchronization->isOutlierRejectionBoundValid) {
//        if ((float)sample < timeSynchronization->outlierLowerBound ||
//                (float)sample > timeSynchronization->outlierUpperBound) {
//            return;
//        }
//    }

    if (timeSynchronization->timeIntervalSamples.numSamples < TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {
        timeSynchronization->timeIntervalSamples.numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(&timeSynchronization->timeIntervalSamples);
    timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.__insertIndex] = sample;

    // calculate progressive mean
    __calculateProgressiveMean(sample, timeSynchronization);

    // calculate mean stepwise
    if (timeSynchronization->timeIntervalSamples.isDropOutValid) {
        // on fifo has dropped out a first-in value
        __calculateMeanUsingFifoInOutObservations(timeSynchronization,
                                                  sample,
                                                  timeSynchronization->timeIntervalSamples.dropOut);
    } else {
        // on fifo not entirely saturated
        __calculateMeanUsingFifoInObservations(timeSynchronization, sample);
    }
}
