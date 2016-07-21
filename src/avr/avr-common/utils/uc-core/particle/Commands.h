/**
 * @author Raoul Rubien  13.07.16.
 */

#pragma once

#include "Globals.h"
#include "uc-core/communication-protocol/Commands.h"

/**
 * Transmits a new network geometry to the network. Particles outside the new boundary
 * switch to sleep mode.
 *
 * @pre:
 * the ParticleAttributes.protocol.networkGeometry.rows/cols are set accordingly
 */
extern FUNC_ATTRS void setNewNetworkGeometry(void);

FUNC_ATTRS void setNewNetworkGeometry(void) {
    TxPort temporaryPackagePort;
    constructSetNetworkGeometryPackage(&temporaryPackagePort,
                                       ParticleAttributes.protocol.networkGeometry.rows,
                                       ParticleAttributes.protocol.networkGeometry.columns);
    // interpret the constructed package
    executeSetNetworkGeometryPackage((SetNetworkGeometryPackage *) &temporaryPackagePort.buffer.bytes);
}

//extern FUNC_ATTRS void scheduleActuationCommand(uint16_t startPeriodTimeStamp, uint16_t endPeriodTimeStamp);
///**
// * Schedules an actuation command starting and ending at the specified time stamps.
// */
//FUNC_ATTRS void scheduleActuationCommand(uint16_t startPeriodTimeStamp, uint16_t endPeriodTimeStamp) {
//    ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = startPeriodTimeStamp;
//    ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp = endPeriodTimeStamp;
//    ParticleAttributes.actuationCommand.isScheduled = true;
//}

/**
 * Handles neighbour enumeration communication states.
 */
extern FUNC_ATTRS void handleEnumerateNeighbour(
        volatile DirectionOrientedPort *port,
        uint8_t remoteAddressRow,
        uint8_t remoteAddressColumn,
        StateType endState);

FUNC_ATTRS void handleEnumerateNeighbour(volatile DirectionOrientedPort *port,
                                         uint8_t remoteAddressRow,
                                         uint8_t remoteAddressColumn,
                                         StateType endState) {
    volatile CommunicationProtocolPortState *commPortState = port->protocol;
    if (commPortState->stateTimeoutCounter == 0 &&
        commPortState->initiatorState != COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT &&
        commPortState->initiatorState != COMMUNICATION_INITIATOR_STATE_TYPE_IDLE) {
        // on timeout fall back to start state
        commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT;
        commPortState->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
        DEBUG_CHAR_OUT('b');
    }

    volatile TxPort *txPort = port->txPort;
    switch (commPortState->initiatorState) {
        // transmit new address
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            if (port->discoveryPulseCounter->isConnected) {
                clearTransmissionPortBuffer(txPort);
                constructEnumeratePackage(txPort, remoteAddressRow, remoteAddressColumn);
                enableTransmission(txPort);
                DEBUG_CHAR_OUT('F');
                commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED;
            } else {
                ParticleAttributes.node.state = endState;
            }
            break;

            // wait for tx finished
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED:
            if (txPort->isTransmitting) {
                break;
            }
            DEBUG_CHAR_OUT('R');
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE;
            break;

            // wait fo ack with address
        case COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE:
            port->receivePimpl();
            break;

            // send ack back
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
            clearTransmissionPortBuffer(txPort);
            constructEnumerationACKPackage(txPort);
            enableTransmission(txPort);
            DEBUG_CHAR_OUT('f');
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED;
            break;

            // switch state
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED:
            if (txPort->isTransmitting) {
                break;
            }
            DEBUG_CHAR_OUT('D');
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            goto __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            break;

        __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
            ParticleAttributes.node.state = endState;
            break;
    }
}

/**
 * Constructs a heat wires command and puts the particle into sending mode.
 * Destination addresses must be route-able from this node on. That means
 * i) the destination must be in same column but row is greater than current node's row or
 * ii) the destination resides in a different column but the current row equals 1.
 * Otherwise the request is skipped.
 * @param nodeAddress the node address
 * @param wires affected actuator flags
 * @param timeStamp the time stamp when the actuation should start
 * @param duration 10bit actuation duration {@link #HeatWiresPackage}
 */
extern FUNC_ATTRS void sendHeatWires(NodeAddress *nodeAddress, Actuators *wires, uint16_t timeStamp,
                                     uint16_t duration);

FUNC_ATTRS void sendHeatWires(NodeAddress *nodeAddress, Actuators *wires, uint16_t timeStamp,
                              uint16_t duration) {
    if (ParticleAttributes.node.address.row > nodeAddress->row &&
        ParticleAttributes.node.address.column > nodeAddress->column) {
        // illegal address
        return;
    }

    TxPort temporaryPackagePort;
    constructHeatWiresPackage(&temporaryPackagePort, nodeAddress,
                              wires, timeStamp, duration);
    // interpret the constructed package
    executeHeatWiresPackage((HeatWiresPackage *) temporaryPackagePort.buffer.bytes);
}


/**
 * Constructs a heat wires range command and puts the particle into sending mode. The
 * rectangular range span is defined by the first address (left top) and second address (bottom right).
 * It must span at least two nodes. The top left address must be route-able from this node on. That means
 * i) the top left address must be in same column but row is greater than current node's row or
 * ii) the top left address resides in a different column but the current row equals 1.
 * Otherwise the request is skipped.
 */
extern FUNC_ATTRS void sendHeatWiresRange(NodeAddress *nodeAddressTopLeft,
                                          NodeAddress *nodeAddressBottomRight,
                                          Actuators *wires, uint16_t timeStamp,
                                          uint16_t duration);

FUNC_ATTRS void sendHeatWiresRange(NodeAddress *nodeAddressTopLeft, NodeAddress *nodeAddressBottomRight,
                                   Actuators *wires, uint16_t timeStamp,
                                   uint16_t duration) {

    if (nodeAddressBottomRight->row < nodeAddressTopLeft->row ||
        nodeAddressBottomRight->column < nodeAddressTopLeft->column) {
        // illegal range defined
        return;
    }
    if (((nodeAddressBottomRight->row - nodeAddressTopLeft->row) +
         (nodeAddressBottomRight->column - nodeAddressTopLeft->column)) < 1) {
        // illegal range defined
        return;
    }

    if (ParticleAttributes.node.address.row != 1 &&
        (nodeAddressTopLeft->column != ParticleAttributes.node.address.column ||
         nodeAddressBottomRight->column != ParticleAttributes.node.address.column)) {
        // not route-able top left address
        return;
    }

    if (nodeAddressTopLeft->row < ParticleAttributes.node.address.row ||
        nodeAddressTopLeft->column < ParticleAttributes.node.address.column) {
        // not route-able top left address
        return;
    }

    // @ pre: range spans at least 2 nodes
    // @ pre: top left is route-able from this node

    TxPort temporaryPackagePort;
    constructHeatWiresRangePackage(&temporaryPackagePort, nodeAddressTopLeft,
                                   nodeAddressBottomRight, wires, timeStamp, duration);
    // interpret the constructed package
    executeHeatWiresRangePackage(
            (HeatWiresRangePackage *) &temporaryPackagePort.buffer.bytes);
}


/**
 * Sends a header package to neighbours.
 */
extern FUNC_ATTRS void sendHeaderPackage(HeaderPackage *package);

FUNC_ATTRS void sendHeaderPackage(HeaderPackage *package) {
    // interpret the package
    executeHeaderPackage(package);
}