/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization types constructors.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "LeastSquareRegressionTypesCtors.h"
#include "SampleFifoTypesCtors.h"

void constructTimeSynchronization(TimeSynchronization *const o) {
    constructSamplesFifoBuffer(&o->timeIntervalSamples);
    constructLeastSquareRegressionResult(&o->fittingFunction);
    o->mean = 0;
    o->progressiveMean = 0;
    o->variance = 0;
    o->stdDeviance = 0;
    o->isCalculationValid = false;
}
