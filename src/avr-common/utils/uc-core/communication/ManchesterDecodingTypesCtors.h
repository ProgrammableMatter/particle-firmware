/**
 * @author Raoul Rubien 2016
 *
 * Manchester types constructor implementation.
 */

#pragma once

#include "Communication.h"
#include "ManchesterDecodingTypes.h"

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructSnapshot(volatile Snapshot *o);

CTOR_ATTRS void constructSnapshot(volatile Snapshot *o) {
    o->isRisingEdge = false;
    o->timerValue = 0;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructManchesterDecoderState(volatile ManchesterDecoderStates *o);

CTOR_ATTRS void constructManchesterDecoderState(volatile ManchesterDecoderStates *o) {
    o->decodingState = DECODER_STATE_TYPE_START;
    o->phaseState = 0;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructRxSnapshotBuffer(volatile RxSnapshotBuffer *o);

CTOR_ATTRS void constructRxSnapshotBuffer(volatile RxSnapshotBuffer *o) {
    constructManchesterDecoderState(&o->decoderStates);
    o->startIndex = 0;
    o->endIndex = 0;
    o->temporarySnapshotTimerValue = 0;
    o->temporaryTxStartSnapshotTimerValue = 0;
    o->temporaryTxStopSnapshotTimerValue = 0;
    o->numberHalfCyclesPassed = 0;
}
