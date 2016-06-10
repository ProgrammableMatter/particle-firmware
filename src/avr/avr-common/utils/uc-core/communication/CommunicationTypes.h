/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include "ManchesterDecodingTypes.h"
#include "simulation/SimulationMacros.h"
#include "uc-core/fw-configuration/ParticleParameters.h"

/**
 * describes the transmission/reception states
 */
typedef enum {
    STATE_TYPE_XMISSION_TYPE_ENABLED_TX = 0, // transmission only
    STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX, // tx/rx
    STATE_TYPE_XMISSION_TYPE_ENABLED_RX, // reception only
    STATE_TYPE_XMISSION_TYPE_DISABLED_TX_RX // tx/rx disabled
} XmissionType;

/**
 * Describes a bit within a 4 byte buffer.
 */
typedef struct {
    uint8_t byteNumber : 3; // byte number
    uint8_t __pad: 5;
    uint8_t bitMask; // the bit in the byte
} BufferBitPointer; // 1 + 1 = 2 bytes total

/**
 * Provides a linear 4 byte buffer and a bit pointer per communication channel.
 * The struct is used for transmission. Received bits are stored in the buffer
 * after being decoded.
 */
typedef struct {
    uint8_t bytes[7]; // reception buffer
    BufferBitPointer pointer; // points to the next free position
} PortBuffer; // 7 + 2 = 9 bytes total

typedef struct {
    PortBuffer buffer;
    BufferBitPointer dataEndPos; // data in between buffer start and dataEndPos is to be transmitted
    uint8_t enableTransmission : 1; // user handled flag: if enabled transmission is scheduled
    uint8_t retainTransmission : 1; // interrupt handled flag transmission starts after flag is cleared
    uint8_t isTransmitting : 1; // true during transmission, else false
    uint8_t __pad: 5;
} TxPort; // 9 + 2 + 1 = 12 bytes

typedef struct {
    TxPort north;
    TxPort east;
    TxPort south;
} TxPorts; // 3 * 12 = 36 bytes total

typedef struct {
    // each pin interrupt stores snapshots and the flank direction into the buffer
    RxSnapshotBuffer snapshotBuffer;
    PortBuffer buffer;
    uint16_t receptionOffset; // synchronization offset of fist received bit relative to compare counter
    uint8_t isReceiving
            : 4; // interrupt handled: is decremented on each expected but missing coding flank or refreshed on reception
    uint8_t isOverflowed : 1;
    uint8_t isDataBuffered : 1;
    uint8_t __pad: 2;
} RxPort; // 259 + 9 + 2 + 1  = 271  bytes total

typedef struct {
    uint16_t maxValue;
    uint16_t centerValue; // maxValue / 2
    uint16_t leftOfCenter; // centerValue - (centerValue / 2)
    uint16_t rightOfCenter; // centerValue + (centerValue /2)
    uint16_t __reserved;
} TimerCounterAdjustment; // 10 bytes

typedef struct {
    TimerCounterAdjustment timerAdjustment;
    RxPort north;
    RxPort east;
    RxPort south;
} RxPorts; // 10 + 3 * 271 = 823  bytes total

typedef struct {
    TxPorts tx;
    RxPorts rx;
    XmissionType xmissionState;
} Ports; // 36 + 823 = 859 bytes total
