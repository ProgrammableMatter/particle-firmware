/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization related types.
 */

#pragma once

#include <stdint.h>
#include "BasicCalculationTypes.h"
#include "uc-core/configuration/synchronization/Synchronization.h"
#include "LeastSquareRegressionTypes.h"
#include "SampleFifoTypes.h"

//#if defined(SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION) || defined(SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION)
typedef struct AdaptiveSampleRejection {
    uint16_t rejected;
    uint16_t accepted;
    int16_t currentAcceptedDeviation; // must be signed!
    /**
     * Lower bound for outlier to be rejected.
     */
    SampleValueType outlierLowerBound;
    /**
     * Upper bound for outlier to be rejected.
     */
    SampleValueType outlierUpperBound;
    /**
     * switch enabled when outlier rejection boundary is valid
     */
    uint8_t isOutlierRejectionBoundValid : 1;
    uint8_t __pad : 7;
} AdaptiveSampleRejection;
//#endif

typedef struct TimeSynchronization {

//    SyncPackageTiming syncPackageTiming;

//#if defined(SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION) || defined(SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION)
    AdaptiveSampleRejection adaptiveSampleRejection;
//#endif
    SamplesFifoBuffer timeIntervalSamples;
#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING
    LeastSquareRegressionResult fittingFunction;
#endif
    /**
     * mean of all values in FiFo
     */
    CalculationType mean;
    /**
     * mean of all values in FiFo within µ+/-f*σ
     */
    CalculationType meanWithoutOutlier;
    /**
     * mean of all values in FiFo being within the current µ+/-f*σ when entered the FiFo
     */
    CalculationType meanWithoutMarkedOutlier;

#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
    /**
     * The cumulative unnormalized mean field is used to calculate a step-wise mean
     * by just observing the incoming and outgoing FiFo values without
     * the need for a complete FiFo iteration.
     */
    CumulationType __unnormalizedCumulativeMean;
    CumulationType __unnormalizedCumulativeMeanWithoutMarkedOutlier;
    IndexType __numberCumulatedValuesWithoutMarkedOutlier;
#endif
#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
    /**
     * Very simple syn. implementation: The progressive mean has kind of knowledge of previous values.
     */
    CalculationType progressiveMean;
#endif
    /**
    * samples' variance
    */
    CalculationType variance;
    /**
     * samples' standard deviation
     */
    CalculationType stdDeviance;

    /**
     * next local time stamp when transmission is to be initialized
     */
//    uint16_t nextSyncPackageTransmissionStartTime;
    /**
     * default sync package delay in between packages
     */
    uint16_t syncPackageSeparation;
    /**
     * sync. package separation during fast sync. phase; usually much lower than default sync package separation
     */
    uint16_t fastSyncPackageSeparation;
    /**
     * number of packages to transmit for fast synchronization
     */
    uint16_t totalFastSyncPackagesToTransmit;
    /**
     * barrier for sync package scheduling
     */
    uint8_t isNextSyncPackageTransmissionEnabled : 1;
    uint8_t isNextSyncPackageTimeUpdateRequest : 1;
    uint8_t __pad : 6;
} TimeSynchronization;