/**
 * @author Raoul Rubien 2016
 */

#ifndef COMMUNICATION_TYPES_H
#define COMMUNICATION_TYPES_H

#include <inttypes.h>
#include <stdbool.h>

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

/**
 * returns true if the pointer points at the very first position, else false
 */
inline uint8_t isBufferEmpty(volatile BufferBitPointer *bufferPointer) {
    if (bufferPointer->bitMask == 0 && bufferPointer->byteNumber == 0) {
        return true;
    }
    return false;
}

/**
 * returns true if the pointer points at the very last position, else false
 */
inline uint8_t isBufferFull(volatile BufferBitPointer *bufferPointer) {
    if (bufferPointer->bitMask == (1 << 7) &&
        bufferPointer->byteNumber == sizeof(((PortBuffer *) 0)->bytes)) {
        return true;
    }
    return false;
}


inline void constructBufferBitPointer(volatile BufferBitPointer *o) {
    o->byteNumber = 0;
    o->bitMask = 1;
}

inline void constructPortBuffer(volatile PortBuffer *o) {
    for (int i = 0; i < sizeof(o->bytes); i++) {
        o->bytes[i] = 0;
    }
    constructBufferBitPointer(&(o->pointer));
}

inline void constructTxPort(volatile TxPort *o) {
    constructPortBuffer(&(o->buffer));
}

inline void constructTxPorts(volatile TxPorts *o) {
    constructTxPort(&(o->north));
    constructTxPort(&(o->east));
    constructTxPort(&(o->south));
}

inline void constructRxPort(volatile RxPort *o) {
    o->receptionSyncOffset = 0;
    o->isReceiving = 0;
    constructPortBuffer(&(o->buffer));
}

inline void constructRxPorts(volatile RxPorts *o) {
    constructRxPort(&(o->north));
    constructRxPort(&(o->east));
    constructRxPort(&(o->south));
}

inline void constructPorts(volatile Ports *o) {
    constructTxPorts(&(o->tx));
    constructRxPorts(&(o->rx));
}

#endif