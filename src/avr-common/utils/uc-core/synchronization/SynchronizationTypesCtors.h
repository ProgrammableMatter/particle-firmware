/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization types constructors.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "LeastSquareRegressionTypesCtors.h"
#include "SampleFifoTypesCtors.h"


void constructAdaptiveSampleRejection(AdaptiveSampleRejection *const o) {
    o->rejected = 0;
    o->accepted = 0;
    o->currentAcceptedDeviation = 20000;
    o->currentRejectionRatio = 0;
    o->targetRejectionRatio = 0.4;
    o->outlierLowerBound = 0;
    o->outlierUpperBound = 0;
    o->isOutlierRejectionBoundValid = false;
    o->isCurrentRejectionRatioValid = false;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructTimeSynchronization(TimeSynchronization *const o) {
    constructSamplesFifoBuffer(&o->timeIntervalSamples);
    constructAdaptiveSampleRejection(&o->adaptiveSampleRejection);
#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING
    constructLeastSquareRegressionResult(&o->fittingFunction);
#endif
    o->mean = 0;
    o->__unnormalizedCumulativeMean = 0;
    o->progressiveMean = 0;
    o->variance = 0;
    o->stdDeviance = 0;
    // TODO: nextSyncPackge initial start time depends on the network int. phase's end
    o->nextSyncPackageTransmissionStartTime = 20;
    o->fastSyncPackageSeparation = 10;
    o->syncPackageSeparation = 300;
    o->totalFastSyncPackagesToTransmit = 500;
    o->isCalculationValid = false;
    o->isNextSyncPackageTransmissionEnabled = false;
}
