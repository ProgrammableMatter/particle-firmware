/**
 * @author Raoul Rubien 2016
 */

#ifndef __MANCHESTER_DECODING_H__
#define __MANCHESTER_DECODING_H__

#include "./ManchesterDecodingTypes.h"

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

//FUNC_ATTRS void manchesterDecode(volatile RxSnapshotBuffer *o, ManchesterDecoderState state) {
//
//}

/**
 * stores the current TIMER_TX_RX_COUNTER value the specified flank to the given snapshot buffer
 */
FUNC_ATTRS void storeSnapshot(const bool isRisingEdge,
                              volatile RxSnapshotBuffer *snapshotBuffer) {
    volatile Snapshot *snapshot = &(snapshotBuffer->snapshots[++(snapshotBuffer->endIndex)]);
    snapshot->isRisingEdge = isRisingEdge;
    snapshot->snapshot = TIMER_TX_RX_COUNTER;
}

/**
 * re-sets the iterator to the start position
 */
FUNC_ATTRS void rxSnapshotBufferStart(volatile RxSnapshotBuffer *o) {
    o->iteratorIndex = o->startIndex;
}

/**
 * returns the current; post-increments the iterator
 */
FUNC_ATTRS volatile Snapshot *rxSnapshotBufferNext(volatile RxSnapshotBuffer *o) {
    return &o->snapshots[o->iteratorIndex++];
}

/**
 * true if next value is available
 */
FUNC_ATTRS bool rxSnapshotBufferHasNext(volatile RxSnapshotBuffer *o) {
    return o->iteratorIndex != (o->endIndex + 1);
}

/**
 * releases the buffer
 */
FUNC_ATTRS void rxSnapshotBufferClear(volatile RxSnapshotBuffer *o) {
    o->startIndex = o->endIndex;
    o->iteratorIndex = o->startIndex;
}


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif
