/*
 * @author Raoul Rubien 2015
 */
#pragma once

#include "ParticleStateTypes.h"
#include "uc-core/communication/CommunicationTypesCtors.h"

extern FUNC_ATTRS void constructPulseCounter(volatile PulseCounter *o);

FUNC_ATTRS void constructPulseCounter(volatile PulseCounter *o) {
    o->counter = 0;
    o->isConnected = false;
//    *(uint8_t *) o = 0;
}

extern FUNC_ATTRS void constructDiscoveryPulseCounters(volatile DiscoveryPulseCounters *o);

FUNC_ATTRS void constructDiscoveryPulseCounters(volatile DiscoveryPulseCounters *o) {
    constructPulseCounter(&(o->north));
    constructPulseCounter(&(o->east));
    constructPulseCounter(&(o->south));
    o->loopCount = 0;
}

extern FUNC_ATTRS void constructNodeAddress(volatile NodeAddress *o);

FUNC_ATTRS void constructNodeAddress(volatile NodeAddress *o) {
//    o->row = 1;
//    o->column = 1;
    *((uint16_t *) o) = 0x0000;
}

extern FUNC_ATTRS void constructNode(volatile Node *o);

FUNC_ATTRS void constructNode(volatile Node *o) {
    o->state = STATE_TYPE_UNDEFINED;
    o->type = NODE_TYPE_INVALID;
    constructNodeAddress(&(o->address));
}

extern FUNC_ATTRS void constructPeriphery(volatile Periphery *o);

FUNC_ATTRS void constructPeriphery(volatile Periphery *o) {
    o->loopCount = 0;
}

extern FUNC_ATTRS void constructParticleState(volatile ParticleState *o);

FUNC_ATTRS void constructParticleState(volatile ParticleState *o) {
    constructNode(&(o->node));
    constructDiscoveryPulseCounters(&(o->discoveryPulseCounters));
    constructPorts(&(o->ports));
    constructPeriphery(&(o->periphery));
#ifdef SIMULATION
    o->magicEndByte = 0xaa;
#endif
}
