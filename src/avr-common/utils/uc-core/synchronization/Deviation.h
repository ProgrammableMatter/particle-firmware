/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related implementation.
 */

#pragma once

#include "uc-core/configuration/synchronization/Deviation.h"
#include "SynchronizationTypes.h"
#include "Synchronization.h"

#ifndef DEVIATION_BINARY_SEARCH_SQRT
#  include <math.h>
#endif


#ifdef DEVIATION_BINARY_SEARCH_SQRT

#define NAN __builtin_nan("")

/**
 * Binary search for sqrt as proposed in:
 * http://www.avrfreaks.net/forum/where-sqrt-routine
 */
static void __binarySearchSqr(const float *const number, float *const result) {
#define __binary_sqr_search_digits_accuracy 0.1
//#define __binary_sqr_search_digits_accuracy 0.01
    if ((*number) >= 0) {
        float left = 0;
        float right = *number + 1;
//        float *center = result;
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

///**
// * Calculates the mean of all available samples in the FiFo.
// * A call to this function involves iterating the whole FiFo.
// */
//void calculateMean(TimeSynchronization *const timeSynchronization) {
//    CumulationType sum = 0;
//    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
//    do {
//        sum += timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator];
//        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
//    } while (timeSynchronization->timeIntervalSamples.iterator <
//             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);
//    timeSynchronization->mean = sum / (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;
//}

/**
 * Calculate the variance and std deviance of the values in the fifo buffer.
 * @pre The arithmetic mean must be valid.
 */
void calculateVarianceAndStdDeviance(TimeSynchronization *const timeSynchronization) {
    timeSynchronization->isCalculationValid = false;
    MEMORY_BARRIER;

    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    // @pre timeSynchronization->mean is valid
    do {
        CalculationType difference = timeSynchronization->mean -
                                     timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator];
        timeSynchronization->variance += difference * difference;
        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    timeSynchronization->variance /= timeSynchronization->timeIntervalSamples.numSamples;

#ifdef DEVIATION_MATH_SQRT
    measurements->stdDeviance = sqrt(timeSynchronization->variance);
#else
#  if defined(DEVIATION_BINARY_SEARCH_SQRT)
    __binarySearchSqr(&timeSynchronization->variance, &timeSynchronization->stdDeviance);
#  else
#    error sqrt implementation not specified
#  endif
#endif

    MEMORY_BARRIER;
    timeSynchronization->isCalculationValid = true;
}
