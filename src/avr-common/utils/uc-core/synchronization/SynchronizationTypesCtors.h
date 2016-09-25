/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization types constructors.
 */

#pragma once

#include "SynchronizationTypes.h"

void constructSamplesFifoBuffer(SamplesFifoBuffer *const o) {
    o->__startIdx = 0;
    o->__insertIndex = 0;
    o->numSamples = 0;
    o->iterator = o->__startIdx;
    o->k = 0;
    o->d = 0;
    o->variance = 0;
    o->stdDeviance = 0;
    o->isCalculationValid = false;
}

void constructTimeSynchronization(TimeSynchronization *const o) {
    constructSamplesFifoBuffer(&o->timeIntervalSamples);
}