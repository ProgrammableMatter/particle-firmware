/**
 * @author Raoul Rubien 2016
 */

#ifndef COMMUNICATION_TYPES_H
#define COMMUNICATION_TYPES_H

#include <inttypes.h>

/**
 * Describes a bit within a 4 byte buffer.
 */
typedef struct {
    uint8_t byteNumber : 2; // byte number
    uint8_t : 6;
    uint8_t bitMask; // the bit in the byte
} BufferBitPointer; // 1 + 1 = 2 bytes total

/**
 * Provides a linear 4 byte buffer and a bit pointer per communication channel.
 * The struct is used for transmission and reception as well.
 */
typedef struct {
    uint8_t bytes[4]; // reception buffer
    BufferBitPointer pointer; // points to the next free position
} PortBuffer; // 4 + 2 = 6 bytes total


typedef struct {
    PortBuffer buffer;
} TxPort; // 6 bytes total

typedef struct {
    TxPort north;
    TxPort east;
    TxPort south;
} TxPorts; // 3 * 6 = 18 bytes total

typedef struct {
    uint16_t receptionSyncOffset; // synchronization offset of fist received bit relative to compare counter
    uint8_t isReceiving : 2; // is decremented on each expected coding flank, set to top on reception
    uint8_t : 6;
    PortBuffer buffer;
} RxPort; // 2 + 1 + 6 = 9 bytes total

typedef struct {
    RxPort north;
    RxPort east;
    RxPort south;
} RxPorts; // 3 * 9 = 27 bytes total

typedef struct {
    TxPorts tx;
    RxPorts rx;
} Ports; // 18 + 27 = 45 bytes total

#endif