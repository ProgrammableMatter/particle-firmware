/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related implementation.
 */

#pragma once

#include "uc-core/configuration/synchronization/Synchronization.h"
#include "uc-core/configuration/communication/Communication.h"
#include "uc-core/configuration/communication/Commands.h"
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

#if defined(SYNCHRONIZATION_STRATEGY_RAW_OBSERVATION) \
 || defined(SYNCHRONIZATION_STRATEGY_MEAN) \
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
void tryApproximateTimings(void) {
//    TimeSynchronization *const timeSynchronization = &ParticleAttributes.timeSynchronization;
//#if !defined(SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN) || !defined(SYNCHRONIZATION_STRATEGY_RAW_OBSERVATION)
    if (isFiFoFull(&ParticleAttributes.timeSynchronization.timeIntervalSamples)) {
//#endif
        // if ISR already considered previous new values
        if (ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable == false &&
            ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable == false) {

            LED_STATUS4_TOGGLE;
#ifdef SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_MARKED_OUTLIER
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.meanWithoutMarkedOutlier
#endif
#ifdef SYNCHRONIZATION_STRATEGY_MEAN_WITHOUT_OUTLIER
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.meanWithoutOutlier
            //@pre mean is valid
            calculateVarianceAndStdDeviance();
            updateOutlierRejectionLimitDependingOnSigma();
            calculateMeanWithoutOutlier();
#endif
#ifdef SYNCHRONIZATION_STRATEGY_RAW_OBSERVATION
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.mean
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
#  define __synchronization_meanValue ParticleAttributes.timeSynchronization.fittingFunction.d
            calculateLinearFittingFunctionVarianceAndStdDeviance();
#endif

            // shift mean value back by +UINT16_T/2
            CalculationType observedPduDuration =
                    __synchronization_meanValue +
                    (CalculationType) TIME_SYNCHRONIZATION_SAMPLE_OFFSET;

            // calculate one manchester clock duration
            ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay =
                    (observedPduDuration +
                     SYNCHRONIZATION_MANUAL_ADJUSTMENT_CLOCK_ACCELERATION) /
                    SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL;

            // calculate manchester clock/2 duration
            ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelayHalf =
                    ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay / 2.0;

            // calculate the upper limit of measured short interval duration (manchester decoding decision limit)
            ParticleAttributes.communication.timerAdjustment.maxShortIntervalDuration =
                    roundf((CalculationType) COMMUNICATION_DEFAULT_MAX_SHORT_RECEPTION_OVERTIME_PERCENTAGE_RATIO *
                           (CalculationType) ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay);

            // calculate the upper limit of measured long interval duration (manchester decoding decision limit)
            ParticleAttributes.communication.timerAdjustment.maxLongIntervalDuration =
                    roundf((CalculationType) COMMUNICATION_DEFAULT_MAX_LONG_RECEPTION_OVERTIME_PERCENTAGE_RATIO *
                           (CalculationType) ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay);

            // calculate the new local time tracking interrupt delay
            ParticleAttributes.localTime.newTimePeriodInterruptDelay =
                    roundf(ParticleAttributes.communication.timerAdjustment.newTransmissionClockDelay *
                           LOCAL_TIME_TRACKING_INT_DELAY_MANCHESTER_CLOCK_MULTIPLIER);

            MEMORY_BARRIER;
            ParticleAttributes.localTime.isTimePeriodInterruptDelayUpdateable = true;
            ParticleAttributes.communication.timerAdjustment.isTransmissionClockDelayUpdateable = true;
        }
//#ifndef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
    }
//#endif
}

#endif
