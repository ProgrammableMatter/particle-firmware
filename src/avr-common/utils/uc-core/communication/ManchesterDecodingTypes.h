/**
 * @author Raoul Rubien 2016
 *
 * Manchester decoding related types definition.
 */

#pragma once

#include <stdint.h>
#include "uc-core/configuration/Communication.h"

/**
 * Possible manchester decoder states.
 */
typedef enum ManchesterDecodingStateType {
    DECODER_STATE_TYPE_START, // initialization state before decoding
    DECODER_STATE_TYPE_DECODING, // state when decoding
    DECODER_STATE_TYPE_POST_TIMEOUT_PROCESS, // state after last decoding
} ManchesterDecodingStateType;

/**
 * Manchester decoder state and phase state struct.
 */
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
 * bit is scarified for the flank direction information. Thus only the bits [15:1] of the snapshot are stored.
 */
typedef struct Snapshot {
    uint16_t isRisingEdge : 1;
    /**
     * The least significant snapshot value bit is ignored; this should be used as:
     * foo = s.snapshot << 1
     */
    uint16_t timerValue : 15;
} Snapshot;

/**
 * The struct is used as buffer for storing timestamps of received pin change interrupts.
 * The timestamps are then decoded to bits and stored to a RxBuffer struct.
 */
typedef struct RxSnapshotBuffer {
    /**
     * Manchester decoder states
     */
    ManchesterDecoderStates decoderStates;
    /**
     * snapshot buffer
     */
    Snapshot snapshots[RX_NUMBER_SNAPSHOTS];
    /**
     * field stores the previous dequeue value
     */
    uint16_t temporarySnapshotTimerValue;
    /**
     * field describes the 1st buffered position
     */
    uint8_t startIndex : 7;
    uint8_t __pad : 1;
    /**
     * describes the 1st invalid buffer position
     */
    uint8_t endIndex : 7;
    uint8_t __pad1 : 1;
    /**
     * field stores the 1st timer value of the last transmission
     */
    uint16_t temporaryTxStartSnapshotTimerValue;
    /**
     * field stores the last timer value of the last transmission
     */
    uint16_t temporaryTxStopSnapshotTimerValue;
    /**
     * number of passed half cycles reflects the number of π's passed from 1st snapshot
     * until the last snapshot before timeout
     * -------<=========>------
     * -------^---------^------
     */
    uint8_t numberHalfCyclesPassed;
} RxSnapshotBuffer;
