/**
 * Created by Raoul Rubien on 11.05.2016
 *
 * Communication protocol types constructor implementation.
 */

#pragma once

#include "uc-core/particle/Globals.h"
#include "./CommunicationProtocol.h"
#include "./CommunicationProtocolTypes.h"

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructCommunicationProtocolPortState(CommunicationProtocolPortState *const o);

CTOR_ATTRS void constructCommunicationProtocolPortState(CommunicationProtocolPortState *const o) {
    o->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
    o->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE;
    o->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
    o->reTransmissions = COMMUNICATION_PROTOCOL_RETRANSMISSION_COUNTER_MAX;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructCommunicationProtocolPorts(CommunicationProtocolPorts *const o);

CTOR_ATTRS void constructCommunicationProtocolPorts(CommunicationProtocolPorts *const o) {
    constructCommunicationProtocolPortState(&o->north);
    constructCommunicationProtocolPortState(&o->east);
    constructCommunicationProtocolPortState(&o->south);
}


/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructNetworkGeometry(NetworkGeometry *const o);

CTOR_ATTRS void constructNetworkGeometry(NetworkGeometry *const o) {
    o->rows = 0;
    o->columns = 0;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructCommunicationProtocol(CommunicationProtocol *const o);

CTOR_ATTRS void constructCommunicationProtocol(CommunicationProtocol *const o) {
    constructCommunicationProtocolPorts(&o->ports);
    constructNetworkGeometry(&o->networkGeometry);
    o->hasNetworkGeometryDiscoveryBreadCrumb = false;
    o->isBroadcastEnabled = false;
    o->isSimultaneousTransmissionEnabled = false;
}
