/**
 * Created by Raoul Rubien on 25.05.2016
 *
 * Interpreter implementation.
 */

#pragma once

#include "CommunicationProtocolTypes.h"
#include "Commands.h"

/**
 * Interprets reception in wait for being enumerate states.
 * @param rxPort the port to interpret data from
 * @param commPortState port's communication state
 */
extern FUNC_ATTRS void __interpretWaitForBeingEnumeratedReception(volatile RxPort *rxPort,
                                                                  volatile CommunicationProtocolPortState *commPortState);

FUNC_ATTRS void __interpretWaitForBeingEnumeratedReception(volatile RxPort *rxPort,
                                                           volatile CommunicationProtocolPortState *commPortState) {
    // TODO enhancement: check parity bit == expected parity bit

    volatile Package *package = (Package *) rxPort->buffer.bytes;
    // interpret according to local reception protocol state
    switch (commPortState->receptionistState) {
        // on received address information
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE:
            // on address package
            if (equalsPackageSize(&rxPort->buffer.pointer, EnumerationPackageBufferPointerSize) &&
                package->asHeader.id == PACKAGE_HEADER_ID_TYPE_ENUMERATE) {
                executeSetLocalAddress(&package->asEnumerationPackage);
                // send ack with local address back
                // DEBUG_CHAR_OUT('a');
                commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK;
            }
            clearReceptionPortBuffer(rxPort);
            break;

            // on received ack
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE:
            if (equalsPackageSize(&rxPort->buffer.pointer, AckPackagePointerSize) &&
                package->asACKPackage.id == PACKAGE_HEADER_ID_TYPE_ACK) {
                // DEBUG_CHAR_OUT('d');
                ParticleAttributes.protocol.isBroadcastEnabled = package->asACKPackage.enableBroadcast;
                ParticleAttributes.node.state = STATE_TYPE_LOCALLY_ENUMERATED;
                commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE;
            }
            clearReceptionPortBuffer(rxPort);
            break;

        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE:
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED:
            clearReceptionPortBuffer(rxPort);
            break;
    }
}

/**
 * Interpret received packages in working state (idle state).
 * @param rxPort the port to interpret data from
 */
extern FUNC_ATTRS void __interpretReceivedPackage(volatile DirectionOrientedPort *port);

FUNC_ATTRS void __interpretReceivedPackage(volatile DirectionOrientedPort *port) {
    // TODO enhancement: check equalsPackageSize() == expected size
    // TODO enhancement: check parity bit == expected parity bit
    Package *package = (Package *) port->rxPort->buffer.bytes;

    switch (package->asHeader.id) {

        case PACKAGE_HEADER_ID_TYPE_SYNC_TIME:
            executeSynchronizeLocalTimePackage(&package->asSyncTimePackage, &port->rxPort->buffer);
            break;

        case PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE:
            executeAnnounceNetworkGeometryPackage(&package->asAnnounceNetworkGeometryPackage);
            break;

        case PACKAGE_HEADER_ID_TYPE_SET_NETWORK_GEOMETRY:
            executeSetNetworkGeometryPackage(&package->asSetNetworkGeometryPackage);
            break;

        case PACKAGE_HEADER_ID_TYPE_HEAT_WIRES:
            if (package->asHeader.isRangeCommand) {
                executeHeatWiresRangePackage(&package->asHeatWiresRangePackage);
            } else {
                executeHeatWiresPackage(&package->asHeatWiresPackage);
            }
            break;

        case PACKAGE_HEADER_ID_HEADER:
            executeHeaderPackage(&package->asHeader);
            break;

        case PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_MODE:
            executeHeatWiresModePackage(&package->asHeatWiresModePackage);
            break;

        default:
            DEBUG_CHAR_OUT('u');
            break;
    }
    clearReceptionPortBuffer(port->rxPort);
}

/**
 * Interprets reception buffer in neighbour enumeration states.
 * @param rxPort the port to interpret data from
 * @param commPortState port's communication state
 * @param expectedRemoteAddressRow the expected remote address row
 * @param expectedRemoteAddressColumn the expected remote address column
 */
extern FUNC_ATTRS void __interpretEnumerateNeighbourAckReception(volatile RxPort *rxPort,
                                                                 volatile CommunicationProtocolPortState *commPortState,
                                                                 uint8_t expectedRemoteAddressRow,
                                                                 uint8_t expectedRemoteAddressColumn);

FUNC_ATTRS void __interpretEnumerateNeighbourAckReception(volatile RxPort *rxPort,
                                                          volatile CommunicationProtocolPortState *commPortState,
                                                          uint8_t expectedRemoteAddressRow,
                                                          uint8_t expectedRemoteAddressColumn) {
    // TODO enhancement: check parity bit == expected parity bit
    // DEBUG_INT16_OUT(expectedRemoteAddressRow);
    // DEBUG_INT16_OUT(ParticleAttributes.node.address.row);
    // DEBUG_INT16_OUT(expectedRemoteAddressColumn);
    // DEBUG_INT16_OUT(ParticleAttributes.node.address.column);
    volatile Package *package = (Package *) rxPort->buffer.bytes;
    switch (commPortState->initiatorState) {
        // on ack wih remote address
        case COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE:
            // on ack with data
            if (equalsPackageSize(&rxPort->buffer.pointer, AckWithAddressPackageBufferPointerSize) &&
                package->asHeader.id == PACKAGE_HEADER_ID_TYPE_ACK_WITH_DATA) {
                // on correct address
                if (expectedRemoteAddressRow == package->asACKWithRemoteAddress.addressRow &&
                    expectedRemoteAddressColumn == package->asACKWithRemoteAddress.addressColumn) {
                    // DEBUG_CHAR_OUT('D');
                    commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK;
                } else { // on wrong address, restart transmission
                    // DEBUG_CHAR_OUT('V');
                    // DEBUG_CHAR_OUT('T');
                    commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT;
                }
            }

            clearReceptionPortBuffer(rxPort);
            break;
        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED:
            break;
    }
}

/**
 * Interprets reception buffer in respect to the current particle state.
 * @param rxPort the port to interpret data from
 */
extern FUNC_ATTRS void interpretRxBuffer(volatile DirectionOrientedPort *port);

FUNC_ATTRS void interpretRxBuffer(volatile DirectionOrientedPort *port) {
    // TODO enhancement: check equalsPackageSize() == expected size
    // TODO enhancement: check parity bit == expected parity bit
    DEBUG_CHAR_OUT('I');
    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            __interpretWaitForBeingEnumeratedReception(port->rxPort,
                                                       port->protocol);
            break;

        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
            __interpretEnumerateNeighbourAckReception(port->rxPort,
                                                      port->protocol,
                                                      ParticleAttributes.node.address.row,
                                                      ParticleAttributes.node.address.column + 1);
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
            __interpretEnumerateNeighbourAckReception(port->rxPort,
                                                      port->protocol,
                                                      ParticleAttributes.node.address.row + 1,
                                                      ParticleAttributes.node.address.column);
            break;

        case STATE_TYPE_IDLE:
            __interpretReceivedPackage(port);
            break;

        default:
            break;
    }
    // DEBUG_CHAR_OUT('i');
}
