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
                                                      const SampleValueType *const fifoIn,
                                                      const SampleValueType *const fifoOut) {
    timeSynchronization->__unnormalizedCumulativeMean += *fifoIn;
    timeSynchronization->__unnormalizedCumulativeMean -= *fifoOut;

    if (isEnoughFifoDataAvailable(timeSynchronization)) {
        // on enough data stored: update mean
        timeSynchronization->mean = (CalculationType) timeSynchronization->__unnormalizedCumulativeMean /
                                    (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;
    }
}

/**
 * Updates the mean of all available samples using only observations of incoming and outgoing FiFo values.
 * One call to this function does not iterate the whole FiFo.
 * An updated is performed if the TIME_SYNCHRONIZATION_MINIMUM_SAMPLES limit is exceeded.
 */
static void __calculateMeanUsingFifoInObservations(TimeSynchronization *const timeSynchronization,
                                                   const SampleValueType *const fifoIn) {
    timeSynchronization->__unnormalizedCumulativeMean += *fifoIn;
    if (isEnoughFifoDataAvailable(timeSynchronization)) {
        timeSynchronization->mean = (CalculationType) timeSynchronization->__unnormalizedCumulativeMean /
                                    (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;
    }
}

/**
 * Calculates a quick and simple approximated mean value.
 * This method is not very stable against outlier.
 */
static void __calculateProgressiveMean(const SampleValueType *const sample,
                                       TimeSynchronization *const timeSynchronization) {
    if (timeSynchronization->progressiveMean == 0) {
        timeSynchronization->progressiveMean = *sample;
    } else {
        timeSynchronization->progressiveMean += *sample;
        timeSynchronization->progressiveMean = (CalculationType) timeSynchronization->progressiveMean / 2.0;
    }
}

#ifdef SYNCHRONIZATION_ENABLE_OUTLIER_REJECTION
static void __reduceRejectionCounters(AdaptiveSampleRejection *const adaptiveSampleRejection) {
    if ((adaptiveSampleRejection->rejected >= UINT8_MAX) ||
        (adaptiveSampleRejection->accepted >= UINT8_MAX)) {
        adaptiveSampleRejection->rejected =
                (1.0 + (float) adaptiveSampleRejection->rejected) / 2.0;
        adaptiveSampleRejection->accepted =
                (1.0 + (float) adaptiveSampleRejection->accepted) / 2.0;
    }
}

static void __updateCurrentRejectionRatio(AdaptiveSampleRejection *const adaptiveSampleRejection) {
    // omit division by 0
    if (adaptiveSampleRejection->accepted <= 0 || adaptiveSampleRejection->rejected <= 0) {
        adaptiveSampleRejection->accepted++;
        adaptiveSampleRejection->rejected++;
    }
    adaptiveSampleRejection->currentRejectionRatio =
            (float) adaptiveSampleRejection->rejected /
            (float) adaptiveSampleRejection->accepted;
}

static void __updateCurrentRejectionBoundaries(TimeSynchronization *const timeSynchronization) {
    LED_STATUS1_ON;
    AdaptiveSampleRejection *const adaptiveSampleRejection = &timeSynchronization->adaptiveSampleRejection;
    __updateCurrentRejectionRatio(adaptiveSampleRejection);

    // TODO: move attenuation factor to configuration
    // update rejection interval
    if (adaptiveSampleRejection->currentRejectionRatio >
        adaptiveSampleRejection->targetRejectionRatio) {
        // linear decrease rejection boundary with attenuation factor
        adaptiveSampleRejection->currentAcceptedDeviation -=
                0.75 * adaptiveSampleRejection->currentAcceptedDeviation /
                (adaptiveSampleRejection->currentRejectionRatio /
                 adaptiveSampleRejection->targetRejectionRatio);

        if (adaptiveSampleRejection->currentAcceptedDeviation <= 1000) {
            adaptiveSampleRejection->currentAcceptedDeviation = 1000;
        }
        LED_STATUS2_ON;
        LED_STATUS3_OFF;
    } else {
        // linear increase rejection boundary with attenuation factor
        adaptiveSampleRejection->currentAcceptedDeviation +=
                0.75 * adaptiveSampleRejection->currentAcceptedDeviation *
                (adaptiveSampleRejection->targetRejectionRatio /
                 adaptiveSampleRejection->currentRejectionRatio);

        if (adaptiveSampleRejection->currentAcceptedDeviation >= 32000) {
            adaptiveSampleRejection->currentAcceptedDeviation = 32000;
        }
        LED_STATUS2_OFF;
        LED_STATUS3_ON;
    }


    if (adaptiveSampleRejection->currentAcceptedDeviation > 3000) {
        LED_STATUS1_OFF;
    }
    // update new rejection boundaries
    adaptiveSampleRejection->outlierLowerBound =
            timeSynchronization->mean - adaptiveSampleRejection->currentAcceptedDeviation;
    adaptiveSampleRejection->outlierUpperBound =
            timeSynchronization->mean + adaptiveSampleRejection->currentAcceptedDeviation;
    adaptiveSampleRejection->isOutlierRejectionBoundValid = true;
}
#endif

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample,
                                TimeSynchronization *const timeSynchronization) {
#ifdef SYNCHRONIZATION_ENABLE_OUTLIER_REJECTION
    if (timeSynchronization->timeIntervalSamples.numSamples >=
        TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {

        bool isToBeRejected = false;
        if (timeSynchronization->adaptiveSampleRejection.isOutlierRejectionBoundValid) {
            if (*sample < timeSynchronization->adaptiveSampleRejection.outlierLowerBound ||
                *sample > timeSynchronization->adaptiveSampleRejection.outlierUpperBound) {
                timeSynchronization->adaptiveSampleRejection.rejected++;
                isToBeRejected = true;
            } else {
                timeSynchronization->adaptiveSampleRejection.accepted++;
            }
        }

        __reduceRejectionCounters(&timeSynchronization->adaptiveSampleRejection);
        __updateCurrentRejectionBoundaries(timeSynchronization);

        if (isToBeRejected) {
            return;
        }
    }
#endif

    if (timeSynchronization->timeIntervalSamples.numSamples < TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {
        timeSynchronization->timeIntervalSamples.numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(&timeSynchronization->timeIntervalSamples);
    timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.__insertIndex] = *sample;

    // calculate progressive mean
    __calculateProgressiveMean(sample, timeSynchronization);

    // calculate mean stepwise
    if (timeSynchronization->timeIntervalSamples.isDropOutValid) {
        // on fifo has dropped out a first-in value
        __calculateMeanUsingFifoInOutObservations(timeSynchronization,
                                                  sample,
                                                  &timeSynchronization->timeIntervalSamples.dropOut);
    } else {
        // on fifo not entirely saturated
        __calculateMeanUsingFifoInObservations(timeSynchronization, sample);
    }
}
