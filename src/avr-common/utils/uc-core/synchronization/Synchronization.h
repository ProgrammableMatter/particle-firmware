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

#if defined(SYNCHRONIZATION_STRATEGY_MEAN) || defined(SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN)

#include "Deviation.h"

/**
 * Clock skew approximation entry point: Independent on which approximation strategy is chosen,
 * this approximation function is triggered after each TimePackage has been received correctly and
 * it's pdu reception duration offset has been stored to the fifo queue.
 */
void tryApproximateTimings(TimeSynchronization *const timeSynchronization) {
//    if (isEnoughFifoDataAvailable(timeSynchronization)) {
    if (isFiFoFull(&timeSynchronization->timeIntervalSamples)) {
        if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable == false) {

//            LED_STATUS4_TOGGLE;
//#ifndef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
//            calculateMean(timeSynchronization);
//#endif
//
//#ifdef SYNCHRONIZATION_STRATEGY_MEAN
//#  ifdef SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_MARKED_OUTLIER
//#    define __synchronization_meanValue ParticleAttributes.timeSynchronization.meanWithoutMarkedOutlier
//#  else
//#    ifdef SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_OUTLIER
//#      define __synchronization_meanValue ParticleAttributes.timeSynchronization.meanWithoutOutlier
//            calculateVarianceAndStdDeviance(timeSynchronization);
//            calculateMeanWithoutOutlier(timeSynchronization);
//#    else
//#      define __synchronization_meanValue ParticleAttributes.timeSynchronization.mean
//#    endif
//#  endif
//#endif
//#  ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
//#    define __synchronization_meanValue ParticleAttributes.timeSynchronization.progressiveMean
//#  endif

//#define __synchronization_meanValue ParticleAttributes.timeSynchronization.mean
#define __synchronization_meanValue ParticleAttributes.timeSynchronization.meanWithoutOutlier

            calculateMeanWithoutOutlier(timeSynchronization);
            // TODO: evaluation code
            if (__synchronization_meanValue >= UINT16_MAX) {
                blinkLed3Forever(&ParticleAttributes.alerts);
            }

            if (__synchronization_meanValue <= 1000) {
                blinkLed4Forever(&ParticleAttributes.alerts);
            }

            CalculationType newTimePeriodInterruptDelay =
                    (__synchronization_meanValue + INT16_MAX) *
                    __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE;

//            uint32_t foo = (uint32_t) newTimePeriodInterruptDelay;
//            printf("s %lu\n", foo);
//            if (__synchronization_meanValue > TIME_SYNCHRONIZATION_SAMPLE_OFFSET) {
//                newTimePeriodInterruptDelay =
//                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY +
//                        //                        SYNCHRONIZATION_MANUAL_SAMPLE_MEAN_ATTENUATION_FACTOR_ABOVE_EXPECTATION *
//                        (__synchronization_meanValue - (CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET);
//                // TODO: evaluation code
//                if (newTimePeriodInterruptDelay >= UINT16_MAX) {
//                    blinkKnightRidersKittForever();
//                }
//            } else {
//                newTimePeriodInterruptDelay =
//                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY -
//                        //                        SYNCHRONIZATION_MANUAL_SAMPLE_MEAN_ATTENUATION_FACTOR_BELOW_EXPECTATION *
//                        ((CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET - __synchronization_meanValue);
//
//                // TODO: evaluation code
//                if (newTimePeriodInterruptDelay >= UINT16_MAX) {
//                    blinkLed1Forever(&ParticleAttributes.alerts);
//                }
//            }



//            if (newTimePeriodInterruptDelay >
//                ParticleAttributes.localTime.newTimePeriodInterruptDelay) {
//
//                newTimePeriodInterruptDelay = 0.999 * newTimePeriodInterruptDelay;
//            }
//
//            // TODO: is this last averaging relevant?
//            newTimePeriodInterruptDelay =
//                    (uint16_t) (0.5 + (ParticleAttributes.localTime.newTimePeriodInterruptDelay +
//                                       newTimePeriodInterruptDelay) / 2.0);



//            // TODO: evaluation code
//            if ((0.5 + newTimePeriodInterruptDelay) >
//                (ParticleAttributes.localTime.newTimePeriodInterruptDelay + 1.0)) {
//                LED_STATUS2_TOGGLE;
//            } else if ((0.5 + newTimePeriodInterruptDelay) <
//                       (ParticleAttributes.localTime.newTimePeriodInterruptDelay - 1.0)) {
//                LED_STATUS3_TOGGLE;
//            }

            // update the new mean/clock skew
            ParticleAttributes.localTime.newTimePeriodInterruptDelay =
                    (uint16_t) roundf(newTimePeriodInterruptDelay);


            MEMORY_BARRIER;
            ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
        }
    }
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