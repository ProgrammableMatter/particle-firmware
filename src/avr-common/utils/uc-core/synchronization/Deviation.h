/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related implementation.
 */

#pragma once

#include "uc-core/configuration/synchronization/Deviation.h"
#include "SynchronizationTypes.h"
#include "Synchronization.h"
//#include "uc-core/stdout/stdio.h"

#ifndef DEVIATION_BINARY_SEARCH_SQRT

#  include <math.h>

#endif


#ifdef DEVIATION_BINARY_SEARCH_SQRT

#define NAN __builtin_nan("")

/**
 * Binary search for sqrt as proposed in:
 * http://www.avrfreaks.net/forum/where-sqrt-routine
 */
static void __binarySearchSqr(const CalculationType *const number, CalculationType *const result) {
#define __binary_sqr_search_digits_accuracy 0.1
//#define __binary_sqr_search_digits_accuracy 0.01
    if ((*number) >= 0) {
        CalculationType left = 0;
        CalculationType right = *number + 1;
        while ((right - left) > __binary_sqr_search_digits_accuracy) {
            *result = (left + right) / 2;
            if ((*result) * (*result) < (*number))
                left = *result;
            else
                right = *result;
        };
        *result = (left + right) / 2;
    } else {
        *result = NAN;
    }
}

#endif

/**
 * Calculates the mean excluding outlier using the current µ and current σ.
 * @pre The arithmetic mean must be valid.
 * @pre The standard deviation must be valid.
 */
void calculateMeanWithoutOutlier(void) {
    TimeSynchronization *const timeSynchronization = &ParticleAttributes.timeSynchronization;
    timeSynchronization->meanWithoutOutlier = 0;
    IndexType numberCumulatedValues = 0;

    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    do {
        SampleValueType sample = timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator].value;
        if (sample >= timeSynchronization->adaptiveSampleRejection.outlierLowerBound &&
            sample <= timeSynchronization->adaptiveSampleRejection.outlierUpperBound) {
            timeSynchronization->meanWithoutOutlier += sample;
            numberCumulatedValues++;
        }
        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    timeSynchronization->meanWithoutOutlier /= (CalculationType) numberCumulatedValues;
}


/**
 * Calculates the mean (off-line) of all available samples in the FiFo.
 * A call to this function involves iterating the whole FiFo.
 */
void calculateMean(void) {
    TimeSynchronization *const timeSynchronization = &ParticleAttributes.timeSynchronization;
    timeSynchronization->mean = 0;
    IndexType numberCumulatedValues = 0;

    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    do {
        SampleValueType sample = timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator].value;
        timeSynchronization->mean = timeSynchronization->mean + sample;
        numberCumulatedValues++;
        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    timeSynchronization->mean = timeSynchronization->mean / numberCumulatedValues;
}

/**
 * Calculates the mean (off-line) of all available samples in the FiFo, and the mean excluding
 * as outlier marked values.
 * A call to this function involves iterating the whole FiFo.
 */
void calculateMeanAndMeanWithoutMarkedOutlier(void) {
    TimeSynchronization *const timeSynchronization = &ParticleAttributes.timeSynchronization;
    timeSynchronization->mean = 0;
    timeSynchronization->meanWithoutMarkedOutlier = 0;
    IndexType numberCumulatedValues = 0;
    IndexType numberCumulatedValuesWithoutOutlier = 0;

    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    do {
        SampleValueType sample = timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator].value;
        if (timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator].isRejected ==
            false) {
            timeSynchronization->meanWithoutMarkedOutlier += sample;
            numberCumulatedValuesWithoutOutlier++;
        }
        timeSynchronization->mean = timeSynchronization->mean + sample;

        numberCumulatedValues++;
        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    timeSynchronization->mean = timeSynchronization->mean / numberCumulatedValues;
    timeSynchronization->meanWithoutMarkedOutlier /= (CalculationType) numberCumulatedValuesWithoutOutlier;
}

/**
 * Calculates the mean of all available samples in the FiFo exclusive marked as outlier.
 * A call to this function involves iterating the whole FiFo.
 */
void calculateMeanWithoutMarkedOutlier(void) {
    TimeSynchronization *const timeSynchronization = &ParticleAttributes.timeSynchronization;
    timeSynchronization->meanWithoutMarkedOutlier = 0;
    IndexType numberCumulatedValuesWithoutOutlier = 0;

    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    do {
        FifoElement sample = timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator];
        if (sample.isRejected == false) {
            timeSynchronization->meanWithoutMarkedOutlier += sample.value;
            numberCumulatedValuesWithoutOutlier++;
        }
        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    timeSynchronization->meanWithoutMarkedOutlier /= (CalculationType) numberCumulatedValuesWithoutOutlier;
}


/**
 * Calculate the variance and std deviance of the values in the fifo buffer.
 * @pre The arithmetic mean must be valid.
 */
void calculateVarianceAndStdDeviance(void) {
    TimeSynchronization *const timeSynchronization = &ParticleAttributes.timeSynchronization;
    timeSynchronization->variance = 0;
    IndexType numberCumulatedValues = 0;

    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    do {
        CalculationType difference = timeSynchronization->mean -
                                     timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator].value;
        timeSynchronization->variance += difference * difference;
        numberCumulatedValues++;
        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    timeSynchronization->variance = timeSynchronization->variance /
                                    (CalculationType) numberCumulatedValues;

#ifdef DEVIATION_MATH_SQRT
    timeSynchronization->stdDeviance = sqrtf(timeSynchronization->variance);
#else
#  if defined(DEVIATION_BINARY_SEARCH_SQRT)
    __binarySearchSqr(&timeSynchronization->variance, &timeSynchronization->stdDeviance);
#  else
#    error sqrt() implementation not specified
#  endif
#endif
}
