//
// Created by Raoul Rubien on 11.05.16
//

/**
 * implementation of tx/rx protocol package functions
 */
#pragma once

#include "uc-core/particle/Globals.h"
#include "./CommunicationProtocol.h"
#include "./CommunicationProtocolTypes.h"


extern CTOR_ATTRS void constructCommunicationProtocolPortState(volatile CommunicationProtocolPortState *o);

CTOR_ATTRS void constructCommunicationProtocolPortState(volatile CommunicationProtocolPortState *o) {
    o->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
    o->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE;
    o->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
}


extern CTOR_ATTRS void constructCommunicationProtocolPorts(volatile CommunicationProtocolPorts *o);

CTOR_ATTRS void constructCommunicationProtocolPorts(volatile CommunicationProtocolPorts *o) {
    constructCommunicationProtocolPortState(&o->north);
    constructCommunicationProtocolPortState(&o->east);
    constructCommunicationProtocolPortState(&o->south);
}

extern CTOR_ATTRS void constructCommunicationProtocol(volatile CommunicationProtocol *o);

CTOR_ATTRS void constructCommunicationProtocol(volatile CommunicationProtocol *o) {
    constructCommunicationProtocolPorts(&o->ports);
}
