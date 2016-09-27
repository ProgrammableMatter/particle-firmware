/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related implementation.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "uc-core/configuration/synchronization/LeastSquareRegression.h"
#include "Synchronization.h"

#ifdef LEAST_SQUARE_REGRESSION_MATH_SQRT
#  include <math.h>
#endif


#ifdef LEAST_SQUARE_REGRESSION_BINARY_SEARCH_SQRT

#define NAN __builtin_nan("")

/**
 * Binary search for sqrt as proposed in:
 * http://www.avrfreaks.net/forum/where-sqrt-routine
 */
static void __binarySearchSqr(const float *const number, float *const result) {
#define __binary_sqr_search_digits_accuracy 0.1
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

/**
 * @return true if enough data is queued for calculations
 */
static bool __isEnoughDataAvailable(const TimeSynchronization *const timeSynchronization) {
    // on too less samples calculation is not performed
    if (timeSynchronization->timeIntervalSamples.numSamples < (TIME_SYNCHRONIZATION_MINIMUM_SAMPLES - 1)) {
        return false;
    } else {
        return true;
    }
}

void calculateMean(TimeSynchronization *const timeSynchronization) {
    if (false == __isEnoughDataAvailable(timeSynchronization)) {
        return;
    }
    CumulationType sum = 0;
    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    do {
        sum += timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator];
        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);
    timeSynchronization->mean = sum / (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;
}

/**
 * Calculate the variance and std deviance of the values in the fifo buffer.
 * @pre The arithmetic mean must be valid.
 */
void calculateVarianceAndStdDeviance(TimeSynchronization *const timeSynchronization) {
    timeSynchronization->isCalculationValid = false;
    MEMORY_BARRIER;

    if (false == __isEnoughDataAvailable(timeSynchronization)) {
        return;
    }

    SampleValueType y = 0;
    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    // @pre timeSynchronization->mean is valid
    do {
        y = timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator];
        CalculationType difference;
        if (y >= timeSynchronization->mean) {
            difference = y - timeSynchronization->mean;
        } else {
            difference = timeSynchronization->mean - y;
        }

        timeSynchronization->variance += difference * difference;

        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    timeSynchronization->variance /= timeSynchronization->timeIntervalSamples.numSamples;

    //  standard deviance as sqrt(variance)
#ifdef LEAST_SQUARE_REGRESSION_MATH_SQRT
    measurements->stdDeviance = sqrt(timeSynchronization->variance);
#else
#  if defined(LEAST_SQUARE_REGRESSION_BINARY_SEARCH_SQRT)
    __binarySearchSqr(&timeSynchronization->variance, &timeSynchronization->stdDeviance);
#  else
#    error sqrt implementation not specified
#  endif
#endif

    MEMORY_BARRIER;
    timeSynchronization->isCalculationValid = true;
}

/**
 * Calculating Linear Least Squares fitting function for measured values:
 * https://en.wikipedia.org/wiki/Linear_least_squares_(mathematics)#Orthogonal_decomposition_methods
 * The output (fitting function and several statistical values) are stored to the SamplesFifoBuffer.
 * The arithmetic mean is calculated in the same pass followed by a second std deviance pass.
 */
void calculateLinearFittingFunction(TimeSynchronization *const timeSynchronization) {
    timeSynchronization->fittingFunction.isCalculationValid = false;
    MEMORY_BARRIER;

    if (false == __isEnoughDataAvailable(timeSynchronization)) {
        return;
    }

    CumulationType a_ = 0, b_ = 0, c_ = 0, e_ = 0;
    SampleValueType y = 0;
    IndexType x = 1;
    timeSynchronization->mean = 0;

    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    do {
        y = timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator];
        // a_ += 2 * x * x;
        a_ += x * x;
        // b_ += 2 * x * y;
        b_ += x * y;
        // c_ += 2 * x;
        c_ += x;
        // d_ += 2; --> same as numSamples * 2
        e_ += y;
        // f += 2 * x; --> f == c

        // 2-factor pulled our of loop means each factor *= 2
        timeSynchronization->mean += y;
        x++;

        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
    } while (timeSynchronization->timeIntervalSamples.iterator <
             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    a_ *= 2;
    b_ *= 2;
    c_ *= 2;
    uint8_t d_ = timeSynchronization->timeIntervalSamples.numSamples * 2;
    e_ *= 2;
    CumulationType *f_ = &c_;

    timeSynchronization->mean /= timeSynchronization->timeIntervalSamples.numSamples;
    calculateVarianceAndStdDeviance(timeSynchronization);

    // we obtain two linear equations from the transformed partial derivatives to d and k
    // with k explicitly
    timeSynchronization->fittingFunction.k = (d_ * b_ - c_ * e_) / (CalculationType) (d_ * a_ - (*f_) * c_);
    // and d explicitly
    timeSynchronization->fittingFunction.d = (timeSynchronization->fittingFunction.k * a_ + b_) / c_;

    MEMORY_BARRIER;
    timeSynchronization->fittingFunction.isCalculationValid = true;
}