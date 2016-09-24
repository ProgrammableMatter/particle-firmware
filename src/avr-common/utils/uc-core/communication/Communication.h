/**
 * @author Raoul Rubien 2016
 *
 * Byte oriented communication related implementation.
 */

#pragma once

#include "common/common.h"
#include "CommunicationTypes.h"
#include "ManchesterDecodingTypes.h"

extern FUNC_ATTRS void bufferBitPointerIncrement(volatile BufferBitPointer *o);
/**
 * Increments the bit mask and the byte number accordingly.
 * @param o the buffer bit pointer reference
 */
FUNC_ATTRS void bufferBitPointerIncrement(volatile BufferBitPointer *o) {
    o->bitMask <<= 1;
    if ((o->bitMask == 0) && (o->byteNumber < (sizeof(((PortBuffer *) 0)->bytes)))) {
        o->bitMask = 1;
        o->byteNumber++;
    }
}

extern FUNC_ATTRS void bufferBitPointerStart(volatile BufferBitPointer *o);
/**
 * points the reception buffer pointer to the start position (lowest bit)
 * @param o the buffer bit pointer reference
 */
FUNC_ATTRS void bufferBitPointerStart(volatile BufferBitPointer *o) {
    o->bitMask = 1;
    o->byteNumber = 0;
}

/**
 * Returns true if the transmission buffer pointer points exactly at the data end marker.
 * The marker is set one bit beyond the last data bit.
 * @param o the transmission port reference
 */
extern FUNC_ATTRS bool isDataEndPosition(TxPort *o);

FUNC_ATTRS bool isDataEndPosition(TxPort *o) {
    return o->dataEndPos.bitMask == o->buffer.pointer.bitMask &&
           o->dataEndPos.byteNumber == o->buffer.pointer.byteNumber;
}

/**
 * Evaluates to true if the buffer bit pointer points beyond the last buffer bit.
 * @param bufferBitPointer the pointer to compare to
 */
#define isBufferEndPosition(bufferBitPointer) \
    (((bufferBitPointer)->byteNumber == (sizeof(((PortBuffer *) (0))->bytes))) && \
    ((bufferBitPointer)->bitMask == 0x1))
