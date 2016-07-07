/**
 * @author Raoul Rubien 2016
 */


#pragma once

#include "CommunicationTypes.h"
#include "ManchesterDecodingTypes.h"

//extern FUNC_ATTRS bool isDataAvailable(volatile RxPort *o);
///**
// * returns true if data on the specified is available, false otherwise
// */
//FUNC_ATTRS bool isDataAvailable(volatile RxPort *o) {
//    return o->isDataBuffered;
//}

extern FUNC_ATTRS void bufferBitPointerIncrement(volatile BufferBitPointer *o);
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

extern FUNC_ATTRS void bufferBitPointerStart(volatile BufferBitPointer *o);
/**
 * points the reception buffer pointer to the start position (lowest bit)
 */
FUNC_ATTRS void bufferBitPointerStart(volatile BufferBitPointer *o) {
    o->bitMask = 1;
    o->byteNumber = 0;
}

extern FUNC_ATTRS bool isDataEndPosition(volatile TxPort *o);
/**
 * Returns true if the transmission buffer pointer points exactly at the data end marker.
 * The marker is set one bit beyond the last data bit.
 */
FUNC_ATTRS bool isDataEndPosition(volatile TxPort *o) {
    return o->dataEndPos.bitMask == o->buffer.pointer.bitMask &&
           o->dataEndPos.byteNumber == o->buffer.pointer.byteNumber;
}

//extern FUNC_ATTRS bool isBufferEndPosition(volatile BufferBitPointer *o);
/**
 * Returns true if the pointer points at exactly one bit beyond the buffer's last bit.
 */
//FUNC_ATTRS bool isBufferEndPosition(volatile BufferBitPointer *o) {
//    return (o->byteNumber == (sizeof(((PortBuffer *) (0))->bytes))) && (o->bitMask == 0x1);
//}

#define isBufferEndPosition(bufferBitPointer) \
    (((bufferBitPointer)->byteNumber == (sizeof(((PortBuffer *) (0))->bytes))) && \
    ((bufferBitPointer)->bitMask == 0x1))
