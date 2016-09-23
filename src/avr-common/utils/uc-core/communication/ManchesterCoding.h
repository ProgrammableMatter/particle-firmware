/**
 * @author Raoul Rubien 2016
 *
 * Manchester coding related implementation.
 */

#pragma once

#include "ManchesterDecodingTypes.h"

/**
 * rectifies the transmission signal according to the upcoming bit
 * @param port the designated port to read buffered data and write signal to
 */
//extern FUNC_ATTRS void __rectifyTransmissionBit(volatile DirectionOrientedPort *port);

static FUNC_ATTRS void __rectifyTransmissionBit(volatile DirectionOrientedPort *port) {

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

/**
 * modulates the transmission signal according to the current bit and increments the buffer pointer
 * @param port the designated port to read buffered data from and write signal to
 */
//extern FUNC_ATTRS void __modulateTransmissionBit(volatile DirectionOrientedPort *port);

static FUNC_ATTRS void __modulateTransmissionBit(volatile DirectionOrientedPort *port) {
    if (port->txPort->buffer.pointer.bitMask &
        port->txPort->buffer.bytes[port->txPort->buffer.pointer.byteNumber]) {
        port->txLowPimpl();
    } else {
        port->txHighPimpl();
    }
    bufferBitPointerIncrement(&port->txPort->buffer.pointer);
}

/**
 * writes the next signal on the port pin
 * @param port the designated port to read buffered data from and write signal to
 */
extern FUNC_ATTRS void transmit(volatile DirectionOrientedPort *port);

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