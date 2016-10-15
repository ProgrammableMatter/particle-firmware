/*
 * @author Raoul Rubien 2016
 *
 * Particle state definition.
 */

#pragma once

#include <stdint.h>
#include "uc-core/particle/types/NodeAddressTypes.h"
#include "uc-core/actuation/ActuationTypes.h"
#include "uc-core/time/TimeTypes.h"
#include "uc-core/periphery/PeripheryTypes.h"
#include "uc-core/synchronization/SynchronizationTypes.h"
#include "uc-core/particle/types/AlertsTypes.h"
#include "uc-core/particle/types/DiscoveryPulseCountersTypes.h"
#include "uc-core/particle/types/CommunicationTypes.h"
#include "uc-core/scheduler/SchedulerTypes.h"
#include "uc-core/particle/types/ParticleStateTypes.h"
#include "uc-core/evaluation/EvaluationTypes.h"

/**
 * Describes the node state type and address.
 */
typedef struct Node {
    /**
     * Describes the global node states.
     */
    volatile StateType state;
    volatile NodeType type;
    NodeAddress address;
} Node;

/**
 * The global particle structure containing buffers, states, counters and alike.
 */
typedef struct Particle {
#ifdef SIMULATION
    // a marker used to assure the correct interpretation of the particle structure when simulating
    uint8_t __structStartMarker;
#endif
    /**
     * Node type, state and address fields.
     */
    Node node;
    /**
     * Node connectivity settings and states.
     */
    DiscoveryPulseCounters discoveryPulseCounters;
    /**
     * Communication (physical layer) related states and buffers.
     */
    Communication communication;
    /**
     * Resources needed for non-essential periphery implementation such as LEDs, test points and alike.
     */
    Periphery periphery;
    /**
     * Communication protocol (layer 1) related settings and states.
     */
    CommunicationProtocol protocol;
    /**
     * Settings related to actuation command.
     */
    ActuationCommand actuationCommand;

    /**
     * clock skew adjustment
     */
    TimeSynchronization timeSynchronization;
    /**
     * Local time and settings are stored to this field.
     */
    LocalTimeTracking localTime;
    /**
     * Facade to bundle the DiscoveryPulseCounters, Communication and CommunicationProtocol port
     * related resources in a direction oriented way.
     */
    DirectionOrientedPorts directionOrientedPorts;
    /**
     * flags arming/disarming alerts on run time
     */
    Alerts alerts;
    /**
     * Simple scheduler to plan and execute tasks from the main loop.
     */
    Scheduler scheduler;
    /**
     * Evaluation relevant fields. Can be removed in productive application.
     */
    Evaluation evaluation;
#ifdef SIMULATION
    // a marker used to assure the correct interpretation of the particle structure when simulating
    uint8_t __structEndMarker;
#endif
} Particle;
