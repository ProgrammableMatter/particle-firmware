/**
 * @author Raoul Rubien 24.09.2016
 *
 * Synchronization types constructors.
 */

#pragma once

#include "SynchronizationTypes.h"

void constructSamplesFifoBuffer(SamplesFifoBuffer *const o) {
    o->numSamples = 0;
    o->startIdx = 0;
    o->endIdx = 0;
    o->iterator = o->startIdx;
    o->k = 0;
    o->d = 0;
    o->variance = 0;
}

void constructTimeSynchronization(TimeSynchronization *const o) {
    constructSamplesFifoBuffer(&o->timeIntervalSamples);
}