/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include <stdint.h>

typedef enum ManchesterDecodingStateType {
    DECODER_STATE_TYPE_START, // initialization state before decoding
    DECODER_STATE_TYPE_DECODING, // state when decoding
} ManchesterDecodingStateType;

typedef struct ManchesterDecoderStates {
    ManchesterDecodingStateType decodingState;
    /**
     * phase state: the 1 bit counter is incremented by 1 on short intervals and
     * by increased by 2 on long intervals
     */
    uint8_t phaseState: 1;
    uint8_t __pad : 7;
} ManchesterDecoderStates;

/**
 * A snapshot consists of a 16 bit value and the flank direction information. The least significant snapshot
 * value bit is scarified for the flank direction information. Thus only the bits [1:15] of the snapshot are stored.
 */
typedef struct Snapshot {
    uint16_t isRisingEdge : 1;
    /**
     * the least significant snapshot value bit is ignored; this should be used as
     * foo = s.snapshot << 1
     */
    uint16_t timerValue : 15;
} Snapshot; // 2 bytes




/**
 * The struct is used as buffer for storing timestamps of received pin change interrupts
 * and manchester decoding. The timestamps are then decoded to bits and stored to the RxBuffer struct.
 */
typedef struct RxSnapshotBuffer {
    /**
     * decoding states
     */
    ManchesterDecoderStates decoderStates;
    /**
     * each snapshot's lsb describes rising (1) or falling (0) flank occurred at the given snapshot
     */
    Snapshot snapshots[128];
    /**
     * describes the 1st buffered position
     */
    uint8_t startIndex : 7;
    /**
     * describes the 1st invalid position
     */
    uint8_t __pad : 1;
    uint8_t endIndex : 7;
//    /**
//     * used for iteration within start and end position:
//     * inclusive start and (end position - 1)
//     */
    uint8_t __pad1 : 1;
//    uint8_t iteratorIndex : 7;

    /**
     * used to store the current dequeue value
     */
    uint16_t temporaryDequeueRegister;
} RxSnapshotBuffer; // 128 * 2 + 3 bytes = 259 bytes
