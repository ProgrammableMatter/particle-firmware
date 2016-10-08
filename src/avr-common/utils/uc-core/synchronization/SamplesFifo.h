/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo related implementation.
 */

#pragma once

#include "uc-core/configuration/synchronization/SampleFifoTypes.h"
#include "SynchronizationTypes.h"
#include <math.h>

bool isFiFoFull(const SamplesFifoBuffer *const samplesFifoBuffer) {
    return samplesFifoBuffer->numSamples >= TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE;
}

/**
 * circular-increment an index with respect to the fifo buffer boundaries
 */
static void __samplesFifoBufferIncrementIndex(IndexType *const index) {
    if (*index < (TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE - 1)) {
        (*index)++;
    } else {
        *index = 0;
    }
}

/**
 * Increments the iterator until the iterator reaches the start position.
 * If the position equals TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END
 * the buffer's end as been reached. The end is the first element before start. The iterator
 * is not aware of the number of elements stored.
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

            if (samplesBuffer->__isPreDropOutValid == true) {
                samplesBuffer->dropOut.value = samplesBuffer->samples[samplesBuffer->__startIdx].value;
                samplesBuffer->dropOut.isRejected = samplesBuffer->samples[samplesBuffer->__startIdx].isRejected;
                samplesBuffer->isDropOutValid = true;
            } else {
                samplesBuffer->__isPreDropOutValid = true;
            }

            __samplesFifoBufferIncrementIndex(&samplesBuffer->__startIdx);
        }
    }
}

///**
// * @return true if enough data is queued for calculations
// */
//bool isEnoughFifoDataAvailable(const TimeSynchronization *const timeSynchronization) {
//    // if too less samples calculation is not performed
//    if (timeSynchronization->timeIntervalSamples.numSamples < (TIME_SYNCHRONIZATION_MINIMUM_SAMPLES - 1)) {
//        return false;
//    } else {
//        return true;
//    }
//}

#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION

/**
 * Updates the mean of all available samples using only observations of incoming and outgoing FiFo values.
 * One call to this function does not iterate the whole FiFo.
 * An updated is performed if the TIME_SYNCHRONIZATION_MINIMUM_SAMPLES limit is exceeded.
 */
static void __calculateMeanUsingFifoInOutObservations(TimeSynchronization *const timeSynchronization,
                                                      const FifoElement *const fifoIn,
                                                      const FifoElement *const fifoOut) {

    // consider mean with rejected outlier
    if (fifoIn->isRejected == false) {
        timeSynchronization->__unnormalizedCumulativeMeanWithoutMarkedOutlier += (CumulationType) fifoIn->value;

//        // TODO: evaluation code
//        if (timeSynchronization->__numberCumulatedValuesWithoutMarkedOutlier >= UINT16_MAX) {
//            blinkParityErrorForever(&ParticleAttributes.alerts, 1);
//        }
        timeSynchronization->__numberCumulatedValuesWithoutMarkedOutlier++;
    }

    // consider mean with rejected outlier
    if (fifoOut->isRejected == false) {
        timeSynchronization->__unnormalizedCumulativeMeanWithoutMarkedOutlier -= (CumulationType) fifoOut->value;

//        // TODO: evaluation code
//        if (timeSynchronization->__numberCumulatedValuesWithoutMarkedOutlier == 0) {
//            blinkParityErrorForever(&ParticleAttributes.alerts, 0);
//        }
        timeSynchronization->__numberCumulatedValuesWithoutMarkedOutlier--;
    }

    // consider mean
    timeSynchronization->__unnormalizedCumulativeMean += (CumulationType) fifoIn->value;
    timeSynchronization->__unnormalizedCumulativeMean -= (CumulationType) fifoOut->value;
//
//    // TODO: evaluation code
//    if (timeSynchronization->__unnormalizedCumulativeMean >= (UINT32_MAX / 2)) {
//        blinkParityErrorForever(&ParticleAttributes.alerts, 0);
//    }

    // mean
    timeSynchronization->mean =
            (CalculationType) timeSynchronization->__unnormalizedCumulativeMean /
            (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;

    // mean without outlier
    timeSynchronization->meanWithoutMarkedOutlier =
            (CalculationType) timeSynchronization->__unnormalizedCumulativeMeanWithoutMarkedOutlier /
            (CalculationType) timeSynchronization->__numberCumulatedValuesWithoutMarkedOutlier;
}

/**
 * Updates the mean of all available samples using only observations of incoming and outgoing FiFo values.
 * One call to this function does not iterate the whole FiFo.
 * An updated is performed if the TIME_SYNCHRONIZATION_MINIMUM_SAMPLES limit is exceeded.
 */
static void __calculateMeanUsingFifoInObservations(TimeSynchronization *const timeSynchronization,
                                                   const FifoElement *const fifoIn) {
    // consider mean without outlier
    if (fifoIn->isRejected == false) {
        timeSynchronization->__unnormalizedCumulativeMeanWithoutMarkedOutlier += fifoIn->value;
        timeSynchronization->__numberCumulatedValuesWithoutMarkedOutlier++;
    }

    // consider mean
    timeSynchronization->__unnormalizedCumulativeMean += (CumulationType) fifoIn->value;

    // mean
    timeSynchronization->mean =
            (CalculationType) timeSynchronization->__unnormalizedCumulativeMean /
            (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;

    // mean without outlier
    timeSynchronization->meanWithoutMarkedOutlier =
            (CalculationType) timeSynchronization->__unnormalizedCumulativeMeanWithoutMarkedOutlier /
            (CalculationType) timeSynchronization->__numberCumulatedValuesWithoutMarkedOutlier;
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

//#ifdef SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION

/**
 * Update the outlier limits.
 */
static void __updateOutlierRejectionLimitDependingOnSigma(TimeSynchronization *const timeSynchronization) {
    CalculationType rejectionLimit =
            SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR *
            timeSynchronization->stdDeviance;
    timeSynchronization->adaptiveSampleRejection.outlierLowerBound =
            roundf(timeSynchronization->mean - rejectionLimit);
    timeSynchronization->adaptiveSampleRejection.outlierUpperBound =
            roundf(timeSynchronization->mean + rejectionLimit);
    timeSynchronization->adaptiveSampleRejection.isOutlierRejectionBoundValid = true;
}

//#endif

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

static void __updateCurrentRejectionBoundariesDependingOnCounters(TimeSynchronization *const timeSynchronization) {
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
            LED_STATUS2_ON;
        } else {
            LED_STATUS2_TOGGLE;
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
            LED_STATUS3_TOGGLE;
        }
    } else {
        LED_STATUS3_TOGGLE;
        LED_STATUS2_TOGGLE;
    }

    // update new rejection boundaries
    adaptiveSampleRejection->outlierLowerBound =
            (SampleValueType) roundf(timeSynchronization->mean - (CalculationType)
                               adaptiveSampleRejection->currentAcceptedDeviation);
    adaptiveSampleRejection->outlierUpperBound =
            (SampleValueType) roundf(timeSynchronization->mean +
                               (CalculationType) adaptiveSampleRejection->currentAcceptedDeviation);
    adaptiveSampleRejection->isOutlierRejectionBoundValid = true;
}

#endif

#include "Deviation.h"

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample,
                                TimeSynchronization *const timeSynchronization) {
    bool isToBeRejected = false;
//    if (isFiFoFull(&timeSynchronization->timeIntervalSamples) &&
//        timeSynchronization->adaptiveSampleRejection.isOutlierRejectionBoundValid) {
//
////        calculateVarianceAndStdDeviance(timeSynchronization);
////        __updateOutlierRejectionLimitDependingOnSigma(timeSynchronization);
//
//        if (*sample < timeSynchronization->adaptiveSampleRejection.outlierLowerBound ||
//            *sample > timeSynchronization->adaptiveSampleRejection.outlierUpperBound) {
//            timeSynchronization->adaptiveSampleRejection.rejected++;
//            isToBeRejected = true;
//        } else {
//            timeSynchronization->adaptiveSampleRejection.accepted++;
//        }
//
////        __reduceRejectionCounters(&timeSynchronization->adaptiveSampleRejection);
////        __updateCurrentRejectionBoundariesDependingOnCounters(timeSynchronization);
//
//        if (isToBeRejected) {
//            LED_STATUS1_TOGGLE;
//        }
//    }


    // add sample to FiFo
    if (timeSynchronization->timeIntervalSamples.numSamples <
        TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {
        timeSynchronization->timeIntervalSamples.numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(&timeSynchronization->timeIntervalSamples);
    timeSynchronization->timeIntervalSamples.
            samples[timeSynchronization->timeIntervalSamples.__insertIndex].value = *sample;
    timeSynchronization->timeIntervalSamples.
            samples[timeSynchronization->timeIntervalSamples.__insertIndex].isRejected = isToBeRejected;

//#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
//    // calculate progressive mean
//    __calculateProgressiveMean(sample, timeSynchronization);
//#endif

//#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
//    // calculate mean stepwise / on-line
//    if (timeSynchronization->timeIntervalSamples.isDropOutValid) {
//        // on fifo has dropped out a first-in value
//        __calculateMeanUsingFifoInOutObservations(timeSynchronization,
//                                                  &timeSynchronization->timeIntervalSamples.
//                                                          samples[timeSynchronization->timeIntervalSamples.__insertIndex],
//                                                  &timeSynchronization->timeIntervalSamples.dropOut);
//    } else {
//        // on fifo not entirely saturated
//        __calculateMeanUsingFifoInObservations(timeSynchronization,
//                                               &timeSynchronization->timeIntervalSamples.
//                                                       samples[timeSynchronization->timeIntervalSamples.__insertIndex]);
//    }
//#endif

    if (isFiFoFull(&timeSynchronization->timeIntervalSamples)) {
        LED_STATUS3_TOGGLE;
        calculateMean(timeSynchronization);
        calculateVarianceAndStdDeviance(timeSynchronization);
        __updateOutlierRejectionLimitDependingOnSigma(timeSynchronization);
    }
}
