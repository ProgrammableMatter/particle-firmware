/**
 * @author Raoul Rubien  13.07.2016
 *
 * Communication related types definitions.
 */

#pragma once

#include <stdint.h>

/**
 * Describes communication states of the initiator. The initiator is the particle which
 * started the communication.
 */
typedef enum {
    COMMUNICATION_INITIATOR_STATE_TYPE_IDLE,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED,
    COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED
} CommunicationInitiatorStateTypes;

/**
 * Describes communication states of the receptionist. The receptionist is the particle
 * which reacts on a received package.
 */
typedef enum {
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE
} CommunicationReceptionistStateTypes;

/**
 * Describes the communication port state.
 */
typedef struct {
    CommunicationInitiatorStateTypes initiatorState;
    CommunicationReceptionistStateTypes receptionistState;
    /**
     * value 0 indicates a timeout
     */
    uint8_t stateTimeoutCounter;
    /**
    * retransmissions: value 0 indicates all retransmissions consumed
    */
    uint8_t reTransmissions : 4;
    uint8_t __pad : 4;
} CommunicationProtocolPortState;

/**
 * Communication protocol ports bundle.
 */
typedef struct {
    CommunicationProtocolPortState north;
    CommunicationProtocolPortState east;
    CommunicationProtocolPortState south;
} CommunicationProtocolPorts;

/**
 * Describes the network geometry; valid row/col values are (> 0) && (<= UINT8_MAX).
 */
typedef struct {
    uint8_t rows;
    uint8_t columns;
} NetworkGeometry;

/**
 * The communication protocol structure.
 */
typedef struct {
    CommunicationProtocolPorts ports;
    NetworkGeometry networkGeometry;
    uint8_t hasNetworkGeometryDiscoveryBreadCrumb : 1;
    volatile uint8_t isBroadcastEnabled : 1;
    volatile uint8_t isSimultaneousTransmissionEnabled : 1;
    uint8_t __pad : 5;
} CommunicationProtocol;
