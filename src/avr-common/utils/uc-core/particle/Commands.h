/**
 * @author Raoul Rubien  13.07.2016
 *
 * Particle's higher level network commands ought to be used as network API.
 */

#pragma once

#include "Globals.h"
#include "uc-core/communication-protocol/Commands.h"

/**
 * Transmits a new network geometry to the network. Particles outside the new boundary
 * switch to sleep mode.
 *
 * @pre the ParticleAttributes.protocol.networkGeometry.rows/cols are set accordingly
 */
extern FUNC_ATTRS void setNewNetworkGeometry(void);

FUNC_ATTRS void setNewNetworkGeometry(void) {
    TxPort temporaryPackagePort;
    constructSetNetworkGeometryPackage(&temporaryPackagePort,
                                       ParticleAttributes.protocol.networkGeometry.rows,
                                       ParticleAttributes.protocol.networkGeometry.columns);
    // interpret the constructed package
    executeSetNetworkGeometryPackage((SetNetworkGeometryPackage *) temporaryPackagePort.buffer.bytes);
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
 * State driven neighbour enumeration handler.
 * @param port the port at which to handle enumeration
 * @param remoteAddressRow the neighbour's address row to assign
 * @param remoteAddressColumn the neighbour's address column to assign
 * @param endState state when transaction has finished
 */
extern FUNC_ATTRS void handleEnumerateNeighbour(
        DirectionOrientedPort *port,
        uint8_t remoteAddressRow,
        uint8_t remoteAddressColumn,
        StateType endState);

FUNC_ATTRS void handleEnumerateNeighbour(DirectionOrientedPort *const port,
                                         const uint8_t remoteAddressRow,
                                         const uint8_t remoteAddressColumn,
                                         const StateType endState) {
    // TODO: move function to ParticleCore.h
    CommunicationProtocolPortState *commPortState = port->protocol;


    if (commPortState->stateTimeoutCounter == 0 &&
        commPortState->initiatorState != COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT &&
        commPortState->initiatorState != COMMUNICATION_INITIATOR_STATE_TYPE_IDLE) {
        // on timeout: fall back to start state
        DEBUG_CHAR_OUT('T');
        commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT;
        commPortState->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
        if (commPortState->reTransmissions > 0) {
            commPortState->reTransmissions--;
        }
        DEBUG_CHAR_OUT('b');
    }

    if (commPortState->reTransmissions == 0) {
        // on retransmissions consumed: sort cut the state machine to end state
        commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
    }

    TxPort *txPort = port->txPort;
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
 * Sends a heat wires mode package to neighbours.
 * The package is propagated through the entire network.
 * @param heatingPowerLevel the new power level to set up
 */
extern FUNC_ATTRS void sendHeatWiresModePackage(const HeatingLevelType heatingPowerLevel);

FUNC_ATTRS void sendHeatWiresModePackage(HeatingLevelType heatingPowerLevel) {
    TxPort temporaryPackagePort;
    constructHeatWiresModePackage(&temporaryPackagePort, heatingPowerLevel);
    // interpret the constructed package
    executeHeatWiresModePackage((HeatWiresModePackage *) temporaryPackagePort.buffer.bytes);
}

/**
 * Constructs a heat wires command and puts the particle into sending mode.
 * Destination addresses must be route-able from this node on. That means
 * i) the destination must be in same column but row is greater than current node's row or
 * ii) the destination resides in a different column but the current row equals 1.
 * Otherwise the request is skipped.
 * For more details about duration see {@link constructHeatWiresRangePackage()} constructor.
 * @param nodeAddress the node address
 * @param wires affected actuator flags
 * @param timeStamp the time stamp when the actuation should start
 * @param duration 10bit actuation duration {@link #HeatWiresPackage}
 */
extern FUNC_ATTRS void sendHeatWires(const NodeAddress *const nodeAddress, const Actuators *const wires,
                                     const uint16_t timeStamp,
                                     const uint16_t duration);

FUNC_ATTRS void sendHeatWires(const NodeAddress *const nodeAddress, const Actuators *const wires,
                              const uint16_t timeStamp,
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
 * For more details about duration see {@link constructHeatWiresRangePackage()} constructor.
 * @param nodeAddressTopLeft top left node address
 * @param nodeAddressBottomRight bottom right node address
 * @param wires affected actuator flags
 * @param timeStamp the time stamp when the actuation should start
 * @param duration 10bit actuation duration {@link #HeatWiresPackage}
 */
extern FUNC_ATTRS void sendHeatWiresRange(const NodeAddress *const nodeAddressTopLeft,
                                          const NodeAddress *const nodeAddressBottomRight,
                                          const Actuators *const wires, const uint16_t timeStamp,
                                          const uint16_t duration);

FUNC_ATTRS void sendHeatWiresRange(const NodeAddress *const nodeAddressTopLeft,
                                   const NodeAddress *const nodeAddressBottomRight,
                                   const Actuators *const wires, const uint16_t timeStamp,
                                   const uint16_t duration) {

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
            (HeatWiresRangePackage *) temporaryPackagePort.buffer.bytes);
}

/**
 * Sends a header package to adjacent neighbours.
 * @param package the package to send
 */
extern FUNC_ATTRS void sendHeaderPackage(HeaderPackage *const package);

FUNC_ATTRS void sendHeaderPackage(HeaderPackage *const package) {
    package->startBit = 1;
    // interpret the package
    executeHeaderPackage(package);
}


extern FUNC_ATTRS void sendSyncPackage(void);

FUNC_ATTRS void sendSyncPackage(void) {
    ParticleAttributes.node.state = STATE_TYPE_SYNC_NEIGHBOUR;
    setInitiatorStateStart(ParticleAttributes.directionOrientedPorts.simultaneous.protocol);
}