/**
 * @author Raoul Rubien  13.07.16.
 */

#pragma once

#include <stdint.h>

/**
 * Describes communication states of the initiator.
 */
typedef enum CommunicationInitiatorStateTypes {
    COMMUNICATION_INITIATOR_STATE_TYPE_IDLE,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED,
    COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK,
    COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED
} CommunicationInitiatorStateTypes;

/**
 * Describes communication states of the receptionist.
 */
typedef enum CommunicationReceptionistStateTypes {
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED,
    COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE
} CommunicationReceptionistStateTypes;

/**
 * describes the communication port state
 */
typedef struct CommunicationProtocolPortState {
    CommunicationInitiatorStateTypes initiatorState;
    CommunicationReceptionistStateTypes receptionistState;
    /**
     * value of 0 indicates timeout
     */
    uint8_t stateTimeoutCounter;
} CommunicationProtocolPortState;

typedef struct CommunicationProtocolPorts {
    CommunicationProtocolPortState north;
    CommunicationProtocolPortState east;
    CommunicationProtocolPortState south;
} CommunicationProtocolPorts;

/**
 * describes the network geometry; valid row/col values are > 0
 */
typedef struct NetworkGeometry {
    uint8_t rows;
    uint8_t columns;
} NetworkGeometry;

/**
 * describes the communication ports
 */
typedef struct CommunicationProtocol {
    CommunicationProtocolPorts ports;
    NetworkGeometry networkGeometry;
    uint8_t hasNetworkGeometryDiscoveryBreadCrumb : 1;
    uint8_t isBroadcastEnabled : 1;
    uint8_t isSimultaneousTransmissionEnabled : 1;
    uint8_t __pad : 5;
} CommunicationProtocol;