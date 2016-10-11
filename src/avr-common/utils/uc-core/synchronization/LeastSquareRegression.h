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
    CumulationType a_ = 0, b_ = 0, c_ = 0, e_ = 0;
    SampleValueType y = 0;
    IndexType x = 1;

//    // TODO: evaluation code
//    CLI;
//    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
//    do {
//        printf("%u,\n",
//               timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator].value);
//        samplesFifoBufferFiFoBufferIteratorNext(&timeSynchronization->timeIntervalSamples);
//    } while (timeSynchronization->timeIntervalSamples.iterator <
//             TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_ITERATOR_END);


    samplesFifoBufferIteratorStart(&timeSynchronization->timeIntervalSamples);
    do {
        y = timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.iterator].value;
        // printf("d%u\n", y);
        // a_ += 2 * x * x;
        a_ += (CumulationType) x * x;

        // b_ += 2 * x * y;
        b_ += (CumulationType) x * y;
        // c_ += 2 * x;
        c_ += x;
        // d_ += 2; --> same as numSamples * 2
        e_ += y;
        // f += 2 * x; --> f == c

        // 2-factor pulled our of loop means each factor *= 2
        // TODO: evaluation code
        // printf("[%u] --> a %lu b %lu c %lu e %lu\n", x, a_, b_, c_, e_);
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
    // TODO: evaluation code
    // printf("a%lu b%lu c%lu d%u e%lu f%lu\n", a_, b_, c_, d_, e_, *f_);

    // we obtain two linear equations from the transformed partial derivatives to d and k
    // with k explicitly
    timeSynchronization->fittingFunction.k =
            ((CalculationType) d_ * b_ - (CalculationType) c_ * e_) / (d_ * a_ - (*f_) * c_);
    // and d explicitly
    timeSynchronization->fittingFunction.d = (timeSynchronization->fittingFunction.k * a_ + b_) / c_;

    // TODO: evaluation code
    // printf("k%li d%li\n", (int32_t) timeSynchronization->fittingFunction.k,
    //   (int32_t) timeSynchronization->fittingFunction.d);
}