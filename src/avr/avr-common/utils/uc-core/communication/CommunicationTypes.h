/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include <stdint.h>
#include "ManchesterDecodingTypes.h"

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
    uint8_t bytes[7]; // reception buffer
    BufferBitPointer pointer; // points to the next free position
} PortBuffer;

typedef struct TxPort {
    PortBuffer buffer;
    BufferBitPointer dataEndPos; // data in between buffer start and dataEndPos is to be transmitted
    uint8_t enableTransmission : 1; // user handled flag: if enabled transmission is scheduled
    uint8_t retainTransmission : 1; // interrupt handled flag transmission starts after flag is cleared
    uint8_t isTransmitting : 1; // true during transmission, else false
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
     *
     */
    uint16_t maxCounterValue;
    /**
     * snapshot differences below this threshold are treated as short interval occurrences
     */
    uint16_t maxShortIntervalDuration;
    /**
     * snapshot differences below this threshold are treated as long interval occurrences
     */
    uint16_t maxLongIntervalDuration;
    /**
     * True if the maxCounterValue has been updated.
     */
    uint8_t isMaxCounterUpdated : 1;
    uint8_t __pad : 7;
} TimerCounterAdjustment;

typedef struct RxPorts {
    TimerCounterAdjustment timerAdjustment;
    RxPort north;
    RxPort east;
    RxPort south;
} RxPorts;

typedef struct Ports {
    TxPorts tx;
    RxPorts rx;
    XmissionType xmissionState;
} Ports;
