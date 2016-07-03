/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "Communication.h"
#include "ManchesterDecodingTypes.h"
#include "uc-core/interrupts/TimerCounter.h"

extern CTOR_ATTRS void constructSnapshot(volatile Snapshot *o);

CTOR_ATTRS void constructSnapshot(volatile Snapshot *o) {
    o->isRisingEdge = false;
    o->timerValue = 0;
}

extern CTOR_ATTRS void constructManchesterDecoderState(volatile ManchesterDecoderStates *o);

CTOR_ATTRS void constructManchesterDecoderState(volatile ManchesterDecoderStates *o) {
    o->decodingState = DECODER_STATE_TYPE_START;
    o->phaseState = 0;
}

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
