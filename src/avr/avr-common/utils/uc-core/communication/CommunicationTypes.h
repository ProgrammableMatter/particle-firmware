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

/**
 * returns true if the pointer points at the very first position, else false
 */
FUNC_ATTRS uint8_t isBufferEmpty(volatile BufferBitPointer *bufferPointer) {
    if (bufferPointer->bitMask == 0 && bufferPointer->byteNumber == 0) {
        return true;
    }
    return false;
}

/**
 * returns true if the pointer points at the very last position, else false
 */
FUNC_ATTRS uint8_t isBufferFull(volatile BufferBitPointer *bufferPointer) {
    if (bufferPointer->bitMask == (1 << 7) &&
        bufferPointer->byteNumber == sizeof(((PortBuffer *) 0)->bytes)) {
        return true;
    }
    return false;
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

/*
typedef struct {
    uint16_t receptionOffset; // synchronization offset of fist received bit relative to compare counter
    uint16_t estimatedCounterTop; // the estimated reception counter's top value
    uint16_t estimatedCounterCenter;
    uint16_t receptionDelta; // time span for reception classification: center|top +/- delta
    int8_t estimatedTopCounterOffset; // an approximated counter offset
} TimerCounterAdjustment; // 2 + 1 + (+ 1) = 4 bytes total
*/

typedef struct {
    uint16_t receptionOffset; // synchronization offset of fist received bit relative to compare counter
    uint16_t center; // center, usually DEFAULT_TX_RX_COMPARE_TOP_VALUE / TX_RX_COUNTER_CENTER_VALUE_DIVISOR
    uint16_t leftOfCenter; // left border of center classification
    uint16_t rightOfCenter; // right border of center classification
    uint16_t leftOfTop; // left border of top classification
    // top equals to TIMER_TX_RX_COMPARE_TOP_VALUE
    // rightOfTop equals to 0+receptionDelta
} TimerCounterAdjustment;

FUNC_ATTRS void constructTimerCounterAdjustment(volatile TimerCounterAdjustment *o,
                                                volatile uint16_t *receptionDelta) {
    o->receptionOffset = 0;
    o->center = DEFAULT_TX_RX_COMPARE_TOP_VALUE / TX_RX_COUNTER_CENTER_VALUE_DIVISOR;
    o->leftOfCenter = o->center - *receptionDelta;
    o->rightOfCenter = o->center + *receptionDelta;
    o->leftOfTop = DEFAULT_TX_RX_COMPARE_TOP_VALUE - *receptionDelta;
}

typedef struct {
    // port specific reception offsets and factors according to tis port's reception
    TimerCounterAdjustment adjustment;
    PortBuffer buffer;
    uint8_t isReceiving : 4; // is decremented on each expected coding flank, set to top on reception
    uint8_t __pad: 4;
} RxPort; // 4 + 6 + 1 = 11 bytes total

FUNC_ATTRS void constructRxPort(volatile RxPort *o, volatile uint16_t *receptionDelta) {
    constructTimerCounterAdjustment(&(o->adjustment), receptionDelta);
    constructPortBuffer(&(o->buffer));
    o->isReceiving = 0;
}

typedef struct {
    uint16_t receptionDelta;
    RxPort north;
    RxPort east;
    RxPort south;
} RxPorts; // 3 * 11 = 33 bytes total

FUNC_ATTRS void constructRxPorts(volatile RxPorts *o) {
    o->receptionDelta = DEFAULT_TX_RX_COMPARE_TOP_VALUE / TX_RX_RECEPTION_DELTA_VALUE_DIVISOR - 1;
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


#include "../ParticleParameters.h"


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif