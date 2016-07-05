//
// Created by Raoul Rubien on 11.05.16
//

/**
 * implementation of tx/rx protocol package functions
 */
#pragma once

#include "uc-core/particle/Globals.h"
#include "CommunicationProtocolTypes.h"
#include "simulation/SimulationMacros.h"
#include "uc-core/configuration/CommunicationProtocol.h"
/**
 * Sets the data end pointer to the specified position. For optimization purpose the struct is treated
 * as uint16_t where the end position is
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
    DEBUG_CHAR_OUT('c');
    ParticleAttributes.communication.ports.rx.north.isDataBuffered = false;
    ParticleAttributes.communication.ports.rx.north.isOverflowed = false;
    ParticleAttributes.communication.ports.rx.east.isDataBuffered = false;
    ParticleAttributes.communication.ports.rx.east.isOverflowed = false;
    ParticleAttributes.communication.ports.rx.south.isDataBuffered = false;
    ParticleAttributes.communication.ports.rx.south.isOverflowed = false;
}

extern FUNC_ATTRS void clearReceptionBuffer(volatile RxPort *o);
/**
 * invalidates the given port's reception buffer
 */
FUNC_ATTRS void clearReceptionBuffer(volatile RxPort *o) {
    o->isDataBuffered = false;
    o->isOverflowed = false;
    DEBUG_CHAR_OUT('c');
}

extern FUNC_ATTRS void clearTransmissionPortBuffer(volatile TxPort *o);
/**
 * prepares the given transmission port for buffering and later transmission
 */
FUNC_ATTRS void clearTransmissionPortBuffer(volatile TxPort *o) {
    o->isTransmitting = false;
    bufferBitPointerStart(&o->buffer.pointer);
}

extern FUNC_ATTRS void setReceptionistStateStart(volatile CommunicationProtocolPortState *commPortState);
/**
 * Puts the receptionist in start state and sets the timeout counter.
 */
FUNC_ATTRS void setReceptionistStateStart(volatile CommunicationProtocolPortState *commPortState) {
//    ParticleAttributes.communicationProtocol.stateTimeoutCounter = COMMUNICATION_STATE_TIMEOUT_COUNTER;
    DEBUG_CHAR_OUT('r');
    commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE;
    commPortState->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
}

extern FUNC_ATTRS void setInitiatorStateStart(volatile CommunicationProtocolPortState *commPortState);
/**
 * Puts the initiator in start state and set the timeout counter.
 */
FUNC_ATTRS void setInitiatorStateStart(volatile CommunicationProtocolPortState *commPortState) {
    DEBUG_CHAR_OUT('T');
    commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT;
    commPortState->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
}
