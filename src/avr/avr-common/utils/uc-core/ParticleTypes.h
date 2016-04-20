/*
 * @author Raoul Rubien 2015
 */

#ifndef __PARTICLE_TYPES_H
#define __PARTICLE_TYPES_H

#include "../common/PortInteraction.h"
#include "./communication/CommunicationTypes.h"
#include "./ParticleParameters.h"

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

/**
 * Possible particle's state machine states are listed in this enum.
 */
typedef enum {
    STATE_TYPE_UNDEFINED = 0, // uninitialized state
    STATE_TYPE_START, // particle is initialized
    STATE_TYPE_ACTIVE, // start neighbour discovery
    STATE_TYPE_NEIGHBOURS_DISCOVERY, // evaluates comm. port pulse counters
    STATE_TYPE_NEIGHBOURS_DISCOVERED, // discovery ended
    STATE_TYPE_DISCOVERY_PULSING, // keep pulsing post discovery
    STATE_TYPE_RESET, // state after reset command is received
    STATE_TYPE_WAIT_FOR_BEING_ENUMERATED, // waiting for network address assignment
    STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR, // assigning network address to south neighbour
    STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR, // assigning network address to east neighbour
    STATE_TYPE_ENUMERATED, // local address assigned and neighbour/s enumerated
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
typedef enum {
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
typedef struct {
    uint8_t counter : 4; //pulse counter
    uint8_t isConnected : 1; // connectivity flag
    uint8_t __pad :3;
} PulseCounter; // 1 byte total

/**
 * Stores the amount of incoming pulses for each communication channel. The isConnected flags are set
 * if the number of incoming pulses exceeds a specific threshold.
 */
typedef struct {
    PulseCounter north;
    PulseCounter east;
    PulseCounter south;
    uint8_t loopCount; // particle loop counter
} DiscoveryPulseCounters; // 3 + 1 = 4 byte total

/**
 * The node address in the network. It is spread from the origin node which assigns itself
 * the first address (row=1, column=1).
 */
typedef struct {
    uint8_t row;
    uint8_t column;
} NodeAddress; // 2 byte total

/**
 * Describes the node state type and address.
 */
typedef struct {
    StateType state;
    NodeType type;
    NodeAddress address;
} Node; // 2 + 2 + 2 = 6 byte total

/**
 * Counters/resources needed for platform's periphery.
 */
typedef struct {
    uint8_t loopCount; // particle loop counter
} Periphery;

/**
 * The global particle state with references to the most important states, buffers, counters,
 * etc.
 */
typedef struct {
    Node node;
    DiscoveryPulseCounters discoveryPulseCounters;
    Ports ports;
    Periphery periphery;
#ifdef SIMULATION
    uint8_t magicEndByte;
#endif
} ParticleState; // 6 + 4 + 51 = 61 bytes total


FUNC_ATTRS void constructPulseCounter(volatile PulseCounter *o) {
    o->counter = 0;
    o->isConnected = false;
}

FUNC_ATTRS void constructDiscoveryPulseCounters(volatile DiscoveryPulseCounters *o) {
    constructPulseCounter(&(o->north));
    constructPulseCounter(&(o->east));
    constructPulseCounter(&(o->south));
    o->loopCount = 0;
}

FUNC_ATTRS void constructNodeAddress(volatile NodeAddress *o) {
    o->row = 0;
    o->column = 0;
}

FUNC_ATTRS void constructNode(volatile Node *o) {
    o->state = STATE_TYPE_UNDEFINED;
    o->type = NODE_TYPE_INVALID;
    constructNodeAddress(&(o->address));
}

FUNC_ATTRS void constructPeriphery(volatile Periphery *o) {
    o->loopCount = 0;
}

FUNC_ATTRS void constructParticleState(volatile ParticleState *o) {
    constructNode(&(o->node));
    constructDiscoveryPulseCounters(&(o->discoveryPulseCounters));
    constructPorts(&(o->ports));
    constructPeriphery(&(o->periphery));
#ifdef SIMULATION
    o->magicEndByte = 0xaa;
#endif
}


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif