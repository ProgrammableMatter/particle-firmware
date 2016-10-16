/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo related implementation.
 */

#pragma once

#include "uc-core/configuration/synchronization/SampleFifoTypes.h"
#include "SynchronizationTypes.h"
#include <math.h>
#include "uc-core/stdout/stdio.h"

bool isFiFoFull(const SamplesFifoBuffer *const samplesFifoBuffer) {
    return samplesFifoBuffer->numSamples >= SAMPLE_FIFO_NUM_BUFFER_ELEMENTS;
}

/**
 * circular-increment an index with respect to the fifo buffer boundaries
 */
static void __samplesFifoBufferIncrementIndex(IndexType *const index) {
    if (*index < (SAMPLE_FIFO_NUM_BUFFER_ELEMENTS - 1)) {
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

#if !defined(SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN) && !defined(SYNCHRONIZATION_STRATEGY_RAW_OBSERVATION)

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

#endif

#if defined(SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION) \
 && (defined(SYNCHRONIZATION_STRATEGY_MEAN) || defined(SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_OUTLIER))

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
            (CalculationType) SAMPLE_FIFO_NUM_BUFFER_ELEMENTS;

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
static void __calculateProgressiveMean(const SampleValueType *const sample,
                                       TimeSynchronization *const timeSynchronization) {
    if (timeSynchronization->progressiveMean == 0) {
        timeSynchronization->progressiveMean = *sample;
    } else {
        CalculationType newProgressiveMean =
                (CalculationType) ((CalculationType) *sample +
                 (CalculationType) timeSynchronization->progressiveMean) / 2.0;
        timeSynchronization->progressiveMean = (SampleValueType) roundf(newProgressiveMean);
    }
    // workaround
    timeSynchronization->timeIntervalSamples.numSamples = SAMPLE_FIFO_NUM_BUFFER_ELEMENTS;
}

#endif

#if defined(SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_OUTLIER)

/**
 * Update the outlier limits.
 */
void updateOutlierRejectionLimitDependingOnSigma(void) {
    TimeSynchronization *const timeSynchronization = &ParticleAttributes.timeSynchronization;
    CalculationType rejectionLimit =
            SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR *
            timeSynchronization->stdDeviance;
    timeSynchronization->adaptiveSampleRejection.outlierLowerBound =
            roundf(timeSynchronization->mean - rejectionLimit);
    timeSynchronization->adaptiveSampleRejection.outlierUpperBound =
            roundf(timeSynchronization->mean + rejectionLimit);
    timeSynchronization->adaptiveSampleRejection.isOutlierRejectionBoundValid = true;
}


#endif

#ifdef SYNCHRONIZATION_ENABLE_ADAPTIVE_MARKED_OUTLIER_REJECTION

static void __reduceRejectionCounters(void) {
   AdaptiveSampleRejection *const adaptiveSampleRejection =
   &ParticleAttributes.timeSynchronization.adaptiveSampleRejection;

    if ((adaptiveSampleRejection->rejected >= SAMPLE_FIFO_ADAPTIVE_REJECTION_REDUCE_COUNTERS_LIMIT) ||
        (adaptiveSampleRejection->accepted >= SAMPLE_FIFO_ADAPTIVE_REJECTION_REDUCE_COUNTERS_LIMIT)) {
        adaptiveSampleRejection->rejected /= 2;
        adaptiveSampleRejection->accepted /= 2;
    }

    if (adaptiveSampleRejection->accepted <= 0 || adaptiveSampleRejection->rejected <= 0) {
        adaptiveSampleRejection->accepted++;
        adaptiveSampleRejection->rejected++;
    }
}

static void __updateCurrentRejectionBoundariesDependingOnCounters(void) {
    AdaptiveSampleRejection *const adaptiveSampleRejection =
    &ParticleAttributes.timeSynchronization->adaptiveSampleRejection;
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
        }
//        else {
//            LED_STATUS2_TOGGLE;
//        }
    } else if ((adaptiveSampleRejection->accepted +
                SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_INTERVAL_THRESHOLD) <
               (SAMPLE_FIFO_ADAPTIVE_REJECTION_ACCEPTANCE_RATIO * adaptiveSampleRejection->rejected)) {
        // on accepted below limit, increase rejection interval
        adaptiveSampleRejection->currentAcceptedDeviation += SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_STEP;
        if (adaptiveSampleRejection->currentAcceptedDeviation >=
            SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MAX_INTERVAL) {
            adaptiveSampleRejection->currentAcceptedDeviation = SAMPLE_FIFO_ADAPTIVE_REJECTION_UPDATE_REJECTION_MAX_INTERVAL;
//            LED_STATUS3_ON;
        }
//         else {
//            LED_STATUS3_TOGGLE;
//        }
    }
//    else {
//        LED_STATUS3_TOGGLE;
//        LED_STATUS2_TOGGLE;
//    }

    // update new rejection boundaries
    adaptiveSampleRejection->outlierLowerBound =
            (SampleValueType) roundf(timeSynchronization->mean - (CalculationType)
                    adaptiveSampleRejection->currentAcceptedDeviation);
    adaptiveSampleRejection->outlierUpperBound =
            (SampleValueType) roundf(timeSynchronization->mean +
                                     (CalculationType) adaptiveSampleRejection->currentAcceptedDeviation);
    adaptiveSampleRejection->isOutlierRejectionBoundValid = true;
}

#include "Deviation.h"

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample) {
    TimeSynchronization *const timeSynchronization = &ParticleAttributes.timeSynchronization;
    bool isToBeRejected = false;
    // on overfull start rejecting on overfull FiFo
    if (timeSynchronization->timeIntervalSamples.isDropOutValid) {
        if (*sample < timeSynchronization->adaptiveSampleRejection.outlierLowerBound ||
            *sample > timeSynchronization->adaptiveSampleRejection.outlierUpperBound) {
            timeSynchronization->adaptiveSampleRejection.rejected++;
            isToBeRejected = true;
        } else {
            timeSynchronization->adaptiveSampleRejection.accepted++;
        }

        __reduceRejectionCounters(&timeSynchronization->adaptiveSampleRejection);
        // reject means: sample does not enter the queue
        if (isToBeRejected) {
            return;
        }
    }

    // add sample to FiFo
    if (timeSynchronization->timeIntervalSamples.numSamples <
        SAMPLE_FIFO_NUM_BUFFER_ELEMENTS) {
        timeSynchronization->timeIntervalSamples.numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(&timeSynchronization->timeIntervalSamples);
    timeSynchronization->timeIntervalSamples.
            samples[timeSynchronization->timeIntervalSamples.__insertIndex].value = *sample;
    timeSynchronization->timeIntervalSamples.
            samples[timeSynchronization->timeIntervalSamples.__insertIndex].isRejected = isToBeRejected;


#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
    // calculate mean stepwise / on-line
    if (timeSynchronization->timeIntervalSamples.isDropOutValid) {
        // on fifo has dropped out a first-in value
        __calculateMeanUsingFifoInOutObservations(timeSynchronization,
                                                  &timeSynchronization->timeIntervalSamples.
                                                          samples[timeSynchronization->timeIntervalSamples.__insertIndex],
                                                  &timeSynchronization->timeIntervalSamples.dropOut);
    } else {
        // on fifo not entirely saturated
        __calculateMeanUsingFifoInObservations(timeSynchronization,
                                               &timeSynchronization->timeIntervalSamples.
                                                       samples[timeSynchronization->timeIntervalSamples.__insertIndex]);
    }
#else
    // off-line mean calculation
    if (isFiFoFull(&timeSynchronization->timeIntervalSamples)) {
        calculateMean();
    }

    // on full FiFo update the rejection boundaries
    if (isFiFoFull(&timeSynchronization->timeIntervalSamples)) {
        __updateCurrentRejectionBoundariesDependingOnCounters();
    }
#endif
}

#endif


#ifdef SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_MARKED_OUTLIER

#include "Deviation.h"

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample,
                                TimeSynchronization *const timeSynchronization) {
    bool isToBeRejected = false;

    if (isFiFoFull(&timeSynchronization->timeIntervalSamples) &&
        timeSynchronization->adaptiveSampleRejection.isOutlierRejectionBoundValid) {
        if (*sample < timeSynchronization->adaptiveSampleRejection.outlierLowerBound ||
            *sample > timeSynchronization->adaptiveSampleRejection.outlierUpperBound) {
            // reject means: marking the value which enters the queue as outlier
            // according to current mean/deviation
            isToBeRejected = true;
        }
    }

    // add sample to FiFo
    if (timeSynchronization->timeIntervalSamples.numSamples <
        SAMPLE_FIFO_NUM_BUFFER_ELEMENTS) {
        timeSynchronization->timeIntervalSamples.numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(&timeSynchronization->timeIntervalSamples);
    timeSynchronization->timeIntervalSamples.
            samples[timeSynchronization->timeIntervalSamples.__insertIndex].value = *sample;
    timeSynchronization->timeIntervalSamples.
            samples[timeSynchronization->timeIntervalSamples.__insertIndex].isRejected = isToBeRejected;


    // update mean exclusive marked outlier
    calculateMeanWithoutMarkedOutlier();
}

#endif


#if defined(SYNCHRONIZATION_STRATEGY_MEAN) \
 || defined(SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_OUTLIER)

#include "Deviation.h"

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample,
                                TimeSynchronization *const timeSynchronization) {
    // add any sample to FiFo regardless of outlier
    if (timeSynchronization->timeIntervalSamples.numSamples <
        SAMPLE_FIFO_NUM_BUFFER_ELEMENTS) {
        timeSynchronization->timeIntervalSamples.numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(&timeSynchronization->timeIntervalSamples);
    timeSynchronization->timeIntervalSamples.
            samples[timeSynchronization->timeIntervalSamples.__insertIndex].value = *sample;
//    timeSynchronization->timeIntervalSamples.
//            samples[timeSynchronization->timeIntervalSamples.__insertIndex].isRejected = false;

#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
    // calculate mean stepwise / on-line
    if (timeSynchronization->timeIntervalSamples.isDropOutValid) {
        // on fifo has dropped out a first-in value
        __calculateMeanUsingFifoInOutObservations(timeSynchronization,
                                                  &timeSynchronization->timeIntervalSamples.
                                                          samples[timeSynchronization->timeIntervalSamples.__insertIndex],
                                                  &timeSynchronization->timeIntervalSamples.dropOut);
    } else {
        // on fifo not entirely saturated
        __calculateMeanUsingFifoInObservations(timeSynchronization,
                                               &timeSynchronization->timeIntervalSamples.
                                                       samples[timeSynchronization->timeIntervalSamples.__insertIndex]);
    }
#else
    // off-line mean calculation
    if (isFiFoFull(&timeSynchronization->timeIntervalSamples)) {
        calculateMean();
    }
#endif
}

#endif


#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN

/**
 * Does not add any value to the FiFo buffer but calculates a very naive mean immediately.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample,
                                TimeSynchronization *const timeSynchronization) {
    // very naive implementation used for evaluation/comparison
    __calculateProgressiveMean(sample, timeSynchronization);
}

#endif

#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample,
                                TimeSynchronization *const timeSynchronization) {
    // add sample to FiFo
    if (timeSynchronization->timeIntervalSamples.numSamples <
        SAMPLE_FIFO_NUM_BUFFER_ELEMENTS) {
        timeSynchronization->timeIntervalSamples.numSamples++;
    }
    __samplesFifoBufferIncrementInsertIndex(&timeSynchronization->timeIntervalSamples);
    timeSynchronization->timeIntervalSamples.
            samples[timeSynchronization->timeIntervalSamples.__insertIndex].value = *sample;
//    timeSynchronization->timeIntervalSamples.
//            samples[timeSynchronization->timeIntervalSamples.__insertIndex].isRejected = isToBeRejected;
//    printf("%u\n", *sample);
}

#endif

#ifdef SYNCHRONIZATION_STRATEGY_RAW_OBSERVATION

/**
 * Adds a value to the FiFo buffer.
 */
void samplesFifoBufferAddSample(const SampleValueType *const sample,
                                TimeSynchronization *const timeSynchronization) {
    // very naive implementation: use current observation as mean
    timeSynchronization->mean = *sample;
    // workaround
    timeSynchronization->timeIntervalSamples.numSamples = SAMPLE_FIFO_NUM_BUFFER_ELEMENTS;
}

#endif