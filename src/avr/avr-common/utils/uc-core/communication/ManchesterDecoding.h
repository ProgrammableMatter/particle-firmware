/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "Communication.h"
#include "ManchesterDecodingTypes.h"
#include "uc-core/interrupts/TimerCounter.h"

#ifdef TRY_INLINE
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif

/**
 * evaluates to true if the phase state indicates a synchronized state, else to false
 */
#define isInPhase(manchesterDecoderState) \
    (manchesterDecoderState.phaseState == false)

/**
 * updates the phase state by a short interval
 */
#define phaseStateAdvanceShortInterval(manchesterDecoderState) \
    (manchesterDecoderSTate.phaseState += 1)

/**
 * updates the phase state by a long interval
 */
#define phaseStateAdvanceLongInterval(manchesterDecoderState) \
    (manchesterDecoderSTate.phaseState += 2)


/**
 * Stores the data bit to the reception buffer unless the buffer is saturated.
 */
FUNC_ATTRS void __storeDataBit(volatile RxPort *rxPort, const volatile uint8_t isRisingEdge) {
    // save bit to buffer
    if (!isBufferEndPosition(&rxPort->buffer.pointer)) {
        if (isRisingEdge) {
            DEBUG_CHAR_OUT('1');
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] setBit rxPort->buffer.pointer.bitMask;
        } else {
            DEBUG_CHAR_OUT('0');
            rxPort->buffer.bytes[rxPort->buffer.pointer.byteNumber] unsetBit rxPort->buffer.pointer.bitMask;
        }
        bufferBitPointerIncrement(&rxPort->buffer.pointer);
    } else {
        rxPort->isOverflowed = true;
    }
}

/**
 * appends the current TIMER_TX_RX_COUNTER value and the specified flank to the snapshot buffer
 */
FUNC_ATTRS void appendSnapshot(const bool isRisingEdge,
                               volatile RxSnapshotBuffer *snapshotBuffer) {
    volatile Snapshot *snapshot = &(snapshotBuffer->snapshots[snapshotBuffer->endIndex]);
    snapshot->timerValue = TIMER_TX_RX_COUNTER;
    snapshot->isRisingEdge = isRisingEdge;
    ++snapshotBuffer->endIndex;
}

/**
 * re-sets the iterator to the start position
 */
FUNC_ATTRS void __rxSnapshotBufferResetIterator(volatile RxSnapshotBuffer *o) {
    o->iteratorIndex = o->startIndex;
}

/**
 * returns the current element and post-increments the iterator
 */
FUNC_ATTRS volatile Snapshot *__rxSnapshotBufferGetAndIncrement(volatile RxSnapshotBuffer *o) {
    return &o->snapshots[o->iteratorIndex++];
}

/**
 * returns the previous element
 */
FUNC_ATTRS volatile Snapshot *__rxSnapshotBufferGetPrevious(volatile RxSnapshotBuffer *o) {
    return &o->snapshots[o->iteratorIndex - 1];
}

/**
 * returns the current element
 */
FUNC_ATTRS volatile Snapshot *__rxSnapshotBufferGetCurrent(volatile RxSnapshotBuffer *o) {
    return &o->snapshots[o->iteratorIndex];
}

/**
 * returns the next element
 */
FUNC_ATTRS volatile Snapshot *__rxSnapshotBufferGetNext(volatile RxSnapshotBuffer *o) {
    return &o->snapshots[o->iteratorIndex + 1];
}

/**
 * true if a previous value is available
 */
FUNC_ATTRS bool __rxSnapshotBufferHasPrevious(volatile RxSnapshotBuffer *o) {
    return o->iteratorIndex == (o->startIndex + 1);
}

/**
 * true if the current value is available
 */
FUNC_ATTRS bool __rxSnapshotBufferHasCurrent(volatile RxSnapshotBuffer *o) {
    return o->iteratorIndex != (o->endIndex - 1);
}

/**
 * true if the next value is available
 */
FUNC_ATTRS bool __rxSnapshotBufferHasNext(volatile RxSnapshotBuffer *o) {
    return o->iteratorIndex != (o->endIndex - 2);
}

/**
 * releases the buffer
 */
FUNC_ATTRS void __rxSnapshotBufferClear(volatile RxSnapshotBuffer *o) {
    o->startIndex = o->endIndex;
    o->iteratorIndex = o->startIndex;
}

/**
 * decodes the specified buffer's snapshots to bits and bytes
 */
// TODO: implementation pending
FUNC_ATTRS void manchesterDecodeBuffer(volatile RxPort *rxPort) {
//                TODO ...
    volatile RxSnapshotBuffer *snapshotBuffer = &rxPort->snapshotBuffer;

    DECODING:
    switch (snapshotBuffer->decoderState) {

        case DECODER_STATE_TYPE_STOPPED:
            // 1st falling edge is synchronizing the decoder
            if (__rxSnapshotBufferHasNext(snapshotBuffer)) {
                volatile Snapshot *snapshot = __rxSnapshotBufferGetAndIncrement(snapshotBuffer);
                if (snapshot->isRisingEdge == false) {
                    snapshotBuffer->decoderState = DECODER_STATE_TYPE_DECODING;
                    DEBUG_CHAR_OUT('S');
                    goto DECODING;
                }
            }
            break;

        case DECODER_STATE_TYPE_DECODING:
            while (__rxSnapshotBufferHasNext(snapshotBuffer)) {
//                TODO ...
            }
            break;

        default:
            break;
    }
}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif
