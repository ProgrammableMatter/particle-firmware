/**
 * @author Raoul Rubien 2016
 */

#ifndef __MANCHESTER_DECODING_TYPES_H__
#define __MANCHESTER_DECODING_TYPES_H__

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

typedef enum {
    DECODING_STATE_TYPE_SYNCHRONIZED,
    DECODING_STATE_TYPE_DECODING,
    DECODIGN_STATE_TYPE_UNSYNCHRONIZED
} ManchesterDecodingStateType;

typedef struct {
    ManchesterDecodingStateType state;

} ManchesterDecoderState;

/**
 * A snapshot consists of a 16 bit value and the flank direction information. The least significant snapshot
 * value bit is scarified for the flank direction information. Thus only the bits [1:15] of the snapshot are stored.
 */
typedef struct {
    uint16_t isRisingEdge : 1;
    /**
     * the least significant snapshot value bit is ignored; this should be used as
     * foo = s.snapshot << 1
     */
    uint16_t snapshot : 15;
} Snapshot;


FUNC_ATTRS void __constructSnapshot(volatile Snapshot *o) {
    o->isRisingEdge = false;
    o->snapshot = 0;
}

/**
 * The struct is used as buffer for storing timestamps of received pin change interrupts
 * and manchester decoding. The timestamps are then decoded to bits and stored to the RxBuffer struct.
 */
typedef struct {
    /**
     * decoding states
     */
    ManchesterDecodingStateType decoderState;
    /**
     * each snapshot's lsb describes rising (1) or falling (0) flank occurred at the given snapshot
     */
    Snapshot snapshots[128];
    uint8_t startIndex : 7;
    uint8_t endIndex : 7;
    uint8_t iteratorIndex : 7;
} RxSnapshotBuffer;


FUNC_ATTRS void constructRxSnapshotBuffer(volatile RxSnapshotBuffer *o) {
    for (uint8_t idx = 0; idx < (sizeof(o->snapshots) / sizeof(Snapshot)); idx++) {
        __constructSnapshot(&o->snapshots[idx]);
    }
    o->startIndex = 0;
    o->endIndex = 0;
    o->iteratorIndex = 0;
}


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif
