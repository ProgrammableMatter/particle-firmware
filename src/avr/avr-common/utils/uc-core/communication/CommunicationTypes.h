/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include <stdint.h>
#include "ManchesterDecodingTypes.h"
#include "uc-core/configuration/Particle.h"

/**
 * describes the transmission/reception states
 */
typedef enum XmissionType {
    STATE_TYPE_XMISSION_TYPE_ENABLED_TX = 0, // transmission only
    STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX, // tx/rx
    STATE_TYPE_XMISSION_TYPE_ENABLED_RX, // reception only
    STATE_TYPE_XMISSION_TYPE_DISABLED_TX_RX // tx/rx disabled
} XmissionType;

/**
 * Describes a bit within a 4 byte buffer.
 */
typedef struct BufferBitPointer {
    uint8_t byteNumber : 3; // byte number
    uint8_t __pad: 5;
    uint8_t bitMask; // the bit in the byte
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

typedef struct TxPort {
    PortBuffer buffer;
    BufferBitPointer dataEndPos; // data in between buffer start and dataEndPos is to be transmitted
    uint8_t isTransmitting : 1; // true during transmission, else false
    uint8_t isTxClockPhase : 1; // true if clock phase, else on data phase
    uint8_t isDataBuffered : 1; // true if the buffer contains data to be transmitted
    uint8_t __pad: 5;
} TxPort;

typedef struct TxPorts {
    TxPort north;
    TxPort east;
    TxPort south;
} TxPorts;

typedef struct RxPort {
    // each pin interrupt stores snapshots and the flank direction into the buffer
    RxSnapshotBuffer snapshotsBuffer;
    PortBuffer buffer;
    uint16_t receptionOffset; // synchronization offset of fist received bit relative to compare counter
    uint8_t isOverflowed : 1;
    uint8_t isDataBuffered : 1;
    uint8_t __pad : 6;
} RxPort;

typedef struct TimerCounterAdjustment {

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
    uint16_t transmissionCockShift;

    /**
     * newly calculated / approximated transmission start offset
     */
    uint16_t newTransmissionClockShift;

    uint8_t isTransmissionClockDelayUpdateable : 1;
    uint8_t isTransmissionClockShiftUpdateable : 1;

    uint8_t __pad : 6;
} TimerCounterAdjustment;

typedef struct RxPorts {
    RxPort north;
    RxPort east;
    RxPort south;
} RxPorts;

typedef struct Ports {
    TimerCounterAdjustment timerAdjustment;
    XmissionType xmissionState;
    TxPorts tx;
    RxPorts rx;
} Ports;
