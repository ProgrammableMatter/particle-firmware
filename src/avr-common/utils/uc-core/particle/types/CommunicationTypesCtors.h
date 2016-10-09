/*
 * @author Raoul Rubien 09.10.2016
 *
 * Communication types constructor implementation.
 */

#pragma once

#include "CommunicationTypes.h"
#include "uc-core/particle/PointerImplementation.h"
#include "uc-core/communication/PointerImplementation.h"

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
