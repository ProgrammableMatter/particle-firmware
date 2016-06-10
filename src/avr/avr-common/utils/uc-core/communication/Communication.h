/**
 * @author Raoul Rubien 2016
 */


#pragma once

#include "CommunicationTypes.h"
#include "ManchesterDecodingTypes.h"

#ifdef TRY_INLINE
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif

/**
 * returns false on ongoing reception else true
 */
FUNC_ATTRS bool isNotReceiving(volatile RxPort *o) {
    return o->isReceiving == false;
}

/**
 * Increments the bit mask and the byte number accordingly. Does not verify the buffer boundary.
 */
FUNC_ATTRS void bufferBitPointerIncrement(volatile BufferBitPointer *o) {
    o->bitMask <<= 1;
    if ((o->bitMask == 0) && (o->byteNumber < (sizeof(((PortBuffer *) 0)->bytes)))) {
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
 * Returns true if the transmission buffer pointer points exactly at the data end marker.
 * The marker is set one bit beyond the last data bit.
 */
FUNC_ATTRS bool isDataEndPosition(volatile TxPort *o) {
    return o->dataEndPos.bitMask == o->buffer.pointer.bitMask &&
           o->dataEndPos.byteNumber == o->buffer.pointer.byteNumber;
}

/**
 * Returns true if the pointer points at exactly one bit beyond the buffer's last bit.
 */
FUNC_ATTRS bool isBufferEndPosition(volatile BufferBitPointer *o) {
    return o->byteNumber == (sizeof(((PortBuffer *) (0))->bytes)) &&
           o->bitMask == 0x1;

}

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif