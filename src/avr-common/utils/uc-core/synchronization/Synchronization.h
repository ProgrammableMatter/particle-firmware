/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related implementation.
 */

#pragma once

#include "uc-core/configuration/synchronization/Synchronization.h"
#include "SynchronizationTypes.h"
#include "SamplesFifo.h"

#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING
#  include "LeastSquareRegression.h"
#endif
#ifdef SYNCHRONIZATION_STRATEGY_SIMPLE_MEAN_AND_STDDEVIANCE

#  include "Deviation.h"

#endif

#include "uc-core/configuration/Time.h"


static void __roundFloatToUnsigned(volatile const float *const source, volatile uint16_t *const result) {
    if ((*source >= 0 - 0.5) && (*source <= UINT16_MAX + 0.5)) {
        *result = (uint16_t) (*source + 0.5);
        MEMORY_BARRIER;
        ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
    } else {
        *result = 0;
    }
}

/**
 * Clock skew approximation entry point: Independent on which approximation strategy is chosen,
 * this approximation function is triggered after each TimePackage has been received correctly and
 * it's pdu reception duration offset has been stored to the fifo queue.
 */
void tryApproximateTimings(TimeSynchronization *const timeSynchronization) {
    if (__isEnoughDataAvailable(timeSynchronization)) {
        if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable == false) {

#ifdef SYNCHRONIZATION_STRATEGY_SIMPLE_MEAN_AND_STDDEVIANCE
            calculateMean(timeSynchronization);
            calculateVarianceAndStdDeviance(timeSynchronization);
#else
#  if defined(SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING)
            calculateLinearFittingFunctionVarianceAndStdDeviance(timeSynchronization);
#  else
#    error no approximation strategy specified
#  endif
#endif
            float newTimePeriodInterruptDelay;
            if (ParticleAttributes.timeSynchronization.mean > TIME_SYNCHRONIZATION_SAMPLE_OFFSET) {
                newTimePeriodInterruptDelay = LOCAL_TIME_DEFAULT_INTERRUPT_DELAY +
                                              LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
                                              (ParticleAttributes.timeSynchronization.mean -
                                               TIME_SYNCHRONIZATION_SAMPLE_OFFSET);
            } else {
                newTimePeriodInterruptDelay = LOCAL_TIME_DEFAULT_INTERRUPT_DELAY -
                                              LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
                                              (TIME_SYNCHRONIZATION_SAMPLE_OFFSET -
                                               ParticleAttributes.timeSynchronization.mean);
            }
            __roundFloatToUnsigned(&newTimePeriodInterruptDelay,
                                   &ParticleAttributes.localTime.newTimePeriodInterruptDelay);
            ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
        }
    }
}
