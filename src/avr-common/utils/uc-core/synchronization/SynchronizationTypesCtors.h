/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization types constructors.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "LeastSquareRegressionTypesCtors.h"
#include "SampleFifoTypesCtors.h"

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructTimeSynchronization(TimeSynchronization *const o) {
    constructSamplesFifoBuffer(&o->timeIntervalSamples);
#ifdef SYNCHRONIZATION_STRATEGY_LEAST_SQUARE_LINEAR_FITTING
    constructLeastSquareRegressionResult(&o->fittingFunction);
#endif
    o->mean = 0;
    o->__unnormalizedCumulativeMean = 0;
    o->progressiveMean = 0;
    o->variance = 0;
    o->stdDeviance = 0;
    o->outlierLowerBound = 0;
    o->outlierUpperBound = 0;
    // TODO: nextSyncPackge initial start time must be dependent to the network int. phase end
    o->nextSyncPackageTransmissionStartTime = 20;
    o->fastSyncPackageSeparation = 10;
    o->syncPackageSeparation = 600;
    o->totalFastSyncPackagesToTransmit = 500;
    o->isCalculationValid = false;
    o->isNextSyncPackageTransmissionEnabled = false;
    o->isOutlierRejectionBoundValid = false;
}
