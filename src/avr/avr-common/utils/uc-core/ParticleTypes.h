/**
 * @author Raoul Rubien 2015
 */

#ifndef PARTICLE_TYPES_H
#define PARTICLE_TYPES_H

#include <common/PortInteraction.h>

typedef enum {
    STATE_TYPE_START,
    STATE_TYPE_ACTIVE,
    STATE_TYPE_NEIGHBOURS_DISCOVERY,
    STATE_TYPE_NEIGHBOURS_DISCOVERED,
    STATE_TYPE_DISCOVERY_PULSING,
    STATE_TYPE_WAITING,
    STATE_TYPE_RESET,
    STATE_TYPE_WAIT_FOR_BEING_ENUMERATED,
    STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR,
    STATE_TYPE_ENUMERATED,
    STATE_TYPE_IDLE,
    STATE_TYPE_INTERPRETE_COMMAND,
    STATE_TYPE_RX_A,
    STATE_TYPE_RX_B,
    STATE_TYPE_FORWARD_PKG,
    STATE_TYPE_SCHEDULE_COMMAND,
    STATE_TYPE_EXECUTE_COMMAND,
    STATE_TYPE_COMMAND_SCHEDULED_ACK,
    STATE_TYPE_ERRONEOUS
} StateType;

typedef enum {
    NODE_TYPE_ORPHAN, NODE_TYPE_HEAD, NODE_TYPE_INTER_NODE, NODE_TYPE_TAIL
} NodeType;

typedef struct {
    unsigned char north : 1;
    unsigned char south : 1;
    unsigned char east : 1;
    unsigned char isInitialized : 1;
    unsigned char : 4;
} RxInterruptFlankStates;

typedef struct {
    unsigned char north : 4;
    unsigned char south : 4;
    unsigned char east : 4;
    unsigned char : 4;
    unsigned char loopCount;
} RxDiscoveryPulseCounter;

#define RX_PULSE_COUNTER_MAX 0xF

typedef struct {
    unsigned char northBytes[2];
    unsigned char southBytes[2];
    unsigned char eastBytes[2];
    unsigned char northBitCount : 4;
    unsigned char southBitCount : 4;
    unsigned char eastBitCount : 4;
    unsigned char : 4;
} RxBitBuffer;

typedef struct {
    unsigned char row;
    unsigned char column;
} NodeId;

typedef struct {
    StateType state;
    NodeType type;
    NodeId nodeId;
    RxBitBuffer rxBitBuffer;
    RxDiscoveryPulseCounter rxDiscoveryPulseCounters;
    RxInterruptFlankStates rxInterruptFlankStates;
} ParticleState;

#endif