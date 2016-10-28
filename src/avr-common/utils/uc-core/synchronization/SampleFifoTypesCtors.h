/**
 * @author Raoul Rubien 26.09.2016
 *
 * Synchronization FiFo constructors.
 */

#pragma once

#include "SynchronizationTypes.h"
#include "LeastSquareRegressionTypesCtors.h"

void constructFifoElement(FifoElement *const o) {
    // The lightweight FiFo works only if full:
    // One can wait until enough observations have been stored to the queue
    // or speed up by pre-filling with synthetic values.
    o->value = (uint16_t) (roundf(SYNCHRONIZATION_PDU_NUMBER_CLOCKS_IN_MEASURED_INTERVAL *
                                  COMMUNICATION_DEFAULT_TX_RX_CLOCK_DELAY))
               - TIME_SYNCHRONIZATION_SAMPLE_OFFSET;
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
    // workaround for pre-filling with synthetic values: indicate that FiFo is full
    o->numSamples = SAMPLE_FIFO_NUM_BUFFER_ELEMENTS;
    // o->numSamples = 0;
    o->iterator = o->__startIdx;
    constructFifoElement(&o->dropOut);
    o->isDropOutValid = false;
    o->__isPreDropOutValid = false;
}
