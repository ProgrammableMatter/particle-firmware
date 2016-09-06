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
//
///**
// * constructor function
// * @param o the object to construct
// */
//extern void constructTimerCounterAdjustment(volatile TimerCounterAdjustment *o);
//
//void constructTimerCounterAdjustment(volatile TimerCounterAdjustment *o) {
//    o->counterOffset = 0;
//    o->isPositiveCounterOffset = true;
//    o->isCounterOffsetValid = false;
//}

/**
 * constructor function
 * @param o the object to construct
 */
extern CTOR_ATTRS void constructDiscoveryPulseCounter(volatile DiscoveryPulseCounter *o);

CTOR_ATTRS void constructDiscoveryPulseCounter(volatile DiscoveryPulseCounter *o) {
    o->counter = 0;
    o->isConnected = false;
}

/**
 * constructor function
 * @param o the object to construct
 */
extern CTOR_ATTRS void constructDiscoveryPulseCounters(volatile DiscoveryPulseCounters *o);

CTOR_ATTRS void constructDiscoveryPulseCounters(volatile DiscoveryPulseCounters *o) {
    constructDiscoveryPulseCounter(&(o->north));
    constructDiscoveryPulseCounter(&(o->east));
    constructDiscoveryPulseCounter(&(o->south));
    o->loopCount = 0;
}

/**
 * constructor function
 * @param o the object to construct
 */
extern CTOR_ATTRS void constructNodeAddress(volatile NodeAddress *o);

CTOR_ATTRS void constructNodeAddress(volatile NodeAddress *o) {
    *((uint16_t *) o) = 0x0000;
}

/**
 * constructor function
 * @param o the object to construct
 */
extern CTOR_ATTRS void constructNode(volatile Node *o);

CTOR_ATTRS void constructNode(volatile Node *o) {
    o->state = STATE_TYPE_UNDEFINED;
    o->type = NODE_TYPE_INVALID;
    constructNodeAddress(&(o->address));
}

/**
 * constructor function
 * @param o the object to construct
 */
extern CTOR_ATTRS void constructPeriphery(volatile Periphery *o);

CTOR_ATTRS void constructPeriphery(volatile Periphery *o) {
    o->loopCount = 0;
}

/**
 * constructor function
 * @param o the object to construct
 */
extern CTOR_ATTRS void constructDirectionOrientedPort(volatile DirectionOrientedPort *o,
                                                      volatile DiscoveryPulseCounter *discoveryPulseCounter,
                                                      volatile TxPort *txPort,
                                                      volatile RxPort *rxPort,
                                                      void (receivePimpl)(void),
                                                      void (txHighPimpl)(void),
                                                      void (txLowPimpl)(void),
                                                      volatile CommunicationProtocolPortState *protocolState);

void CTOR_ATTRS constructDirectionOrientedPort(volatile DirectionOrientedPort *o,
                                               volatile DiscoveryPulseCounter *discoveryPulseCounter,
                                               volatile TxPort *txPort,
                                               volatile RxPort *rxPort,
                                               void (receivePimpl)(void),
                                               void (txHighPimpl)(void),
                                               void (txLowPimpl)(void),
                                               volatile CommunicationProtocolPortState *protocolState) {
    o->discoveryPulseCounter = (DiscoveryPulseCounter *) discoveryPulseCounter;
    o->rxPort = (RxPort *) rxPort;
    o->txPort = (TxPort *) txPort;
    o->receivePimpl = receivePimpl;
    o->txHighPimpl = txHighPimpl;
    o->txLowPimpl = txLowPimpl;
    o->protocol = (CommunicationProtocolPortState *) protocolState;
}

/**
 * constructor function
 * @param o the object to construct
 */
extern void CTOR_ATTRS constructDirectionOrientedPorts(volatile DirectionOrientedPorts *o);

void CTOR_ATTRS constructDirectionOrientedPorts(volatile DirectionOrientedPorts *o) {
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
extern CTOR_ATTRS void constructParticle(volatile Particle *o);

CTOR_ATTRS void constructParticle(volatile Particle *o) {
    constructNode(&(o->node));
    constructDiscoveryPulseCounters(&o->discoveryPulseCounters);
    constructCommunication(&o->communication);
    constructPeriphery(&o->periphery);
    constructCommunicationProtocol(&o->protocol);
    constructActuationCommand(&o->actuationCommand);
    constructLocalTimeTracking(&o->localTime);
    constructDirectionOrientedPorts(&o->directionOrientedPorts);
#ifdef SIMULATION
    o->__structStartMarker = 0xaa;
    o->__structEndMarker = 0xaa;
#endif
}
