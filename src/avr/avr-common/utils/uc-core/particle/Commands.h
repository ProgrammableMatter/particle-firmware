/**
 * @author Raoul Rubien  13.07.16.
 */

#pragma once

extern FUNC_ATTRS void setNewNetworkGeometry(void);
/**
 * Transmits a new network geometry to the network. Particles outside the new boundary
 * switch to sleep mode.
 *
 * @pre:
 * + the network must be in broadcast mode
 * + the ParticleAttributes.protocol.networkGeometry.rows/cols are set accordingly
 */
FUNC_ATTRS void setNewNetworkGeometry(void) {
    // TODO refactoring necessary
    clearTransmissionPortBuffer(ParticleAttributes.communication.ports.tx.simultaneous);
    setInitiatorStateStart(&ParticleAttributes.protocol.ports.east);
    ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
    ParticleAttributes.node.state = STATE_TYPE_SEND_SET_NETWORK_GEOMETRY;
}

extern FUNC_ATTRS void scheduleActiationCommand(uint16_t startPeriodTimeStamp, uint16_t endPeriodTimeStamp);
/**
 * Schedules an actuation command starting and ending at the specified time stamps.
 */
FUNC_ATTRS void scheduleActiationCommand(uint16_t startPeriodTimeStamp, uint16_t endPeriodTimeStamp) {
    ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = startPeriodTimeStamp;
    ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp = endPeriodTimeStamp;
    ParticleAttributes.actuationCommand.isScheduled = true;
}

extern FUNC_ATTRS void handleEnumerateNeighbour(
        volatile DirectionOrientedPort *port,
        uint8_t remoteAddressRow,
        uint8_t remoteAddressColumn,
        StateType endState);
/**
 * Handles neighbour enumeration communication states.
 */
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
            port->receive();
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