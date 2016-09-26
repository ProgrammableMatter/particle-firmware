/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo constructors.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "LeastSquareRegressionTypesCtors.h"

void constructSamplesFifoBuffer(SamplesFifoBuffer *const o) {
    o->__startIdx = 0;
    o->__insertIndex = 0;
    o->numSamples = 0;
    o->iterator = o->__startIdx;
}
