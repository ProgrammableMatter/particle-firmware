/**
 * @author Raoul Rubien 23.09.2016
 *
 * Linear Least Squares Regression related implementation.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "Deviation.h"

/**
 * Calculating Linear Least Squares fitting function for measured values:
 * https://en.wikipedia.org/wiki/Linear_least_squares_(mathematics)#Orthogonal_decomposition_methods
 * The output (fitting function and several statistical values) are stored to the SamplesFifoBuffer.
 * The arithmetic mean is calculated in the same pass followed by a second std deviance pass.
 */
void calculateLinearFittingFunctionVarianceAndStdDeviance(TimeSynchronization *const timeSynchronization) {
    timeSynchronization->fittingFunction.isCalculationValid = false;
    MEMORY_BARRIER;

//    if (false == __isEnoughDataAvailable(timeSynchronization)) {
//        return;
//    }

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

    timeSynchronization->mean =
            timeSynchronization->mean / (CalculationType) timeSynchronization->timeIntervalSamples.numSamples;
    calculateVarianceAndStdDeviance(timeSynchronization);

    // we obtain two linear equations from the transformed partial derivatives to d and k
    // with k explicitly
    timeSynchronization->fittingFunction.k = (d_ * b_ - c_ * e_) / (CalculationType) (d_ * a_ - (*f_) * c_);
    // and d explicitly
    timeSynchronization->fittingFunction.d = (timeSynchronization->fittingFunction.k * a_ + b_) / c_;

    MEMORY_BARRIER;
    timeSynchronization->fittingFunction.isCalculationValid = true;
}