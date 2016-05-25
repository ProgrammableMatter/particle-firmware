/**
 * @author Raoul Rubien 2016
 */

#ifndef __COMMUNICATION_TYPES_H
#define __COMMUNICATION_TYPES_H

#include <inttypes.h>
#include <stdbool.h>
#include "../../simulation/SimulationMacros.h"
#include "../ParticleParameters.h"

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

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
 * The struct is used for transmission and reception as well.
 */
typedef struct {
    uint8_t bytes[7]; // reception buffer
    BufferBitPointer pointer; // points to the next free position
} PortBuffer; // 4 + 2 = 6 bytes total

FUNC_ATTRS void constructBufferBitPointer(volatile BufferBitPointer *o) {
    o->byteNumber = 0;
    o->bitMask = 1;
}

FUNC_ATTRS void constructPortBuffer(volatile PortBuffer *o) {
    for (uint8_t i = 0; i < sizeof(o->bytes); i++) {
        o->bytes[i] = 0;
    }
    constructBufferBitPointer(&(o->pointer));
}

typedef struct {
    PortBuffer buffer;
    BufferBitPointer dataEndPos; // data in between buffer start and dataEndPos is to be transmitted
    uint8_t enableTransmission : 1; // user handled flag: if enabled transmission is scheduled
    uint8_t retainTransmission : 1; // interrupt handled flag transmission starts after flag is cleared
    uint8_t __pad: 6;
} TxPort; // 6 + 2 + 1 bytes total

FUNC_ATTRS void constructTxPort(volatile TxPort *o) {
    constructPortBuffer(&(o->buffer));
    constructBufferBitPointer(&o->dataEndPos);
    o->enableTransmission = false;
    o->retainTransmission = true;
}

typedef struct {
    TxPort north;
    TxPort east;
    TxPort south;
} TxPorts; // 3 * 9 = 27 bytes total

FUNC_ATTRS void constructTxPorts(volatile TxPorts *o) {
    constructTxPort(&(o->north));
    constructTxPort(&(o->east));
    constructTxPort(&(o->south));
}

typedef struct {
    uint16_t receptionOffset; // synchronization offset of fist received bit relative to compare counter
    // TODO: deprecated
    uint16_t __pad; //center__; // center, usually DEFAULT_TX_RX_COMPARE_TOP_VALUE / TX_RX_COUNTER_CENTER_VALUE_DIVISOR
    // TODO: deprecated
    uint16_t __pad1; //leftOfCenter__; // left border of center classification
    // TODO: deprecated
    uint16_t __pad2; //rightOfCenter__; // right border of center classification
    // TODO: deprecated
    uint16_t __pad3; // leftOfTop__; // left border of top classification
} TimerCounterAdjustment;

FUNC_ATTRS void constructTimerCounterAdjustment(volatile TimerCounterAdjustment *o) {
    o->receptionOffset = 0;
//    o->center = (DEFAULT_TX_RX_COMPARE_TOP_VALUE / TX_RX_COUNTER_CENTER_VALUE_DIVISOR);
//    o->leftOfCenter = o->center - *receptionDelta;
//    o->rightOfCenter = o->center + *receptionDelta;
//    o->leftOfTop = o->rightOfCenter + 1;
}

typedef struct {
    // port specific reception offsets and factors according to tis port's reception
    TimerCounterAdjustment adjustment;
    PortBuffer buffer;
    uint8_t isReceiving
            : 4; // interrupt handled: is decremented on each expected but missing coding flank or refreshed on reception
    uint8_t isOverflowed : 1;
    uint8_t isDataBuffered : 1;
    uint8_t __pad: 2;
} RxPort; // 4 + 6 + 1 = 11 bytes total

FUNC_ATTRS void constructRxPort(volatile RxPort *o) {
    constructTimerCounterAdjustment(&(o->adjustment));
    constructPortBuffer(&(o->buffer));
    o->isReceiving = false;
    o->isOverflowed = false;
    o->isDataBuffered = false;
}

/**
 * returns false on ongoing reception else true
 */
FUNC_ATTRS bool isNotReceiving(volatile RxPort *o) {
    return o->isReceiving == false;
}

typedef struct {
    uint16_t receptionDelta;
    RxPort north;
    RxPort east;
    RxPort south;
} RxPorts; // 3 * 11 = 33 bytes total

FUNC_ATTRS void constructRxPorts(volatile RxPorts *o) {
    o->receptionDelta = DEFAULT_TX_RX_COMPARE_TOP_VALUE / TX_RX_RECEPTION_DELTA_VALUE_DIVISOR;
    constructRxPort(&(o->north));
    constructRxPort(&(o->east));
    constructRxPort(&(o->south));
}

typedef struct {
    TxPorts tx;
    RxPorts rx;
} Ports; // 27 + 33 = 60 bytes total

FUNC_ATTRS void constructPorts(volatile Ports *o) {
    constructTxPorts(&(o->tx));
    constructRxPorts(&(o->rx));
}

/**
 * Increments the bit mask and the byte number accordingly. Does not verify the buffer boundary.
 */
FUNC_ATTRS void bufferBitPointerNext(volatile BufferBitPointer *o) {
    o->bitMask <<= 1;
    if ((o->bitMask == 0) && (o->byteNumber < (sizeof(((PortBuffer *) 0)->bytes) - 1))) {
        o->bitMask = 1;
        o->byteNumber++;
    }
}

/**
 * points the reception buffer pointer to the start position (lowest bit)
 */
FUNC_ATTRS void bufferBitPointerStart(volatile BufferBitPointer *o) {
    o->bitMask = 1;
    o->byteNumber = 0;
}

/**
 * returns true if the transmission buffer pointer position equals to the tx port's data end position
 */
FUNC_ATTRS bool isDataEnd(volatile TxPort *o) {
    return o->dataEndPos.bitMask == o->buffer.pointer.bitMask &&
           o->dataEndPos.byteNumber == o->buffer.pointer.byteNumber;
}

/**
 * returns true if the pointer points at the beyond the maximum buffer position
 */
FUNC_ATTRS bool isBufferFull(volatile BufferBitPointer *o) {
    return o->bitMask == false;
}

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif