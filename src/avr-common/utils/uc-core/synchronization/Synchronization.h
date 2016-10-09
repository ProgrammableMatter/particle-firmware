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
#include "uc-core/stdout/Stdout.h"
#include "uc-core/stdout/stdio.h"

#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING

#  include "LeastSquareRegression.h"

#else

#  include "Deviation.h"

#endif

#include "uc-core/configuration/Time.h"

#if defined(SYNCHRONIZATION_STRATEGY_MEAN) \
 || defined(SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_MARKED_OUTLIER) \
 || defined(SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN) \
 || defined(SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_OUTLIER) \
 || defined(SYNCHRONIZATION_ENABLE_ADAPTIVE_MARKED_OUTLIER_REJECTION) \
 || defined(SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING)

/**
 * Clock skew approximation entry point: Independent on which approximation strategy is chosen,
 * this approximation function is triggered after each TimePackage has been received correctly and
 * it's pdu reception duration offset has been stored to the fifo queue.
 */
void tryApproximateTimings(TimeSynchronization *const timeSynchronization) {
#ifndef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
    if (isFiFoFull(&timeSynchronization->timeIntervalSamples)) {
#endif
        if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable == false) {

#ifdef SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_MARKED_OUTLIER
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.meanWithoutMarkedOutlier
#endif
#ifdef SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_OUTLIER
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.meanWithoutOutlier
            //@pre mean is valid
            calculateVarianceAndStdDeviance(timeSynchronization);
            updateOutlierRejectionLimitDependingOnSigma(timeSynchronization);
            calculateMeanWithoutOutlier(timeSynchronization);
#endif
#ifdef SYNCHRONIZATION_STRATEGY_MEAN
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.mean
#endif
#ifdef SYNCHRONIZATION_ENABLE_ADAPTIVE_MARKED_OUTLIER_REJECTION
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.mean
#endif
#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
#    define __synchronization_meanValue ParticleAttributes.timeSynchronization.progressiveMean
#endif
#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING
#  define __synchronization_meanValue timeSynchronization->fittingFunction.d
            calculateLinearFittingFunctionVarianceAndStdDeviance(timeSynchronization);
#endif

            // TODO: evaluation code
            if (__synchronization_meanValue >= UINT16_MAX) {
                blinkLed3Forever(&ParticleAttributes.alerts);
            }

            if (__synchronization_meanValue <= 1000) {
                blinkLed4Forever(&ParticleAttributes.alerts);
            }

//            CalculationType newTimePeriodInterruptDelay =
//                    ((CalculationType) __synchronization_meanValue + (CalculationType) INT16_MAX) *
//                    (CalculationType) __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE;

            // shift mean value back by +UINT16_T/2
            CalculationType realDuration = __synchronization_meanValue
                                           + (CalculationType) INT16_MAX;
            // TODO: expected duration - 512 must be taken from runtime definition!
#ifdef SYNCHRONIZATION_TIME_PACKAGE_DURATION_COUNTING_EXCLUSIVE_LAST_RISING_EDGE
            // on considering 1st-falling to last falling time package edge
            CalculationType expectedDuration = UINT16_MAX - 512;
#else
            // on considering whole time package duration
            CalculationType expectedDuration = UINT16_MAX;
#endif
            // calculate new local time tracking interrupt delay
            CalculationType factor = realDuration / expectedDuration;
            CalculationType newTimePeriodInterruptDelay =
                    factor * (CalculationType) UINT16_MAX *
                    __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE;

            // TODO: evaluation code
            if (newTimePeriodInterruptDelay >= UINT16_MAX) {
                blinkLed1Forever(&ParticleAttributes.alerts);
            }
            if (newTimePeriodInterruptDelay <= 10000) {
                blinkLed2Forever(&ParticleAttributes.alerts);
            }

            // update the new mean/clock skew smoothly
            ParticleAttributes.localTime.newTimePeriodInterruptDelay =
                    (uint16_t) roundf(
                            ((CalculationType) ParticleAttributes.localTime.newTimePeriodInterruptDelay +
                             newTimePeriodInterruptDelay) / 2.0);

            MEMORY_BARRIER;
            ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
        }
#ifndef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
    }
#endif
}

#endif
