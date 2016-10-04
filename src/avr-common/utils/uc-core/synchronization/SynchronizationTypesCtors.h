/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization types constructors.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "LeastSquareRegressionTypesCtors.h"
#include "SampleFifoTypesCtors.h"

#if defined(SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION) || defined(SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION)

void constructAdaptiveSampleRejection(AdaptiveSampleRejection *const o) {
    o->rejected = 0;
    o->accepted = 0;
    o->currentAcceptedDeviation = 1000;
    o->outlierLowerBound = 0;
    o->outlierUpperBound = 0;
    o->isOutlierRejectionBoundValid = false;
}

#endif

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructTimeSynchronization(TimeSynchronization *const o) {

//    constructSyncPackageTiming(&o->syncPackageTiming);
#if defined(SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION) || defined(SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION)
    constructAdaptiveSampleRejection(&o->adaptiveSampleRejection);
#endif
    constructSamplesFifoBuffer(&o->timeIntervalSamples);
#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING
    constructLeastSquareRegressionResult(&o->fittingFunction);
#endif
    o->mean = 0;
    o->meanWithoutOutlier = 0;
#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
    o->__unnormalizedCumulativeMean = 0;
#endif
    o->__unnormalizedCumulativeMeanWithoutOutlier = 0;
    o->__numberCumulatedValuesWithoutOutlier = 0;
#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
    o->progressiveMean = 0;
#endif
    o->variance = 0;
    o->stdDeviance = 0;
    // TODO: nextSyncPackge initial start time depends on the network int. phase's end
    o->nextSyncPackageTransmissionStartTime = 20;
    o->fastSyncPackageSeparation = 10;
//    o->syncPackageSeparation = 300;
    o->syncPackageSeparation = 20;
    o->totalFastSyncPackagesToTransmit = 500;
    o->isCalculationValid = false;
    o->isNextSyncPackageTransmissionEnabled = false;
}
