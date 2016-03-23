/**
 * @author Raoul Rubien 2016
 */

#include "CommunicationTypes.h"
#include <stdbool.h>

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