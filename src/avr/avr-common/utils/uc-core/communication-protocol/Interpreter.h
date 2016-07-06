//
// Created by Raoul Rubien on 25.05.16.
//

#pragma once

#include "CommunicationProtocolTypes.h"
#include "Commands.h"


extern FUNC_ATTRS void __interpretWaitForBeingEnumeratedReception(volatile RxPort *rxPort,
                                                                  volatile CommunicationProtocolPortState *o);
/**
 * Interprets reception in "wait for being enumerate states".
 */
FUNC_ATTRS void __interpretWaitForBeingEnumeratedReception(volatile RxPort *rxPort,
                                                           volatile CommunicationProtocolPortState *commPortState) {

    volatile Package *package = (Package *) rxPort->buffer.bytes;
    // interpret according to local reception protocol state
    switch (commPortState->receptionistState) {
        // on received address information
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE:
            // on address package
            if (package->asHeader.headerId == PACKAGE_HEADER_ID_TYPE_ENUMERATE &&
                equalsPackageSize(rxPort->buffer.pointer, EnumerationPackageBufferPointerSize)) {
                executeSetLocalAddress(&package->asEnumerationPackage);
                // send ack with local address back
                DEBUG_CHAR_OUT('a');
                commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK;
            }
            clearReceptionBuffer(rxPort);
            break;

            // on received ack
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE:
            if (equalsPackageSize(rxPort->buffer.pointer, AckPackagePointerSize) &&
                package->asACKPackage.headerId == PACKAGE_HEADER_ID_TYPE_ACK) {
                DEBUG_CHAR_OUT('d');
                commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE;
                ParticleAttributes.node.state = STATE_TYPE_LOCALLY_ENUMERATED;
            }
            clearReceptionBuffer(rxPort);
            break;

        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE:
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED:
            clearReceptionBuffer(rxPort);
            break;
    }
}

extern FUNC_ATTRS void __interpretReceivedPackage(volatile RxPort *rxPort
        //, volatile CommunicationProtocolPortState *commPortState
);

/**
 * Interpret received packages in idle state.
 */
FUNC_ATTRS void __interpretReceivedPackage(volatile RxPort *rxPort
        //, volatile CommunicationProtocolPortState *commPortState
) {
    Package *package = (Package *) rxPort->buffer.bytes;

    switch (package->asHeader.headerId) {

        case PACKAGE_HEADER_ID_TYPE_SYNC_TIME:
            executeSetLocalTime(&package->asSyncTimePackage);
            clearReceptionBuffer(rxPort);
            break;

        default:
            clearReceptionBuffer(rxPort);
            break;
    }
}


extern FUNC_ATTRS void __interpretEnumerateNeighbourAckReception(volatile RxPort *rxPort,
                                                              volatile CommunicationProtocolPortState *commPortState,
                                                              uint8_t expectedRemoteAddressRow,
                                                              uint8_t expectedRemoteAddressColumn);
/**
 * Interprets reception in neighbour enumeration states.
 */
FUNC_ATTRS void __interpretEnumerateNeighbourAckReception(volatile RxPort *rxPort,
                                                          volatile CommunicationProtocolPortState *commPortState,
                                                          uint8_t expectedRemoteAddressRow,
                                                          uint8_t expectedRemoteAddressColumn) {
    DEBUG_INT16_OUT(expectedRemoteAddressRow);
    DEBUG_INT16_OUT(ParticleAttributes.node.address.row);
    DEBUG_INT16_OUT(expectedRemoteAddressColumn);
    DEBUG_INT16_OUT(ParticleAttributes.node.address.column);
    volatile Package *package = (Package *) rxPort->buffer.bytes;
    switch (commPortState->initiatorState) {
        // on ack wih remote address
        case COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE:
            // on ack with data
            if (package->asHeader.headerId == PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA &&
                equalsPackageSize(rxPort->buffer.pointer, AckWithAddressPackageBufferPointerSize)) {
                // on correct address
                if (expectedRemoteAddressRow == package->asACKWithRemoteAddress.addressRow0 &&
                    expectedRemoteAddressColumn == package->asACKWithRemoteAddress.addressColumn0) {
                    DEBUG_CHAR_OUT('D');
                    commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK;
                } else { // on wrong address, restart transmission
                    DEBUG_CHAR_OUT('V');
                    DEBUG_CHAR_OUT('T');
                    commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT;
                }
            }

            clearReceptionBuffer(rxPort);
            break;
        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
//            ParticleAttributes.node.state = endState;
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED:
            break;
    }

}

extern FUNC_ATTRS void interpretRxBuffer(volatile RxPort *rxPort,
                                         volatile CommunicationProtocolPortState *commPortState);
/**
 * interprets reception according to the particle state
 */
FUNC_ATTRS void interpretRxBuffer(volatile RxPort *rxPort,
                                  volatile CommunicationProtocolPortState *commPortState) {

    DEBUG_CHAR_OUT('I');
    if (!rxPort->isDataBuffered) {
        DEBUG_CHAR_OUT('i');
        return;
    }

    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            __interpretWaitForBeingEnumeratedReception(rxPort, commPortState);
            break;


        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
            __interpretEnumerateNeighbourAckReception(rxPort, commPortState,
                                                      ParticleAttributes.node.address.row,
                                                   ParticleAttributes.node.address.column + 1);
            break;


        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
            __interpretEnumerateNeighbourAckReception(rxPort, commPortState,
                                                   ParticleAttributes.node.address.row + 1,
                                                      ParticleAttributes.node.address.column);
            break;

        case STATE_TYPE_IDLE:
            __interpretReceivedPackage(rxPort); //, commPortState);
            break;

        default:
            break;
    }
    DEBUG_CHAR_OUT('i');
}


