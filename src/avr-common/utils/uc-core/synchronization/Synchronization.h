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
 || defined(SYNCHRONIZATION_ENABLE_ADAPTIVE_MARKED_OUTLIER_REJECTION)

#include "Deviation.h"

#include "uc-core/stdout/stdio.h"

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

            calculateMeanWithoutOutlier(timeSynchronization);
            // TODO: evaluation code
            if (__synchronization_meanValue >= UINT16_MAX) {
                blinkLed3Forever(&ParticleAttributes.alerts);
            }

            if (__synchronization_meanValue <= 1000) {
                printf("s %lu \n", (uint32_t) __synchronization_meanValue);
                blinkLed4Forever(&ParticleAttributes.alerts);
            }


            CalculationType newTimePeriodInterruptDelay =
                    ((CalculationType) __synchronization_meanValue + (CalculationType) INT16_MAX) *
                    (CalculationType) __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE;


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

#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING

///**
// * Clock skew approximation entry point: Independent on which approximation strategy is chosen,
// * this approximation function is triggered after each TimePackage has been received correctly and
// * it's pdu reception duration offset has been stored to the fifo queue.
// */
//void tryApproximateTimings(TimeSynchronization *const timeSynchronization) {
//    if (isEnoughFifoDataAvailable(timeSynchronization)) {
//        if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable == false) {
//            calculateLinearFittingFunctionVarianceAndStdDeviance(timeSynchronization);
//
//#define __synchronization_meanValue timeSynchronization->fittingFunction.d
//
//            CalculationType newTimePeriodInterruptDelay;
//            if (__synchronization_meanValue > TIME_SYNCHRONIZATION_SAMPLE_OFFSET) {
//                newTimePeriodInterruptDelay =
//                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY +
//                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
//                        (__synchronization_meanValue - (CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET)
//                        SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION;
//            } else {
//                newTimePeriodInterruptDelay =
//                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY -
//                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
//                        ((CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET - __synchronization_meanValue)
//                        SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION;
//            }
//            ParticleAttributes.localTime.newTimePeriodInterruptDelay = (uint16_t) roundf(newTimePeriodInterruptDelay);
//            MEMORY_BARRIER;
//            ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
//        }
//    }
//}

#endif