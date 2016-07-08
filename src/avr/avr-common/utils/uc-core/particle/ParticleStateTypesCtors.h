/*
 * @author Raoul Rubien 2015
 */
#pragma once

#include "ParticleStateTypes.h"
#include "uc-core/communication/CommunicationTypesCtors.h"
#include "uc-core/communication-protocol/CommunicationProtocolTypesCtors.h"

extern CTOR_ATTRS void constructDiscoveryPulseCounter(volatile DiscoveryPulseCounter *o);
/**
 * constructor function
 */
CTOR_ATTRS void constructDiscoveryPulseCounter(volatile DiscoveryPulseCounter *o) {
    o->counter = 0;
    o->isConnected = false;
//    *(uint8_t *) o = 0;
}

extern CTOR_ATTRS void constructDiscoveryPulseCounters(volatile DiscoveryPulseCounters *o);

CTOR_ATTRS void constructDiscoveryPulseCounters(volatile DiscoveryPulseCounters *o) {
    constructDiscoveryPulseCounter(&(o->north));
    constructDiscoveryPulseCounter(&(o->east));
    constructDiscoveryPulseCounter(&(o->south));
    o->loopCount = 0;
}

extern CTOR_ATTRS void constructNodeAddress(volatile NodeAddress *o);
/**
 * constructor function
 */
CTOR_ATTRS void constructNodeAddress(volatile NodeAddress *o) {
//    o->row = 1;
//    o->column = 1;
    *((uint16_t *) o) = 0x0000;
}

extern CTOR_ATTRS void constructNode(volatile Node *o);

CTOR_ATTRS void constructNode(volatile Node *o) {
    o->state = STATE_TYPE_UNDEFINED;
    o->type = NODE_TYPE_INVALID;
    constructNodeAddress(&(o->address));
}

extern CTOR_ATTRS void constructPeriphery(volatile Periphery *o);
/**
 * constructor function
 */
CTOR_ATTRS void constructPeriphery(volatile Periphery *o) {
    o->loopCount = 0;
}

extern CTOR_ATTRS void constructParticle(volatile Particle *o);
/**
 * constructor function
 */
CTOR_ATTRS void constructParticle(volatile Particle *o) {
    constructNode(&(o->node));
    constructDiscoveryPulseCounters(&(o->discoveryPulseCounters));
    constructCommunication(&(o->communication));
    constructPeriphery(&(o->periphery));
    constructCommunicationProtocol(&o->protocol);
#ifdef SIMULATION
    o->magicEndByte = 0xaa;
#endif
}
