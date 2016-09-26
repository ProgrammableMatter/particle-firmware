/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related implementation.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "SamplesFifo.h"
#include "LeastSquareRegression.h"

/**
 * Clock skew approximation entry point: Independent on which approximation strategy is chosen,
 * this approximation function is triggered after each TimePackage has been received correctly and
 * it's pdu reception duration offset has been stored to the fifo queue.
 */
void tryApproximateTimings(TimeSynchronization *const timeSynchronization) {
#define __STRATEGY_SIMPLE_MEAN_AND_STDDEVIANCE
//#define __STRATEGY_LEAST_SQUARE_LINEAR_FITTING

    LED_STATUS1_TOGGLE;
    LED_STATUS2_TOGGLE;
    LED_STATUS3_TOGGLE;
#ifdef __STRATEGY_SIMPLE_MEAN_AND_STDDEVIANCE
    if (timeSynchronization->progressiveMean == 0) {
        timeSynchronization->progressiveMean = timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.__insertIndex];
    } else {
        timeSynchronization->progressiveMean +=
                timeSynchronization->timeIntervalSamples.samples[timeSynchronization->timeIntervalSamples.__insertIndex];
        timeSynchronization->progressiveMean /= 2.0;
    }

    calculateMean(timeSynchronization);
    calculateVarianceAndStdDeviance(timeSynchronization);
#else
#  if defined(__STRATEGY_LEAST_SQUARE_LINEAR_FITTING)
    calculateLinearFittingFunction(timeSynchronization);
#  else
#    error no approximation strategy specified
#  endif
#endif
    LED_STATUS1_TOGGLE;
    LED_STATUS2_TOGGLE;
    LED_STATUS3_TOGGLE;
}
