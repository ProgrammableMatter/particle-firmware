//
// Created by Raoul Rubien on 11.05.16
//

/**
 * implementation of tx/rx protocol package functions
 */
#pragma once

#include "uc-core/Globals.h"
#include "CommunicationProtocolTypes.h"


/**
 * Sets the data end pointer to the specified position. For optimization purpose the struct is treated
 * as uint16_t where the end poisition is
 */
#define setBufferDataEndPointer(bufferDataEndPointer, uint16tNewDataEndPointer) \
    (*((uint16_t *) &bufferDataEndPointer) = uint16tNewDataEndPointer)


#define equalsPackageSize(bufferDataEndPointer, uint16tExpectedDataEndPointer) \
    (*((uint16_t *) &bufferDataEndPointer) == uint16tExpectedDataEndPointer)


extern FUNC_ATTRS void clearReceptionBuffers(void);
/**
 * invalidates all reception buffers
 */
FUNC_ATTRS void clearReceptionBuffers(void) {
    ParticleAttributes.ports.rx.north.isDataBuffered = false;
//    ParticleAttributes.ports.rx.north.isReceiving = false;
    ParticleAttributes.ports.rx.north.isOverflowed = false;

    ParticleAttributes.ports.rx.east.isDataBuffered = false;
//    ParticleAttributes.ports.rx.east.isReceiving = false;
    ParticleAttributes.ports.rx.east.isOverflowed = false;

    ParticleAttributes.ports.rx.south.isDataBuffered = false;
//    ParticleAttributes.ports.rx.south.isReceiving = false;
    ParticleAttributes.ports.rx.south.isOverflowed = false;
}

extern FUNC_ATTRS void clearReceptionBuffer(volatile RxPort *o);
/**
 * invalidates the given port's reception buffer
 */
FUNC_ATTRS void clearReceptionBuffer(volatile RxPort *o) {
    o->isDataBuffered = false;
//    o->isReceiving = false;
    o->isOverflowed = false;
    DEBUG_CHAR_OUT('c');
}


extern FUNC_ATTRS void prepareTransmissionPortBuffer(volatile TxPort *o);
/**
 * prepares the given transmission port for buffering and later transmission
 */
FUNC_ATTRS void prepareTransmissionPortBuffer(volatile TxPort *o) {
    o->enableTransmission = false;
    o->retainTransmission = true;
    o->isTransmitting = false;
    bufferBitPointerStart(&o->buffer.pointer);
}

extern FUNC_ATTRS void releaseTransmissionPortBufferForTransmission(volatile TxPort *port);
/**
 * sets flags to enable transmission of the given port
 */
FUNC_ATTRS void releaseTransmissionPortBufferForTransmission(volatile TxPort *port) {
//    DELAY_US_150;
    port->retainTransmission = true;
    port->enableTransmission = true;
}


extern FUNC_ATTRS uint8_t isPortTransmitting(volatile TxPort *o);
/**
 * returns true if the given port is in transmission mode
 */
FUNC_ATTRS uint8_t isPortTransmitting(volatile TxPort *o) {
    return o->isTransmitting || o->enableTransmission;
}