/*
 * @author Raoul Rubien 2015
 */
#pragma once

#include "ParticleStateTypes.h"
#include "uc-core/communication/CommunicationTypesCtors.h"
#include "uc-core/communication-protocol/CommunicationProtocolTypesCtors.h"
#include "uc-core/actuation/ActuationTypesCtors.h"

extern void constructTimerCounterAdjustment(volatile TimerCounterAdjustment *o);

/**
 * constructor function
 */
void constructTimerCounterAdjustment(volatile TimerCounterAdjustment *o) {
    o->counterOffset = 0;
    o->isPositiveCounterOffset = true;
    o->isCounterOffsetValid = false;
}

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
//    constructTimerCounterAdjustment(&o->timerCounterAdjustment);
    constructDiscoveryPulseCounters(&o->discoveryPulseCounters);
    constructCommunication(&o->communication);
    constructPeriphery(&o->periphery);
    constructCommunicationProtocol(&o->protocol);
    constructActuationCommand(&o->actuationCommand);
#ifdef SIMULATION
    o->__structStartMarker = 0xaa;
    o->__structEndMarker = 0xaa;
#endif
}
