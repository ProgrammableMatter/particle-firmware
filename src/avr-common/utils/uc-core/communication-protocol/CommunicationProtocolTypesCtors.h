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
extern CTOR_ATTRS void constructCommunicationProtocolPortState(volatile CommunicationProtocolPortState *o);

CTOR_ATTRS void constructCommunicationProtocolPortState(volatile CommunicationProtocolPortState *o) {
    o->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
    o->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE;
    o->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructCommunicationProtocolPorts(volatile CommunicationProtocolPorts *o);

CTOR_ATTRS void constructCommunicationProtocolPorts(volatile CommunicationProtocolPorts *o) {
    constructCommunicationProtocolPortState(&o->north);
    constructCommunicationProtocolPortState(&o->east);
    constructCommunicationProtocolPortState(&o->south);
}


/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructNetworkGeometry(volatile NetworkGeometry *o);

CTOR_ATTRS void constructNetworkGeometry(volatile NetworkGeometry *o) {
    o->rows = 0;
    o->columns = 0;
}

/**
 * constructor function
 * @param o reference to the object to construct
 */
extern CTOR_ATTRS void constructCommunicationProtocol(volatile CommunicationProtocol *o);

CTOR_ATTRS void constructCommunicationProtocol(volatile CommunicationProtocol *o) {
    constructCommunicationProtocolPorts(&o->ports);
    constructNetworkGeometry(&o->networkGeometry);
    o->hasNetworkGeometryDiscoveryBreadCrumb = false;
    o->isBroadcastEnabled = false;
    o->isSimultaneousTransmissionEnabled = false;
}
