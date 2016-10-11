/*
 * @author Raoul Rubien 2016
 *
 * Particle types constructor implementation.
 */

#pragma once

#include "ParticleTypes.h"
#include "AlertsTypesCtors.h"
#include "DiscoveryPulseCountersCtors.h"
#include "CommunicationTypesCtors.h"
#include "uc-core/communication/CommunicationTypesCtors.h"
#include "uc-core/communication-protocol/CommunicationProtocolTypesCtors.h"
#include "uc-core/actuation/ActuationTypesCtors.h"
#include "uc-core/time/TimeTypesCtors.h"
#include "uc-core/periphery/PeripheryTypesCtors.h"
#include "uc-core/synchronization/SynchronizationTypesCtors.h"
#include "uc-core/scheduler/SchedulerTypesCtors.h"

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
    constructScheduler(&o->scheduler);
#ifdef SIMULATION
    o->__structStartMarker = 0xaa;
    o->__structEndMarker = 0xaa;
#endif
}
