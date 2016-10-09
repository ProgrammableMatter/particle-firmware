/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo constructors.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "LeastSquareRegressionTypesCtors.h"

void constructFifoElement(FifoElement *const o) {
    o->value = 0;
    o->isRejected = false;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
void constructSamplesFifoBuffer(SamplesFifoBuffer *const o) {
    for (uint8_t i = 0; i < SAMPLE_FIFO_NUM_BUFFER_ELEMENTS; i++) {
        constructFifoElement(&o->samples[i]);
    }
    o->__startIdx = 0;
    o->__insertIndex = 0;
    o->numSamples = 0;
    o->iterator = o->__startIdx;
    constructFifoElement(&o->dropOut);
    o->isDropOutValid = false;
    o->__isPreDropOutValid = false;
}
