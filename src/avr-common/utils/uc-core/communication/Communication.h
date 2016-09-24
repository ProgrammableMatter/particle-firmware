/**
 * @author Raoul Rubien 2016
 *
 * Byte oriented communication related implementation.
 */

#pragma once

#include "common/common.h"
#include "CommunicationTypes.h"
#include "ManchesterDecodingTypes.h"

/**
 * Increments the bit mask and the byte number accordingly.
 * @param o the buffer bit pointer reference
 */
void bufferBitPointerIncrement(volatile BufferBitPointer *const o) {
    o->bitMask <<= 1;
    if ((o->bitMask == 0) && (o->byteNumber < (sizeof(((PortBuffer *) 0)->bytes)))) {
        o->bitMask = 1;
        o->byteNumber++;
    }
}

/**
 * points the reception buffer pointer to the start position (lowest bit)
 * @param o the buffer bit pointer reference
 */
void bufferBitPointerStart(volatile BufferBitPointer *const o) {
    o->bitMask = 1;
    o->byteNumber = 0;
}

/**
 * Returns true if the transmission buffer pointer points exactly at the data end marker.
 * The marker is set one bit beyond the last data bit.
 * @param o the transmission port reference
 */

bool isDataEndPosition(const TxPort *const txPort) {
    return (txPort->dataEndPos.bitMask == txPort->buffer.pointer.bitMask) &&
           (txPort->dataEndPos.byteNumber == txPort->buffer.pointer.byteNumber);
}

/**
 * Evaluates to true if the buffer bit pointer points beyond the last buffer bit.
 * @param bufferBitPointer the pointer to compare to
 */
bool isBufferEndPosition(const BufferBitPointer *const bufferBitPointer) {
    return (bufferBitPointer->byteNumber == (sizeof(((PortBuffer *const) (0))->bytes))) &&
           (bufferBitPointer->bitMask == 0x1);
}
