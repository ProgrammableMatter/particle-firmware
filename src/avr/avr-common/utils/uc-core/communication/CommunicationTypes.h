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
    uint8_t byteNumber : 2; // byte number
    uint8_t __pad: 6;
    uint8_t bitMask; // the bit in the byte
} BufferBitPointer; // 1 + 1 = 2 bytes total

FUNC_ATTRS void constructBufferBitPointer(volatile BufferBitPointer *o) {
    o->byteNumber = 0;
    o->bitMask = 1;
}

/**
 * Provides a linear 4 byte buffer and a bit pointer per communication channel.
 * The struct is used for transmission and reception as well.
 */
typedef struct {
    uint8_t bytes[4]; // reception buffer
    BufferBitPointer pointer; // points to the next free position
} PortBuffer; // 4 + 2 = 6 bytes total


FUNC_ATTRS void constructPortBuffer(volatile PortBuffer *o) {
    for (int i = 0; i < sizeof(o->bytes); i++) {
        o->bytes[i] = 0;
    }
    constructBufferBitPointer(&(o->pointer));
}

typedef struct {
    PortBuffer buffer;
} TxPort; // 6 bytes total

FUNC_ATTRS void constructTxPort(volatile TxPort *o) {
    constructPortBuffer(&(o->buffer));
}

typedef struct {
    TxPort north;
    TxPort east;
    TxPort south;
} TxPorts; // 3 * 6 = 18 bytes total

FUNC_ATTRS void constructTxPorts(volatile TxPorts *o) {
    constructTxPort(&(o->north));
    constructTxPort(&(o->east));
    constructTxPort(&(o->south));
}

typedef struct {
    uint16_t receptionOffset; // synchronization offset of fist received bit relative to compare counter
    // TODO: deprecated
    uint16_t center__; // center, usually DEFAULT_TX_RX_COMPARE_TOP_VALUE / TX_RX_COUNTER_CENTER_VALUE_DIVISOR
    // TODO: deprecated
    uint16_t leftOfCenter__; // left border of center classification
    // TODO: deprecated
    uint16_t rightOfCenter__; // right border of center classification
    // TODO: deprecated
    uint16_t leftOfTop__; // left border of top classification
} TimerCounterAdjustment;

FUNC_ATTRS void constructTimerCounterAdjustment(volatile TimerCounterAdjustment *o,
                                                const volatile uint16_t *receptionDelta) {
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
    uint8_t isReceiving : 4; // is decremented on each expected coding flank, set to top on reception
    uint8_t isOverflowed : 1;
    uint8_t __pad: 3;
} RxPort; // 4 + 6 + 1 = 11 bytes total

FUNC_ATTRS void constructRxPort(volatile RxPort *o, volatile uint16_t *receptionDelta) {
    constructTimerCounterAdjustment(&(o->adjustment), receptionDelta);
    constructPortBuffer(&(o->buffer));
    o->isReceiving = false;
    o->isOverflowed = false;
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
    constructRxPort(&(o->north), &(o->receptionDelta));
    constructRxPort(&(o->east), &(o->receptionDelta));
    constructRxPort(&(o->south), &(o->receptionDelta));
}

typedef struct {
    TxPorts tx;
    RxPorts rx;
} Ports; // 18 + 33 = 51 bytes total

FUNC_ATTRS void constructPorts(volatile Ports *o) {
    constructTxPorts(&(o->tx));
    constructRxPorts(&(o->rx));
}

#include "../../simulation/SimulationMacros.h"


/**
 * Decrements the bit mask and the byte number accordingly. Does not verify underflow.
 */
FUNC_ATTRS void txBufferBitPointerNext(volatile BufferBitPointer *o) {
    o->bitMask >>= 0b00000001;
    if ((o->bitMask == 0) && (o->byteNumber > 0)) {
        o->bitMask = 0b10000000;
        o->byteNumber--;
    }
}

/**
 * Increments the bit mask and the byte number accordingly. Does not verify the buffer boundary.
 */
FUNC_ATTRS void rxBufferBitPointerNext(volatile BufferBitPointer *o) {
    o->bitMask <<= 1;
    if ((o->bitMask == 0) && (o->byteNumber < (sizeof(((PortBuffer *) 0)->bytes) - 1))) {
        o->bitMask = 1;
        o->byteNumber++;
    }
}

inline void __setToLowestPosition(volatile BufferBitPointer *o) {
    o->bitMask = 1;
    o->byteNumber = 0;
}

inline void __setToHighestPosition(volatile BufferBitPointer *o) {
    o->bitMask = 0x80;
    o->byteNumber = (sizeof(((PortBuffer *) 0)->bytes) - 1);
}

/**
 * points the reception buffer pointer to the start position (lowest bit)
 */
FUNC_ATTRS void rxBufferBitPointerStart(volatile BufferBitPointer *o) {
    __setToLowestPosition(o);
}


/**
 * points the reception buffer pointer to the start position (lowest bit)
 */
FUNC_ATTRS void txBufferBitPointerStart(volatile BufferBitPointer *o) {
    __setToHighestPosition(o);
}


/**
 * returns true if the pointer points at the beyond the maximum buffer position
 */
FUNC_ATTRS bool isRxBufferFull(volatile BufferBitPointer *o) {
    return o->bitMask == false;
}

/**
 * returns true if the buffer points to no position
 */
FUNC_ATTRS bool isTxBufferEmpty(volatile BufferBitPointer *o) {
    return o->bitMask == false;
}


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif