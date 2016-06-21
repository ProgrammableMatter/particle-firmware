/*
 * @author Raoul Rubien 2015
 */

#pragma once

#include <stdint.h>
#include "uc-core/communication/CommunicationTypes.h"
#include "uc-core/communication-protocol/InterpreterTypes.h"

/**
 * Possible particle's state machine states are listed in this enum.
 */
typedef enum StateType {
    STATE_TYPE_UNDEFINED = 0, // uninitialized state
    STATE_TYPE_START, // particle is initialized
    STATE_TYPE_ACTIVE, // start neighbour discovery

    STATE_TYPE_NEIGHBOURS_DISCOVERY, // evaluates comm. port pulse counters
    STATE_TYPE_NEIGHBOURS_DISCOVERED, // discovery ended
    STATE_TYPE_DISCOVERY_PULSING, // keep pulsing post discovery
    STATE_TYPE_RESET, // state after reset command is received

    STATE_TYPE_WAIT_FOR_BEING_ENUMERATED, // wait for receiving a local address from parent/north port
    STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_SEND_ACK_RESPONSE_TO_PARENT, // send/ACK assigned address back
    STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_WAIT_UNTIL_ACK_RESPONSE_TO_PARENT_TRANSMISSION_FINISHED, // wait until transmission is finished
    STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_ACK_RESPONSE_FROM_PARENT, // wait for ACK or new address assignment
    STATE_TYPE_LOCALLY_ENUMERATED, // local address is assigned successfully

    STATE_TYPE_ENUMERATING_NEIGHBOURS, // starting neighbour enumeration

    STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR, // assigning network address to east neighbour
    STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_WAIT_UNTIL_TRANSMISSION_FINISHED, // wait until transmission is finished
    STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_ACK_RESPONSE_FROM_EAST, // wait for response from east with address
    STATE_TYPE_ENUMERATING_EAST_SEND_ACK_RESPONSE_TO_EAST, // send last ack to east
    STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_ACK_RESPONSE_TO_EAST_TRANSMISSION_FINISHED, // wait until transmission is finished
    STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE, // east enumeration finished

    STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR, // assigning network address to south neighbour
    STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_WAIT_UNTIL_TRANSMISSION_FINISHED, // wait until transmission is finished
    STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_FROM_SOUTH, // wait for response from south with address
    STATE_TYPE_ENUMERATING_SOUTH_SEND_ACK_RESPONSE_TO_SOUTH, // send last ack to south
    STATE_TYPE_ENUMERATING_SOUTH_SEND_ACK_RESPONSE_TO_SOUTH_WAIT_UNTIL_TRANSMISSION_FINISHED,
    STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_TO_SOUTH_TRANSMISSION_FINISHED, // wait until transmission is finished
    STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE, // south enumeration finished

    STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE, // neighbour enumeration finished

    STATE_TYPE_IDLE, // waiting for commands
    STATE_TYPE_INTERPRET_COMMAND, // interpret command
    STATE_TYPE_TX_START, // transmitting data in buffers
    STATE_TYPE_TX_DONE, // transmission done, buffer empty
    STATE_TYPE_FORWARD_PKG,
    STATE_TYPE_SCHEDULE_COMMAND,
    STATE_TYPE_EXECUTE_COMMAND,
    STATE_TYPE_COMMAND_SCHEDULED_ACK,
    STATE_TYPE_ERRONEOUS, // erroneous state machine state
    STATE_TYPE_STALE // dead lock state after all operations, usually before shutdown
} StateType;

/**
 * The node type describes node's connectivity or in other words the position in the network.
 */
typedef enum NodeType {
    NODE_TYPE_INVALID = 0, // invalid or uninitialized note type
    NODE_TYPE_ORPHAN, // no connection
    NODE_TYPE_ORIGIN, // connected at south or south and east
    NODE_TYPE_INTER_HEAD, // connected at north and south and east
    NODE_TYPE_INTER_NODE, // connected at north and south
    NODE_TYPE_TAIL, // connected at north
    NODE_TYPE_MASTER // for testing purposes when the node is attached to the NODE_TYPE_ORIGIN node
} NodeType;

/**
 * A pulse counter and it's connectivity state.
 */
typedef struct PulseCounter {
    uint8_t counter : 4; //pulse counter
    uint8_t isConnected : 1; // connectivity flag
    uint8_t __pad : 3;
} PulseCounter;

/**
 * Stores the amount of incoming pulses for each communication channel. The isConnected flags are set
 * if the number of incoming pulses exceeds a specific threshold.
 */
typedef struct DiscoveryPulseCounters {
    PulseCounter north;
    PulseCounter east;
    PulseCounter south;
    uint8_t loopCount; // discovery loop counter
} DiscoveryPulseCounters;

/**
 * The node address in the network. It is spread from the origin node which assigns itself
 * the first address (row=1, column=1).
 */
typedef struct NodeAddress {
    uint8_t row;
    uint8_t column;
} NodeAddress;


/**
 * Describes the node state type and address.
 */
typedef struct Node {
    StateType state;
    NodeType type;
    NodeAddress address;
} Node;


/**
 * Counters/resources needed for platform's periphery.
 */
typedef struct Periphery {
    uint8_t loopCount; // particle loop counter
} Periphery;

/**
 * The global particle state with references to the most important states, buffers, counters,
 * etc.
 */
typedef struct ParticleState {
    Node node;
    DiscoveryPulseCounters discoveryPulseCounters;
    Ports ports;
    Periphery periphery;
//    Interpreter interpreter;
#ifdef SIMULATION
    uint8_t magicEndByte;
#endif
} ParticleState;


