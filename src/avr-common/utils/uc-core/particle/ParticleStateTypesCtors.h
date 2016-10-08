/*
 * @author Raoul Rubien 2016
 *
 * Particle types constructor implementation.
 */

#pragma once

#include "ParticleStateTypes.h"
#include "uc-core/communication/CommunicationTypesCtors.h"
#include "uc-core/communication-protocol/CommunicationProtocolTypesCtors.h"
#include "uc-core/actuation/ActuationTypesCtors.h"
#include "uc-core/time/TimeTypesCtors.h"
#include "PointerImplementation.h"
#include "uc-core/communication/PointerImplementation.h"
#include "uc-core/periphery/PeripheryTypesCtors.h"
#include "uc-core/synchronization/SynchronizationTypesCtors.h"

/**
 * constructor function
 * @param o the object to construct
 */
void constructDiscoveryPulseCounter(DiscoveryPulseCounter *const o) {
    o->counter = 0;
    o->isConnected = false;
}

/**
 * constructor function
 * @param o the object to construct
 */
void constructDiscoveryPulseCounters(DiscoveryPulseCounters *const o) {
    constructDiscoveryPulseCounter(&o->north);
    constructDiscoveryPulseCounter(&o->east);
    constructDiscoveryPulseCounter(&o->south);
    o->loopCount = 0;
}

/**
 * constructor function
 * @param o the object to construct
 */
void constructNodeAddress(NodeAddress *const o) {
    o->row = 0;
    o->column = 0;
}

/**
 * constructor function
 * @param o the object to construct
 */
void constructNode(Node *o) {
    o->state = STATE_TYPE_UNDEFINED;
    o->type = NODE_TYPE_INVALID;
    constructNodeAddress(&o->address);
}

/**
 * constructor function
 * @param o the object to construct
 */
void constructDirectionOrientedPort(DirectionOrientedPort *const o,
                                    DiscoveryPulseCounter *const discoveryPulseCounter,
                                    TxPort *const txPort,
                                    RxPort *const rxPort,
                                    void (*const receivePimpl)(void),
                                    void (*const txHighPimpl)(void),
                                    void (*const txLowPimpl)(void),
                                    CommunicationProtocolPortState *const protocolState) {
    o->discoveryPulseCounter = discoveryPulseCounter;
    o->rxPort = rxPort;
    o->txPort = txPort;
    o->receivePimpl = receivePimpl;
    o->txHighPimpl = txHighPimpl;
    o->txLowPimpl = txLowPimpl;
    o->protocol = protocolState;
}

/**
 * constructor function
 * @param o the object to construct
 */
void constructDirectionOrientedPorts(DirectionOrientedPorts *const o) {
    constructDirectionOrientedPort(&o->north,
                                   &ParticleAttributes.discoveryPulseCounters.north,
                                   &ParticleAttributes.communication.ports.tx.north,
                                   &ParticleAttributes.communication.ports.rx.north,
                                   receiveNorth,
                                   northTxHiImpl,
                                   northTxLoImpl,
                                   &ParticleAttributes.protocol.ports.north);
    constructDirectionOrientedPort(&o->east,
                                   &ParticleAttributes.discoveryPulseCounters.east,
                                   &ParticleAttributes.communication.ports.tx.east,
                                   &ParticleAttributes.communication.ports.rx.east,
                                   receiveEast,
                                   eastTxHiImpl,
                                   eastTxLoImpl,
                                   &ParticleAttributes.protocol.ports.east);
    constructDirectionOrientedPort(&o->south,
                                   &ParticleAttributes.discoveryPulseCounters.south,
                                   &ParticleAttributes.communication.ports.tx.south,
                                   &ParticleAttributes.communication.ports.rx.south,
                                   receiveSouth,
                                   southTxHiImpl,
                                   southTxLoImpl,
                                   &ParticleAttributes.protocol.ports.south);
    constructDirectionOrientedPort(&o->simultaneous,
                                   &ParticleAttributes.discoveryPulseCounters.east,
                                   &ParticleAttributes.communication.ports.tx.east,
                                   &ParticleAttributes.communication.ports.rx.east,
                                   receiveEast,
                                   simultaneousTxHiImpl,
                                   simultaneousTxLoImpl,
                                   &ParticleAttributes.protocol.ports.east);
}


/**
 * constructor function
 * @param o the object to construct
 */
void constructAlerts(Alerts *const o) {
    o->isRxBufferOverflowEnabled = false;
    o->isRxParityErorEnabled = false;
    o->isGenericErrorEnabled = false;
}

/**
 * constructor function
 * @param o the object to construct
 */
void constructParticle(Particle *const o) {
    constructNode(&(o->node));
    constructDiscoveryPulseCounters(&o->discoveryPulseCounters);
    constructCommunication(&o->communication);
    constructPeriphery(&o->periphery);
    constructCommunicationProtocol(&o->protocol);
    constructActuationCommand(&o->actuationCommand);
    constructTimeSynchronization(&o->timeSynchronization);
    constructLocalTimeTracking(&o->localTime);
    constructDirectionOrientedPorts(&o->directionOrientedPorts);
    constructAlerts(&o->alerts);
#ifdef SIMULATION
    o->__structStartMarker = 0xaa;
    o->__structEndMarker = 0xaa;
#endif
}
