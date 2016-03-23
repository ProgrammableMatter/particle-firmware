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
    uint8_t bitMask; // the bit in the byte
} BufferBitPointer;

/**
 * Provides a linear 4 byte buffer and a bit pointer per communication channel.
 * The struct is used for transmission and reception as well.
 */
typedef struct {
    uint8_t bytes[4]; // reception buffer
    BufferBitPointer pointer; // points to the next free position
} PortBuffer;


typedef struct {
    PortBuffer buffer;
} TxPort;

typedef struct {
    TxPort north;
    TxPort east;
    TxPort south;
} TxPorts;

typedef struct {
    PortBuffer buffer;
    uint8_t isReceiving : 2; // is decremented on each expected coding flank, set to top on reception
    uint16_t receptionSyncOffset; // synchronization offset of fist received bit relative to compare counter
} RxPort;

typedef struct {
    RxPort north;
    RxPort east;
    RxPort south;
} RxPorts;

typedef struct {
    RxPorts rx;
    TxPorts tx;
} Ports;

#endif