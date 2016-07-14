/**
 * @author Raoul Rubien 2016
 */

#pragma once

#include "ManchesterDecodingTypes.h"

extern FUNC_ATTRS void __rectifyTransmissionBit(volatile DirectionOrientedPort *port);
/**
 * rectifies/modulates the transmission signal according to the upcoming bit
 */
FUNC_ATTRS void __rectifyTransmissionBit(volatile DirectionOrientedPort *port) {

    if (isDataEndPosition(port->txPort)) { // on tx pointer match end position
        port->txLowPimpl(); // return signal to default (inverted at receiver side)
        port->txPort->isTransmitting = false;
    } else {
        if (port->txPort->buffer.pointer.bitMask &
            port->txPort->buffer.bytes[port->txPort->buffer.pointer.byteNumber]) {
            port->txHighPimpl();
        } else {
            port->txLowPimpl();
        }
    }
}

extern FUNC_ATTRS void __modulateTransmissionBit(volatile DirectionOrientedPort *port);
/**
 * modulates the transmission signal according to the current bit and increments the buffer pointer
 */
FUNC_ATTRS void __modulateTransmissionBit(volatile DirectionOrientedPort *port) {
    if (port->txPort->buffer.pointer.bitMask &
        port->txPort->buffer.bytes[port->txPort->buffer.pointer.byteNumber]) {
        port->txLowPimpl();
    } else {
        port->txHighPimpl();
    }
    bufferBitPointerIncrement(&port->txPort->buffer.pointer);
}

extern FUNC_ATTRS void transmit(volatile DirectionOrientedPort *port);
/**
 * puts the the next signal on the pin
 */
FUNC_ATTRS void transmit(volatile DirectionOrientedPort *port) {
    if (!port->txPort->isDataBuffered || !port->txPort->isTransmitting) {
        return;
    }

    if (port->txPort->isTxClockPhase) {
        __rectifyTransmissionBit(port);
    } else {
        __modulateTransmissionBit(port);
    }
    port->txPort->isTxClockPhase++;
}