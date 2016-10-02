/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related implementation.
 */

#pragma once

#include "uc-core/configuration/synchronization/Synchronization.h"
#include "uc-core/particle/Globals.h"
#include "SynchronizationTypes.h"
#include "SamplesFifo.h"

#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING
#  include "LeastSquareRegression.h"
#endif
#ifdef SYNCHRONIZATION_STRATEGY_SIMPLE_MEAN_AND_STDDEVIANCE

#  include "Deviation.h"

#endif

#include "uc-core/configuration/Time.h"


//static void __roundFloatToUnsigned(volatile const float *const source, volatile uint16_t *const result) {
//    if ((*source >= 0 - 0.5) && (*source <= UINT16_MAX + 0.5)) {
//        *result = (uint16_t) (*source + 0.5);
//    } else {
//        *result = 0;
//    }
//}

#ifdef SYNCHRONIZATION_ENABLE_OUTLIER_REJECTION
/**
 * Update the outlier limits.
 */
static void __updateOutlierRejectionLimit(TimeSynchronization *const timeSynchronization) {
    float rejectionLimit = 0.5 + (SYNCHRONIZATION_OUTLIER_REJECTION_SIGMA_FACTOR *
                                  timeSynchronization->stdDeviance);
    timeSynchronization->outlierLowerBound = timeSynchronization->mean - rejectionLimit;
    timeSynchronization->outlierUpperBound = timeSynchronization->mean + rejectionLimit;
    timeSynchronization->isOutlierRejectionBoundValid = true;
}

#endif

/**
 * Clock skew approximation entry point: Independent on which approximation strategy is chosen,
 * this approximation function is triggered after each TimePackage has been received correctly and
 * it's pdu reception duration offset has been stored to the fifo queue.
 */
void tryApproximateTimings(TimeSynchronization *const timeSynchronization) {
    if (isEnoughFifoDataAvailable(timeSynchronization)) {
        if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable == false) {

#ifdef SYNCHRONIZATION_STRATEGY_SIMPLE_MEAN_AND_STDDEVIANCE
            // mean already calculated step-wise on inserting values to FiFo
            // calculateMean(timeSynchronization);
            calculateVarianceAndStdDeviance(timeSynchronization);

#  ifdef SYNCHRONIZATION_ENABLE_OUTLIER_REJECTION
            if (timeSynchronization->timeIntervalSamples.numSamples >=
                TIME_SYNCHRONIZATION_SAMPLES_FIFO_BUFFER_SIZE) {
                __updateOutlierRejectionLimit(timeSynchronization);
            }
#  endif

#else
#  if defined(SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING)
            calculateLinearFittingFunctionVarianceAndStdDeviance(timeSynchronization);
#  else
#    error no approximation strategy specified
#  endif
#endif

//#define __synchronization_meanValue ParticleAttributes.timeSynchronization.progressiveMean
#define __synchronization_meanValue ParticleAttributes.timeSynchronization.mean

            float newTimePeriodInterruptDelay;
            if (__synchronization_meanValue > TIME_SYNCHRONIZATION_SAMPLE_OFFSET) {
                newTimePeriodInterruptDelay =
                        LOCAL_TIME_DEFAULT_INTERRUPT_DELAY +
                        LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
                        (__synchronization_meanValue - TIME_SYNCHRONIZATION_SAMPLE_OFFSET)
                        SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION;
            } else {
                newTimePeriodInterruptDelay =
                        LOCAL_TIME_DEFAULT_INTERRUPT_DELAY -
                        LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
                        (TIME_SYNCHRONIZATION_SAMPLE_OFFSET - __synchronization_meanValue)
                        SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION;
            }

            ParticleAttributes.localTime.newTimePeriodInterruptDelay = 0.5 + newTimePeriodInterruptDelay;
            MEMORY_BARRIER;
            ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
        }
    }
}
