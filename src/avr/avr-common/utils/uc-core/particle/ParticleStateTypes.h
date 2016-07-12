/*
 * @author Raoul Rubien 2015
 */

#pragma once

#include <stdint.h>
#include "uc-core/communication/CommunicationTypes.h"
#include "uc-core/communication-protocol/CommunicationProtocolTypes.h"
#include "uc-core/actuation/ActuationTypes.h"

/**
 * Possible particle's state machine states are listed in this enum.
 */
typedef enum StateType {
    // uninitialized state
            STATE_TYPE_UNDEFINED = 0,
    // state when particle is initializing
            STATE_TYPE_START,
    // state when particle is fully initialized
            STATE_TYPE_ACTIVE,

    // state when evaluating discovery pulses
            STATE_TYPE_NEIGHBOURS_DISCOVERY,
    // state wen discovery ended
            STATE_TYPE_NEIGHBOURS_DISCOVERED,
    // state of discovery pulsing period post discovery
            STATE_TYPE_DISCOVERY_PULSING,
    // state when discovery pulsing ended
            STATE_TYPE_DISCOVERY_PULSING_DONE,
    // state after reset
            STATE_TYPE_RESET,

    // state when waiting for /receiving local address from parent neighbor
            STATE_TYPE_WAIT_FOR_BEING_ENUMERATED,
    // state when local address is assigned successfully
            STATE_TYPE_LOCALLY_ENUMERATED,
    // state when starting neighbour enumeration
            STATE_TYPE_ENUMERATING_NEIGHBOURS,
    // state wen assigning network address to east neighbour
            STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR,
    // state when east enumeration finished
            STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE,
    // state when assigning network address to south neighbour
            STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR,
    // state when south enumeration finished
            STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE,
    // state when neighbour enumeration finished
            STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE,

    // state when last particle sends local address to the origin
            STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY,
    // state when relaying the network address announcement to origin
            STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY,
    // state when relaying relay is finished
            STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY_DONE,
    // state when announcing network geometry is finished
            STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_DONE,

    // state when origin node sends local time to neighbours
            STATE_TYPE_SYNC_NEIGHBOUR,
    // state when origin sending local time to neighbours has finished
            STATE_TYPE_SYNC_NEIGHBOUR_DONE,

    // working state when origin broadcasts a new network geometry
            STATE_TYPE_SEND_SET_NETWORK_GEOMETRY,

    // working state when waiting for commands or executing scheduled tasks
            STATE_TYPE_IDLE,
    // erroneous machine state
            STATE_TYPE_ERRONEOUS,
    // dead lock state; usually before shutdown
            STATE_TYPE_STALE,

    // state when before MCU goes int sleep mode
            STATE_TYPE_SLEEP_MODE,
} StateType;

/**
 * The node type describes node type according to the connectivity detected when discovery process
 * finished.
 */
typedef enum NodeType {
    // invalid or uninitialized note
            NODE_TYPE_INVALID = 0,
    // not connected node
            NODE_TYPE_ORPHAN,
    // node connected at south or south and east
            NODE_TYPE_ORIGIN,
    // node connected at north and south and east
            NODE_TYPE_INTER_HEAD,
    // node connected at north and south
            NODE_TYPE_INTER_NODE,
    // node connected at north
            NODE_TYPE_TAIL,
    // for testing purposes when the node is attached to the NODE_TYPE_ORIGIN node
            NODE_TYPE_MASTER
} NodeType;

/**
 * The dsicovery pulse counter stores the amount of discovery pulses and the connectivity state.
 */
typedef struct DiscoveryPulseCounter {
    // discovery pulse counter
    uint8_t counter : 4;
    // connectivity flag
    uint8_t isConnected : 1;
    uint8_t __pad : 3;
} DiscoveryPulseCounter;

/**
 * Stores the amount of incoming pulses for each communication channel. The isConnected flags are set
 * if the number of incoming pulses exceeds a specific threshold.
 */
typedef struct DiscoveryPulseCounters {
    DiscoveryPulseCounter north;
    DiscoveryPulseCounter east;
    DiscoveryPulseCounter south;
    // discovery loop counter
    uint8_t loopCount;
} DiscoveryPulseCounters;

/**
 * The node address in the network. The origin node is addressed as row=1, column=1.
 * Address (0,0) is reserved.
 */
typedef struct NodeAddress {
    uint8_t row;
    uint8_t column;
} NodeAddress;


/**
 * Describes the node state type and address.
 */
typedef struct Node {
    /**
     * Describes the global node states.
     */
    StateType state;
    NodeType type;
    NodeAddress address;
} Node;


/**
 * Counters/resources needed for non vital platform's periphery such as LEDs, test points and alike.
 */
typedef struct Periphery {
    // particle main loop counter
    uint8_t loopCount;
} Periphery;

/**
 * keeps adjustment attributes for the internal 16bit timer/counter
 */
typedef struct TimerCounterAdjustment {
    uint16_t localTime;
    uint16_t remoteTime;
    uint16_t counterOffset;
    int8_t isPositiveCounterOffset : 1;
    int8_t isCounterOffsetValid : 1;
    int8_t __pad : 6;
} TimerCounterAdjustment;

/**
 * The global particle structure containing buffers, states, counters and alike.
 */
typedef struct Particle {
#ifdef SIMULATION
    // a marker used to assure the correct interpretation of the particle structure when simulating
    uint8_t __structStartMarker;
#endif
    Node node;
//    TimerCounterAdjustment timerCounterAdjustment;
    DiscoveryPulseCounters discoveryPulseCounters;
    Communication communication;
    Periphery periphery;
    CommunicationProtocol protocol;
    ActuationCommand actuationCommand;
#ifdef SIMULATION
    // a marker used to assure the correct interpretation of the particle structure when simulating
    uint8_t __structEndMarker;
#endif
} Particle;


