/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related implementation.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "uc-core/configuration/synchronization/LeastSquareRegression.h"

#ifdef LEAST_SQUARE_REGRESSION_MATH_SQRT
#  include <math.h>
#endif


#ifdef LEAST_SQUARE_REGRESSION_BINARY_SEARCH_SQRT

#define NAN __builtin_nan("")


/**
 * Binary search for sqrt as proposed in:
 * http://www.avrfreaks.net/forum/where-sqrt-routine
 */
static void binarySearchSqr(const float *const number, float *const result) {
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
 * Calculating Linear Least Squares fitting function for measured values:
 * https://en.wikipedia.org/wiki/Linear_least_squares_(mathematics)#Orthogonal_decomposition_methods
 * The output (fitting function and several statistical values) are stored to the SamplesFifoBuffer.
 */
void calculateLinearFittingFunction(SamplesFifoBuffer *const measurements) {
    measurements->isCalculationValid = false;

    CumulationType a_ = 0, b_ = 0, c_ = 0, e_ = 0;
    SampleValueType y = 0;
    IndexType x = 1;
    measurements->mean = 0;

    samplesFifoBufferIteratorStart(measurements);
    do {
        y = measurements->samples[measurements->iterator];
        // a_ += 2 * x * x;
        a_ += x * x;
        // b_ += 2 * x * y;
        b_ += x * y;
        // c_ += 2 * x;
        c_ += x;
        // d_ += 2; --> same as numSamples * 2
        e_ += y;
        // f += 2 * x; --> f == c

        // pulled out 2-factor: numSamples * 2
        measurements->mean += y;
        x++;

        samplesFifoBufferFiFoBufferIteratorNext(measurements);
    } while (measurements->iterator < TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    a_ *= 2;
    b_ *= 2;
    c_ *= 2;
    uint8_t d_ = measurements->numSamples * 2;
    e_ *= 2;
    CumulationType *f_ = &c_;

    measurements->mean /= measurements->numSamples;

    samplesFifoBufferIteratorStart(measurements);
    do {
        y = measurements->samples[measurements->iterator];
        CalculationType difference;
        if (y >= measurements->mean) {
            difference = y - measurements->mean;
        } else {
            difference = measurements->mean - y;
        }

        measurements->variance += difference * difference;

        samplesFifoBufferFiFoBufferIteratorNext(measurements);
    } while (measurements->iterator < TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);

    measurements->variance /= measurements->numSamples;
    // we obtain two linear equations from the transformed partial derivatives to d and k
    // with k explicitly
    measurements->k = (d_ * b_ - c_ * e_) / (CalculationType) (d_ * a_ - (*f_) * c_);
    // and d explicitly
    measurements->d = (-measurements->k * a_ + b_) / c_;

    //  standard deviance as sqrt(variance)
#ifdef LEAST_SQUARE_REGRESSION_MATH_SQRT
    measurements->stdDeviance = sqrt(measurements->d);
#else
#  if defined(LEAST_SQUARE_REGRESSION_BINARY_SEARCH_SQRT)
    binarySearchSqr(&measurements->d, &measurements->stdDeviance);
#  else
#    error sqrt implementation not specified
#  endif
#endif

    measurements->isCalculationValid = true;
}