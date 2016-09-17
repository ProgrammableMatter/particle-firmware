/**
 * Created by Raoul Rubien on 11.05.2016
 *
 * Protocol tx/rx relevant implementation.
 */
#pragma once

#include "uc-core/particle/Globals.h"
#include "CommunicationProtocolTypes.h"
#include "simulation/SimulationMacros.h"
#include "uc-core/configuration/CommunicationProtocol.h"

/**
 * Sets the data end pointer to the specified position.
 * For optimization purpose the pointer struct is casted to uint16_t.
 * @param bufferDataEndPointer the pointer field
 * @param uint16tNewDataEndPointer the new pointer field value
 */
#define setBufferDataEndPointer(bufferDataEndPointer, uint16tNewDataEndPointer) \
    (*((uint16_t *) &bufferDataEndPointer) = uint16tNewDataEndPointer)

/**
 * Evaluates to true if the buffered data's end position equals the specified end position,
 * to false otherwise.
 * For optimization purpose the pointer struct is casted to uint16_t.
 * @param bufferDataEndPointer the pointer field
 * @param uint16tExpectedDataEndPointer the expected pointer field value
 */
#define equalsPackageSize(bufferDataEndPointer, uint16tExpectedDataEndPointer) \
    (*((uint16_t *) &bufferDataEndPointer) == uint16tExpectedDataEndPointer)

/**
 * invalidates all reception buffers
 */
extern FUNC_ATTRS void clearReceptionBuffers(void);

FUNC_ATTRS void clearReceptionBuffers(void) {
    DEBUG_CHAR_OUT('c');
    ParticleAttributes.communication.ports.rx.north.isDataBuffered = false;
    ParticleAttributes.communication.ports.rx.north.isOverflowed = false;
    ParticleAttributes.communication.ports.rx.east.isDataBuffered = false;
    ParticleAttributes.communication.ports.rx.east.isOverflowed = false;
    ParticleAttributes.communication.ports.rx.south.isDataBuffered = false;
    ParticleAttributes.communication.ports.rx.south.isOverflowed = false;
}

/**
 * Invalidates the given port's reception buffer.
 * @param o the port to invalidate the reception buffer
 */
extern FUNC_ATTRS void clearReceptionPortBuffer(volatile RxPort *o);

FUNC_ATTRS void clearReceptionPortBuffer(volatile RxPort *o) {
    o->isDataBuffered = false;
    o->isOverflowed = false;
    DEBUG_CHAR_OUT('c');
}

/**
 * Prepares the given transmission port for buffering and later transmission.
 * @param o the port to prepare
 */
extern FUNC_ATTRS void clearTransmissionPortBuffer(volatile TxPort *o);

FUNC_ATTRS void clearTransmissionPortBuffer(volatile TxPort *o) {
    o->isTransmitting = false;
    bufferBitPointerStart(&o->buffer.pointer);
}

/**
 * Puts the receptionist in start state and sets the timeout counter.
 * @param commPortState a reference to the designated port state
 */
extern FUNC_ATTRS void setReceptionistStateStart(volatile CommunicationProtocolPortState *commPortState);

FUNC_ATTRS void setReceptionistStateStart(volatile CommunicationProtocolPortState *commPortState) {
    // ParticleAttributes.communicationProtocol.stateTimeoutCounter = COMMUNICATION_STATE_TIMEOUT_COUNTER;
    DEBUG_CHAR_OUT('r');
    commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE;
    commPortState->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
    commPortState->reTransmissions = COMMUNICATION_PROTOCOL_RETRANSMISSION_COUNTER_MAX;
}

/**
 * Puts the initiator in start state and set the timeout counter.
 * @param commPortState a reference to the designated port state
 */
extern FUNC_ATTRS void setInitiatorStateStart(volatile CommunicationProtocolPortState *commPortState);

FUNC_ATTRS void setInitiatorStateStart(volatile CommunicationProtocolPortState *commPortState) {
    DEBUG_CHAR_OUT('T');
    commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT;
    commPortState->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
    commPortState->reTransmissions = COMMUNICATION_PROTOCOL_RETRANSMISSION_COUNTER_MAX;
}
