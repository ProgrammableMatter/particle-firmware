/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization types constructors.
 */

#pragma once

#include "uc-core/configuration/synchronization/SynchronizationTypesCtors.h"
#include "uc-core/configuration/synchronization/Synchronization.h"
#include "uc-core/configuration/communication/Communication.h"
#include "SynchronizationTypes.h"
#include "LeastSquareRegressionTypesCtors.h"
#include "SampleFifoTypesCtors.h"

//#if defined(SYNCHRONIZATION_ENABLE_ADAPTIVE_OUTLIER_REJECTION) || defined(SYNCHRONIZATION_ENABLE_SIGMA_DEPENDENT_OUTLIER_REJECTION)

void constructAdaptiveSampleRejection(AdaptiveSampleRejection *const o) {
    o->rejected = 0;
    o->accepted = 0;
    o->currentAcceptedDeviation = 1000;
    o->outlierLowerBound = 0;
    o->outlierUpperBound = 0;
    o->isOutlierRejectionBoundValid = false;
}

//#endif

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
    o->meanWithoutMarkedOutlier = 0;

#ifdef SYNCHRONIZATION_STRATEGY_MEAN_ENABLE_ONLINE_CALCULATION
    o->__unnormalizedCumulativeMean = 0;
    o->__unnormalizedCumulativeMeanWithoutMarkedOutlier = 0;
    o->__numberCumulatedValuesWithoutMarkedOutlier = 0;
#endif

#ifdef SYNCHRONIZATION_STRATEGY_PROGRESSIVE_MEAN
    // fake a default sample as start value
    o->progressiveMean = (uint16_t) (roundf(SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL *
                                     COMMUNICATION_DEFAULT_TX_RX_CLOCK_DELAY))
                         - TIME_SYNCHRONIZATION_SAMPLE_OFFSET;
#endif
    o->variance = 0;
    o->stdDeviance = 0;
    o->fastSyncPackageSeparation = SYNCHRONIZATION_TYPES_CTORS_FAST_SYNC_PACKAGE_SEPARATION;
    o->syncPackageSeparation = SYNCHRONIZATION_TYPES_CTORS_SYNC_PACKAGE_SEPARATION;
    o->totalFastSyncPackagesToTransmit = SYNCHRONIZATION_TYPES_CTORS_TOTAL_FAST_SYNC_PACKAGES;
    o->isNextSyncPackageTransmissionEnabled = false;
    o->isNextSyncPackageTimeUpdateRequest = false;
}
