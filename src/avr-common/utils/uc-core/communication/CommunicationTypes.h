/**
 * @author Raoul Rubien 2016
 *
 * Communication types related definitions.
 */

#pragma once

#include <stdint.h>
#include "ManchesterDecodingTypes.h"
#include "uc-core/configuration/Particle.h"

/**
 * Describes a bit within a 4 byte buffer.
 */
typedef struct BufferBitPointer {
    uint8_t byteNumber : 4; // the referenced byte index
    uint8_t __pad: 4;
    uint8_t bitMask; // the referenced bit in the byte
} BufferBitPointer;

/**
 * Provides a linear 4 byte buffer and a bit pointer per communication channel.
 * The struct is used for transmission. Received bits are stored in the buffer
 * after being decoded.
 */
typedef struct PortBuffer {
    uint8_t bytes[TX_RX_NUMBER_BUFFER_BYTES]; // reception buffer
    BufferBitPointer pointer; // points to the next free position
} PortBuffer;

/**
 * Transmission port structure.
 */
typedef struct TxPort {
    PortBuffer buffer;
    BufferBitPointer dataEndPos; // data in between buffer start and dataEndPos is to be transmitted
    uint8_t isTransmitting : 1; // true during transmission, else false
    uint8_t isTxClockPhase : 1; // true if clock phase, else on data phase
    uint8_t isDataBuffered : 1; // true if the buffer contains data to be transmitted
    uint8_t __pad: 5;
} TxPort;

/**
 * Transmission ports bundle.
 */
typedef struct TxPorts {
    TxPort north;
    TxPort east;
    TxPort south;
} TxPorts;

/**
 * Reception port structure.
 */
typedef struct RxPort {
    // each pin interrupt stores snapshots and the flank direction into the buffer
    RxSnapshotBuffer snapshotsBuffer;
    PortBuffer buffer;
    uint16_t receptionOffset; // synchronization offset of fist received bit relative to compare counter
    uint8_t isOverflowed : 1;
    uint8_t isDataBuffered : 1;
    uint8_t __pad : 6;
} RxPort;

/**
 * Transmission timing related fields that allows runtime changes.
 */
typedef struct TransmissionTimerAdjustment {

    /**
     * snapshot differences below this threshold are treated as short interval occurrences
     * maxShortIntervalDuration = (maxShortIntervalDuration / 100) * transmissionClockDelay
     */
    uint16_t maxShortIntervalDuration;

    /**
     * the short interval overtime ratio
     */
    uint8_t maxShortIntervalDurationOvertimePercentageRatio;

    /**
     * snapshot differences below this threshold are treated as long interval occurrences
     * * maxShortIntervalDuration = (maxShortIntervalDuration / 100) * transmissionClockDelay
     */
    uint16_t maxLongIntervalDuration;

    /**
     * the long interval overtime ratio
     */
    uint8_t maxLongIntervalDurationOvertimePercentageRatio;

    /**
     * The tx/rx clock delay for the manchester coding.
     * Regarding tx: transmission interrupts are scheduled according this delay
     * Regarding rx: the short and long interval durations are derived from this delay
     */
    uint16_t transmissionClockDelay;
    /**
     * transmissionClockDelayHalf = transmissionClockDelay / 2
     */
    uint16_t transmissionClockDelayHalf;

    /**
     * the newly calculated / approximated transmission clock delay
     */
    uint16_t newTransmissionClockDelay;

    /**
     * transmission start offset
     */
    uint16_t transmissionClockShift;

    /**
     * newly calculated / approximated transmission start offset
     */
    uint16_t newTransmissionClockShift;

    uint8_t isTransmissionClockDelayUpdateable : 1;
    uint8_t isTransmissionClockShiftUpdateable : 1;

    uint8_t __pad : 6;
} TransmissionTimerAdjustment;

/**
 * Reception ports bundle.
 */
typedef struct RxPorts {
    RxPort north;
    RxPort east;
    RxPort south;
} RxPorts;

/**
 * Communication ports bundle.
 */
typedef struct CommunicationPorts {
    TxPorts tx;
    RxPorts rx;
} CommunicationPorts;

/**
 * Communication structure.
 */
typedef struct Communication {
    TransmissionTimerAdjustment timerAdjustment;
    CommunicationPorts ports;
} Communication;