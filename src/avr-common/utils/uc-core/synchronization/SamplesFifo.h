/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo related implementation.
 */

#pragma once

#include "uc-core/configuration/synchronization/SampleFifoTypes.h"
#include "SynchronizationTypes.h"
#include <math.h>


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
    // if too less samples calculation is not performed
    if (timeSynchronization->timeIntervalSamples.numSamples < (TIME_SYNCHRONIZATION_MINIMUM_SAMPLES - 1)) {
        return false;
    } else {
        return true;
    }
}

#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION

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

#endif

#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
/**
 * Calculates a quick and simple approximated mean value.
 * This method is not very stable against outlier.
 */
static void __calculateProgressiveMean(const SampleValueType sample,
                                       TimeSynchronization *const timeSynchronization) {
    if (timeSynchronization->progressiveMean == 0) {
        timeSynchronization->progressiveMean = sample;
    } else {
        timeSynchronization->progressiveMean += sample;
        timeSynchronization->progressiveMean = (CalculationType) timeSynchronization->progressiveMean / 2.0;
    }
}
#endif

#ifdef SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION

/**
 * Update the outlier limits.
 */
static void __updateOutlierRejectionLimitDependingOnSigma(TimeSynchronization *const timeSynchronization) {
    CalculationType rejectionLimit = (CalculationType) (0.5 +
                                                        (SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR *
                                                         timeSynchronization->stdDeviance));
    timeSynchronization->adaptiveSampleRejection.outlierLowerBound =
            timeSynchronization->mean - rejectionLimit;
    timeSynchronization->adaptiveSampleRejection.outlierUpperBound =
            timeSynchronization->mean + rejectionLimit;
    timeSynchronization->adaptiveSampleRejection.isOutlierRejectionBoundValid = true;
}

#endif

#ifdef SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION

static void __reduceRejectionCounters(AdaptiveSampleRejection *const adaptiveSampleRejection) {
    if ((adaptiveSampleRejection->rejected >= SAMPLE_FIFO_ADAPTIVE_REJECTION_REDUCE_COUNTERS_LIMIT) ||
        (adaptiveSampleRejection->accepted >= SAMPLE_FIFO_ADAPTIVE_REJECTION_REDUCE_COUNTERS_LIMIT)) {
        adaptiveSampleRejection->rejected <<= 1;
        adaptiveSampleRejection->accepted <<= 1;
    }

    if (adaptiveSampleRejection->accepted <= 0 || adaptiveSampleRejection->rejected <= 0) {
        adaptiveSampleRejection->accepted++;
        adaptiveSampleRejection->rejected++;
    }
}

static void __updateCurrentRejectionBoundaries(TimeSynchronization *const timeSynchronization) {
    AdaptiveSampleRejection *const adaptiveSampleRejection = &timeSynchronization->adaptiveSampleRejection;
    // update rejection interval
    if (adaptiveSampleRejection->accepted >
        (SAMPLE_FIFO_ADAPTIVE_REJECTION_ACCEPTANCE_RATIO * adaptiveSampleRejection->rejected +
         SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_INTERVAL_THRESHOLD)) {
        // on accepted above limit, decrease rejection interval
        adaptiveSampleRejection->currentAcceptedDeviation -= SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_STEP;
        if (adaptiveSampleRejection->currentAcceptedDeviation <=
            SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MIN_INTERVAL) {
            adaptiveSampleRejection->currentAcceptedDeviation = SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MIN_INTERVAL;
//            LED_STATUS2_ON;
        } else {
//            LED_STATUS2_TOGGLE;
        }
    } else if ((adaptiveSampleRejection->accepted +
                SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_INTERVAL_THRESHOLD) <
               (SAMPLE_FIFO_ADAPTIVE_REJECTION_ACCEPTANCE_RATIO * adaptiveSampleRejection->rejected)) {
        // on accepted below limit, increase rejection interval
        adaptiveSampleRejection->currentAcceptedDeviation += SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_STEP;
        if (adaptiveSampleRejection->currentAcceptedDeviation >=
            SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MAX_INTERVAL) {
            adaptiveSampleRejection->currentAcceptedDeviation = SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MAX_INTERVAL;
            LED_STATUS3_ON;
        } else {
//            LED_STATUS3_TOGGLE;
        }
    } else {
//        LED_STATUS3_TOGGLE;
//        LED_STATUS2_TOGGLE;
    }

    // update new rejection boundaries
    adaptiveSampleRejection->outlierLowerBound =
            timeSynchronization->mean - adaptiveSampleRejection->currentAcceptedDeviation;
    adaptiveSampleRejection->outlierUpperBound =
            timeSynchronization->mean + adaptiveSampleRejection->currentAcceptedDeviation;
    adaptiveSampleRejection->isOutlierRejectionBoundValid = true;
}

#endif

#include "Deviation.h"

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample,
                                TimeSynchronization *const timeSynchronization) {
#ifdef SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION
    if (timeSynchronization->timeIntervalSamples.numSamples >=
        TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {

        bool isToBeRejected = false;
//        if (timeSynchronization->adaptiveSampleRejection.isOutlierRejectionBoundValid) {
        if (*sample < timeSynchronization->adaptiveSampleRejection.outlierLowerBound ||
            *sample > timeSynchronization->adaptiveSampleRejection.outlierUpperBound) {
            timeSynchronization->adaptiveSampleRejection.rejected++;
            isToBeRejected = true;
        } else {
            timeSynchronization->adaptiveSampleRejection.accepted++;
        }
//        }

        __reduceRejectionCounters(&timeSynchronization->adaptiveSampleRejection);
        __updateCurrentRejectionBoundaries(timeSynchronization);

        if (isToBeRejected) {
            LED_STATUS1_TOGGLE;
            return;
        }
    }
#else
#  if defined(SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION)
    if (timeSynchronization->timeIntervalSamples.numSamples >=
        TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {

        calculateVarianceAndStdDeviance(timeSynchronization);
        __updateOutlierRejectionLimitDependingOnSigma(timeSynchronization);

        if (*sample < timeSynchronization->adaptiveSampleRejection.outlierLowerBound ||
            *sample > timeSynchronization->adaptiveSampleRejection.outlierUpperBound) {
            LED_STATUS1_TOGGLE;
            return;
        }

    }
#  endif
#endif

    // add sample to FiFo
    if (timeSynchronization->timeIntervalSamples.numSamples < TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {
        timeSynchronization->timeIntervalSamples.numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(&timeSynchronization->timeIntervalSamples);
    timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.__insertIndex] = *sample;

#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
    // calculate progressive mean
    __calculateProgressiveMean(sample, timeSynchronization);
#endif

#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
    // calculate mean stepwise / on-line
    if (timeSynchronization->timeIntervalSamples.isDropOutValid) {
        // on fifo has dropped out a first-in value
        __calculateMeanUsingFifoInOutObservations(timeSynchronization,
                                                  sample,
                                                  &timeSynchronization->timeIntervalSamples.dropOut);
    } else {
        // on fifo not entirely saturated
        __calculateMeanUsingFifoInObservations(timeSynchronization, sample);
    }
#endif
}
