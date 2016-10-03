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

#else

#  include "Deviation.h"

#endif

#include "uc-core/configuration/Time.h"

#if defined(SYNCHRONIZATION_STRATEGY_MEAN) || defined(SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN)

/**
 * Clock skew approximation entry point: Independent on which approximation strategy is chosen,
 * this approximation function is triggered after each TimePackage has been received correctly and
 * it's pdu reception duration offset has been stored to the fifo queue.
 */
void tryApproximateTimings(TimeSynchronization *const timeSynchronization) {
    if (isEnoughFifoDataAvailable(timeSynchronization)) {
        if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable == false) {

#ifndef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
            calculateMean(timeSynchronization);
#endif

#ifdef SYNCHRONIZATION_STRATEGY_MEAN
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.mean
#else
#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.progressiveMean
#endif
#endif
            CalculationType newTimePeriodInterruptDelay;
            if (__synchronization_meanValue > TIME_SYNCHRONIZATION_SAMPLE_OFFSET) {
                newTimePeriodInterruptDelay =
                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY +
                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
                        (__synchronization_meanValue - (CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET)
                        SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION;
            } else {
                newTimePeriodInterruptDelay =
                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY -
                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
                        ((CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET - __synchronization_meanValue)
                        SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION;
            }

//            // TODO: evaluation code
            if (newTimePeriodInterruptDelay >= UINT16_MAX) {
                return;
                blinkGenericErrorForever();
            }

            // TODO: evaluation code
            if ((0.5 + newTimePeriodInterruptDelay) >
                (ParticleAttributes.localTime.newTimePeriodInterruptDelay + 2)) {
                LED_STATUS3_TOGGLE;
            } else if ((0.5 + newTimePeriodInterruptDelay) <
                       (ParticleAttributes.localTime.newTimePeriodInterruptDelay - 2)) {
                LED_STATUS1_TOGGLE;
            } else {
                LED_STATUS2_TOGGLE;
            }

            ParticleAttributes.localTime.newTimePeriodInterruptDelay =
                    (uint16_t) (0.5 + newTimePeriodInterruptDelay);
            MEMORY_BARRIER;
            ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
        }
    }
}

#endif

#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING

/**
 * Clock skew approximation entry point: Independent on which approximation strategy is chosen,
 * this approximation function is triggered after each TimePackage has been received correctly and
 * it's pdu reception duration offset has been stored to the fifo queue.
 */
void tryApproximateTimings(TimeSynchronization *const timeSynchronization) {
    if (isEnoughFifoDataAvailable(timeSynchronization)) {
        if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable == false) {

#ifndef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
            calculateMean(timeSynchronization);
#endif
//            calculateVarianceAndStdDeviance(timeSynchronization);

#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING
            calculateLinearFittingFunctionVarianceAndStdDeviance(timeSynchronization);
#endif

#define __synchronization_meanValue timeSynchronization->fittingFunction.d

            CalculationType newTimePeriodInterruptDelay;
            if (__synchronization_meanValue > TIME_SYNCHRONIZATION_SAMPLE_OFFSET) {
                newTimePeriodInterruptDelay =
                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY +
                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
                        (__synchronization_meanValue - (CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET)
                        SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION;
            } else {
                newTimePeriodInterruptDelay =
                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY -
                        (CalculationType) LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_WORKING_POINT_PERCENTAGE *
                        ((CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET - __synchronization_meanValue)
                        SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION;
            }
            ParticleAttributes.localTime.newTimePeriodInterruptDelay = (uint16_t) (0.5 +
                                                                                   newTimePeriodInterruptDelay);
            MEMORY_BARRIER;
            ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
        }
    }
}

#endif