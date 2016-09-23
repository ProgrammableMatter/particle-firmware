/**
 * @author Raoul Rubien 2016
 *
 * Particle core implementation.
 */

#pragma once

#include <avr/sleep.h>
#include "common/common.h"
#include "Globals.h"
#include "ParticleStateTypes.h"
#include "ParticleStateTypesCtors.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/delay/delay.h"
#include "uc-core/discovery/Discovery.h"
#include "uc-core/configuration/Particle.h"
#include "uc-core/configuration/Periphery.h"
#include "uc-core/configuration/interrupts/ReceptionPCI.h"
#include "uc-core/configuration/interrupts/TxRxTimer.h"
#include "uc-core/configuration/interrupts/DiscoveryPCI.h"
#include "uc-core/configuration/interrupts/DiscoveryTimer.h"
#include "uc-core/configuration/interrupts/ActuationTimer.h"
#include "uc-core/interrupts/Interrupts.h"
#include "uc-core/communication-protocol/CommunicationProtocol.h"
#include "uc-core/communication-protocol/CommunicationProtocolTypesCtors.h"
#include "uc-core/communication-protocol/CommunicationProtocolPackageTypesCtors.h"
#include "uc-core/actuation/Actuation.h"
#include "Commands.h"
#include "uc-core/periphery/Periphery.h"

/**
 * Disables discovery sensing interrupts.
 */
//extern FUNC_ATTRS void __disableDiscoverySensing(void);

static FUNC_ATTRS void __disableDiscoverySensing(void) {
    DISCOVERY_INTERRUPTS_DISABLE;
}

/**
 * Enables discovery sensing interrupts.
 */
//extern FUNC_ATTRS void __enableDiscoverySensing(void);

static FUNC_ATTRS void __enableDiscoverySensing(void) {
//    DISCOVERY_INTERRUPTS_SETUP;
    DISCOVERY_INTERRUPTS_ENABLE;
}

/**
 * Disables discovery pulsing.
 */
//extern FUNC_ATTRS void __disableDiscoveryPulsing(void);

static FUNC_ATTRS void __disableDiscoveryPulsing(void) {
    TIMER_NEIGHBOUR_SENSE_PAUSE;
    TIMER_NEIGHBOUR_SENSE_DISABLE;
    NORTH_TX_LO;
    EAST_TX_HI;
    SOUTH_TX_LO;
}

/**
 * Enables discovery sensing.
 */
//extern FUNC_ATTRS void __enableDiscoveryPulsing(void);

static FUNC_ATTRS void __enableDiscoveryPulsing(void) {
    TIMER_NEIGHBOUR_SENSE_ENABLE;
    TIMER_NEIGHBOUR_SENSE_RESUME;
}

/**
 * Sets up ports and interrupts but does not enable the global interrupt (I-flag in SREG).
 */
//extern FUNC_ATTRS void __initParticle(void);

static FUNC_ATTRS void __initParticle(void) {
    LED_STATUS1_OFF;
    LED_STATUS2_OFF;
    LED_STATUS3_OFF;
    LED_STATUS4_OFF;
    LED_STATUS5_OFF;
    LED_STATUS6_OFF;

    TEST_POINT1_LO;
    TEST_POINT2_LO;

    DELAY_MS_1;
    LED_STATUS1_ON;
    DELAY_MS_1;
    LED_STATUS1_OFF;

//    RX_INTERRUPTS_SETUP; // configure input pins interrupts
//    RX_INTERRUPTS_ENABLE; // enable input pin interrupts
    DISCOVERY_INTERRUPTS_SETUP; // configure pin change interrupt
    TIMER_NEIGHBOUR_SENSE_SETUP; // configure timer interrupt for neighbour sensing
    LOCAL_TIME_INTERRUPT_COMPARE_DISABLE; // prepare local time timer interrupt
    ACTUATOR_INTERRUPT_SETUP; // prepare actuation interrupt
    sleep_disable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // set sleep mode to power down

    // TODO: evaluation source
    __TIMER1_OVERFLOW_INTERRUPT_ENABLE;
}

/**
 * Disables all reception interrupts.
 */
//extern FUNC_ATTRS void __disableReceptionPinChangeInterrupts(void);

static FUNC_ATTRS void __disableReceptionPinChangeInterrupts(void) {
    RX_NORTH_INTERRUPT_DISABLE;
    RX_EAST_INTERRUPT_DISABLE;
    RX_SOUTH_INTERRUPT_DISABLE;
}

/**
 * Clears pending and enables reception interrupts for north port if connected.
 */
//extern FUNC_ATTRS void __enableReceptionHardwareNorth(void);

static FUNC_ATTRS void __enableReceptionHardwareNorth(void) {
    if (ParticleAttributes.discoveryPulseCounters.north.isConnected) {
        RX_NORTH_INTERRUPT_CLEAR_PENDING;
        RX_NORTH_INTERRUPT_ENABLE;
    }
}

/**
 * Clears pending and enables reception interrupts for east port if connected.
 */
//extern FUNC_ATTRS void __enableReceptionHardwareEast(void);

static FUNC_ATTRS void __enableReceptionHardwareEast(void) {
    if (ParticleAttributes.discoveryPulseCounters.east.isConnected) {
        RX_EAST_INTERRUPT_CLEAR_PENDING;
        RX_EAST_INTERRUPT_ENABLE;
    }
}

/**
 * Clears pending and enables reception interrupts for south port if connected.
 */
//extern FUNC_ATTRS void __enableReceptionHardwareSouth(void);

static FUNC_ATTRS void __enableReceptionHardwareSouth(void) {
    if (ParticleAttributes.discoveryPulseCounters.south.isConnected) {
        RX_SOUTH_INTERRUPT_CLEAR_PENDING;
        RX_SOUTH_INTERRUPT_ENABLE;
    }
}

/**
 * Sets up and enables the tx/rx timer.
 */
//extern FUNC_ATTRS void __enableTxRxTimer(void);

static FUNC_ATTRS void __enableTxRxTimer(void) {
    TIMER_TX_RX_COUNTER_SETUP;
    TIMER_TX_RX_COUNTER_ENABLE;
//    TIMER_TX_RX_ENABLE_COMPARE_INTERRUPT;
}

/**
 * Disables the signal generating transmission interrupt.
 */
//extern FUNC_ATTRS void __disableTransmission(void);

//static FUNC_ATTRS void __disableTransmission(void) {
//    TIMER_TX_RX_DISABLE_COMPARE_INTERRUPT;
//}

/**
 * Sets up and enables reception for all connected ports.
 */
//extern FUNC_ATTRS void __enableReceptionHardwareForConnectedPorts(void);

static FUNC_ATTRS void __enableReceptionHardwareForConnectedPorts(void) {
//    RX_INTERRUPTS_SETUP;
//    RX_INTERRUPTS_ENABLE;
    __enableReceptionHardwareNorth();
    __enableReceptionHardwareEast();
    __enableReceptionHardwareSouth();
}

/**
 * Disables reception interrupts.
 */
//static extern FUNC_ATTRS void __disableReception(void);

FUNC_ATTRS void __disableReception(void) {
    __disableReceptionPinChangeInterrupts();
}

/**
 * Enables reception timer and interrupts.
 */
//static extern FUNC_ATTRS void __enableReception(void);

FUNC_ATTRS void __enableReception(void) {
    __enableTxRxTimer();
    __enableReceptionHardwareForConnectedPorts();
}

///**
// * Toggles the heartbeat LED.
// */
//extern FUNC_ATTRS void __heartBeatToggle(void);
//
//FUNC_ATTRS void __heartBeatToggle(void) {
//    ParticleAttributes.periphery.loopCount++;
//    if (ParticleAttributes.periphery.loopCount > HEARTBEAT_LOOP_COUNT_TOGGLE) {
//        ParticleAttributes.periphery.loopCount = 0;
//        LED_STATUS6_TOGGLE;
//    }
//}

/**
 * Increments the discovery loop counter.
 */
// extern FUNC_ATTRS void __discoveryLoopCount(void);

static FUNC_ATTRS void __discoveryLoopCount(void) {
    if (ParticleAttributes.discoveryPulseCounters.loopCount < (UINT8_MAX)) {
        ParticleAttributes.discoveryPulseCounters.loopCount++;
    }
}

/**
 * Sets the correct address if this node is the origin node.
 */
//extern FUNC_ATTRS  void __updateOriginNodeAddress(void);

static FUNC_ATTRS void __updateOriginNodeAddress(void) {
    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.node.address.row = 1;
        ParticleAttributes.node.address.column = 1;
    }
}

/**
 * Handles (state driven) the wait for being enumerated node state.
 */
//extern FUNC_ATTRS void __handleWaitForBeingEnumerated(void);

static FUNC_ATTRS void __handleWaitForBeingEnumerated(void) {
    volatile CommunicationProtocolPortState *commPortState = &ParticleAttributes.protocol.ports.north;
    if (commPortState->stateTimeoutCounter == 0 &&
        commPortState->receptionistState != COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK &&
        commPortState->receptionistState !=
        COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED) {
        // on timeout: fall back to start state
        commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE;
        commPortState->stateTimeoutCounter = COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX;
        if (commPortState->reTransmissions > 0) {
            commPortState->reTransmissions--;
        }
        DEBUG_CHAR_OUT('b');
    }

    if (commPortState->reTransmissions == 0) {
        // on retransmissions consumed: sort cut the global state machine to erroneous state
        ParticleAttributes.node.state = STATE_TYPE_ERRONEOUS;
        return;
    }

    switch (commPortState->receptionistState) {
        // receive
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE:
            ParticleAttributes.directionOrientedPorts.north.receivePimpl();
            break;
            // transmit ack with local address
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK:
            constructEnumerationACKWithAddressToParentPackage();
            enableTransmission(&ParticleAttributes.communication.ports.tx.north);
            DEBUG_CHAR_OUT('f');
            commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED;
            break;
            // wait for tx finished
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED:
            if (ParticleAttributes.communication.ports.tx.north.isTransmitting) {
                break;
            }
            clearReceptionPortBuffer(&ParticleAttributes.communication.ports.rx.north);
            DEBUG_CHAR_OUT('R');
            commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE;
            break;
            // wait for response
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE:
            ParticleAttributes.directionOrientedPorts.north.receivePimpl();
            break;

        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE:
            break;
    }
}

/**
 * Handles (state driven) neighbour synchronization node states.
 * @param endState the state when handler has finished
 */
//extern FUNC_ATTRS void __handleSynchronizeNeighbour(StateType endState);

static FUNC_ATTRS void __handleSynchronizeNeighbour(StateType endState) {
    volatile CommunicationProtocolPortState *commPortState = ParticleAttributes.directionOrientedPorts.simultaneous.protocol;
    volatile TxPort *txPort = ParticleAttributes.directionOrientedPorts.simultaneous.txPort;
    switch (commPortState->initiatorState) {
        // transmit local time simultaneously on east and south ports
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            constructSyncTimePackage(txPort);
            enableTransmission(txPort);
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED;
            break;

            // wait for tx finished
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED:
            if (txPort->isTransmitting) {
                break;
            }
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            goto __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            break;

        case COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED:
        __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
            ParticleAttributes.node.state = endState;
            break;
    }
}

/**
 * Handles the sync neighbour done state.
 * In general it simply switches to the specified state,
 * but if simulation/test macros are defined it redirects to other states accordingly.
 * @param endState the state after this call if no test macros defined
 */
//extern FUNC_ATTRS void __handleSynchronizeNeighbourDoneOrRunTest(StateType endState);

static FUNC_ATTRS void __handleSynchronizeNeighbourDoneOrRunTest(StateType endState) {
#if defined(SIMULATION_SET_NEW_NETWORK_GEOMETRY_TEST)
    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.protocol.networkGeometry.rows = 2;
        ParticleAttributes.protocol.networkGeometry.columns = 1;
        DELAY_MS_1;
        setNewNetworkGeometry();
        return;
    }
#elif defined(SIMULATION_HEAT_WIRES_TEST)
    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        Actuators actuators;
        actuators.northLeft = true;
        actuators.northRight = true;
        NodeAddress nodeAddress;
        nodeAddress.row = 2;
        nodeAddress.column = 2;
        DELAY_MS_1;
        sendHeatWires(&nodeAddress, &actuators, 5, 2);
        return;
    }
#elif defined(SIMULATION_HEAT_WIRES_RANGE_TEST)
    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        Actuators actuators;
        actuators.northLeft = false;
        actuators.northRight = true;
        NodeAddress fromAddress;
        fromAddress.row = 1;
        fromAddress.column = 2;
        NodeAddress toAddress;
        toAddress.row = 2;
        toAddress.column = 2;
        DELAY_MS_1;
        sendHeatWiresRange(&fromAddress, &toAddress, &actuators, 50000, 10);
        return;
    }
#elif defined(SIMULATION_HEAT_WIRES_MODE_TEST)
    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        DELAY_MS_1;
        sendHeatWiresModePackage(HEATING_LEVEL_TYPE_STRONG);
        return;
    }
#elif defined(SIMULATION_SEND_HEADER_TEST)
    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        HeaderPackage package;
        package.id = PACKAGE_HEADER_ID_HEADER;
        package.enableBroadcast = true;
        DELAY_MS_1;
        sendHeaderPackage(&package);
        return;
    }
#endif
    ParticleAttributes.node.state = endState;
}

/**
 * Handles (state driven) relaying network geometry communication states.
 * @param endState the state when handler finished
 */
//extern FUNC_ATTRS void __handleRelayAnnounceNetworkGeometry(StateType endState);

static FUNC_ATTRS void __handleRelayAnnounceNetworkGeometry(StateType endState) {
    volatile CommunicationProtocolPortState *commPortState = &ParticleAttributes.protocol.ports.north;
    switch (commPortState->initiatorState) {
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            enableTransmission(&ParticleAttributes.communication.ports.tx.north);
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED;
            break;

            // wait for tx finished
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED:
            if (ParticleAttributes.communication.ports.tx.north.isTransmitting) {
                break;
            }
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            goto __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            break;

        case COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED:
        __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
            ParticleAttributes.node.state = endState;
            break;
    }
}


/**
 * Handles (event driven) network geometry announcement states.
 * @param endState the state when handler finished
 */
// extern FUNC_ATTRS void __handleSendAnnounceNetworkGeometry(StateType endState);

static FUNC_ATTRS void __handleSendAnnounceNetworkGeometry(StateType endState) {
    volatile TxPort *txPort = &ParticleAttributes.communication.ports.tx.north;
    volatile CommunicationProtocolPortState *commPortState = &ParticleAttributes.protocol.ports.north;

    switch (ParticleAttributes.protocol.ports.north.initiatorState) {
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            constructAnnounceNetworkGeometryPackage(ParticleAttributes.node.address.row,
                                                    ParticleAttributes.node.address.column);
            enableTransmission(txPort);
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED;
            break;

            // wait for tx finished
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED:
            if (txPort->isTransmitting) {
                break;
            }
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            goto __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            break;

        case COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED:
        __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
            ParticleAttributes.node.state = endState;
            break;
    }
}

/**
 * Advance communication timeout counters.
 */
//extern FUNC_ATTRS void __advanceCommunicationProtocolCounters(void);

static FUNC_ATTRS void __advanceCommunicationProtocolCounters(void) {
    if (!ParticleAttributes.communication.ports.tx.north.isTransmitting &&
        ParticleAttributes.protocol.ports.north.stateTimeoutCounter > 0) {
        ParticleAttributes.protocol.ports.north.stateTimeoutCounter--;
    }
    if (!ParticleAttributes.communication.ports.tx.east.isTransmitting &&
        ParticleAttributes.protocol.ports.east.stateTimeoutCounter > 0) {
        ParticleAttributes.protocol.ports.east.stateTimeoutCounter--;
    }
    if (!ParticleAttributes.communication.ports.tx.south.isTransmitting &&
        ParticleAttributes.protocol.ports.south.stateTimeoutCounter > 0) {
        ParticleAttributes.protocol.ports.south.stateTimeoutCounter--;
    }
}

/**
 * Handles discovery states, classifies the local node type and switches to next state.
 */
// extern FUNC_ATTRS void __handleNeighboursDiscovery(void);

static FUNC_ATTRS void __handleNeighboursDiscovery(void) {
    __discoveryLoopCount();


//    if (ParticleAttributes.discoveryPulseCounters.loopCount == 0) {
//    LED_STATUS1_TOGGLE;
//    }

    if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOURS_DISCOVERY_LOOPS) {
//        LED_STATUS1_ON;
        // on discovery timeout
        __disableDiscoverySensing();
        ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERED;
    } else if (ParticleAttributes.discoveryPulseCounters.loopCount >=
               // on min. discovery loops exceeded
               MIN_NEIGHBOURS_DISCOVERY_LOOPS) {
        if (updateAndDetermineNodeType()) {
            // on distinct discovery
            __disableDiscoverySensing();
            ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERED;
        }
        else {
            PARTICLE_DISCOVERY_LOOP_DELAY;
        }
        __updateOriginNodeAddress();
    }
}

/**
 * Handles the post discovery extended pulsing period with subsequent switch to next state.
 */
//extern FUNC_ATTRS void __handleDiscoveryPulsing(void);

static FUNC_ATTRS void __handleDiscoveryPulsing(void) {
    if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOUR_PULSING_LOOPS) {
        __disableDiscoveryPulsing();
        ParticleAttributes.node.state = STATE_TYPE_DISCOVERY_PULSING_DONE;
    } else {
        __discoveryLoopCount();
    }
}

/**
 * Handle the discovery to enumeration state switch.
 */
//extern FUNC_ATTRS void __handleDiscoveryPulsingDone(void);

static FUNC_ATTRS void __handleDiscoveryPulsingDone(void) {

    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS;
        PARTICLE_DISCOVERY_PULSE_DONE_POST_DELAY;
    } else {
        setReceptionistStateStart(&ParticleAttributes.protocol.ports.north);
        ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
        DEBUG_CHAR_OUT('W');
    }
    __enableReception();
    clearReceptionBuffers();
}

/**
 * Sends the set network geometry package and switches to the given state.
 * @param rows the new network geometry rows
 * @param cols the new network geometry rows
 * @param endState the state when handler finished
 */
//extern FUNC_ATTRS void __handleSetNetworkGeometry(uint8_t rows, uint8_t cols, StateType endState);

static FUNC_ATTRS void __handleSetNetworkGeometry(uint8_t rows, uint8_t cols, StateType endState) {
    volatile CommunicationProtocolPortState *commPortState = ParticleAttributes.directionOrientedPorts.simultaneous.protocol;
    volatile TxPort *txPort = ParticleAttributes.directionOrientedPorts.simultaneous.txPort;
    switch (commPortState->initiatorState) {
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            constructSetNetworkGeometryPackage(txPort, rows, cols);
            enableTransmission(txPort);
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED;
            break;

            // wait for tx finished
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED:
            if (txPort->isTransmitting) {
                break;
            }
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            goto __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            break;

        case COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED:
        __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
            ParticleAttributes.node.state = endState;
            break;
    }
}

/**
 * State driven handling of sending package states without ack requests.
 * @param port the designated port
 * @param endState the end state when handler finished
 */
//extern FUNC_ATTRS void __handleSendPackage(volatile DirectionOrientedPort *port, StateType endState);

static FUNC_ATTRS void __handleSendPackage(volatile DirectionOrientedPort *port, StateType endState) {
    volatile CommunicationProtocolPortState *commPortState = port->protocol;
    switch (commPortState->initiatorState) {
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            enableTransmission(port->txPort);
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED;
            break;

            // wait for tx finished
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_WAIT_FOR_TX_FINISHED:
            if (port->txPort->isTransmitting) {
                break;
            }
            commPortState->initiatorState = COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            goto __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE;
            break;

        case COMMUNICATION_INITIATOR_STATE_TYPE_WAIT_FOR_RESPONSE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK_WAIT_FOR_TX_FINISHED:
        __COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
            ParticleAttributes.node.state = endState;
            break;
    }
}

/**
 * Callback when actuation command has finished.
 */
//extern FUNC_ATTRS void __onActuationDoneCallback(void);

static FUNC_ATTRS void __onActuationDoneCallback(void) {
    ParticleAttributes.node.state = STATE_TYPE_IDLE;
}

/**
 * Checks whether an actuation is to be executed. Switches state if an actuation
 * command is scheduled and the current local time indicates an actuation start.
 */
//extern FUNC_ATTRS void __handleIsActuationCommandPeriod(void);

static FUNC_ATTRS void __handleIsActuationCommandPeriod(void) {
    if (ParticleAttributes.actuationCommand.isScheduled &&
        ParticleAttributes.actuationCommand.executionState == ACTUATION_STATE_TYPE_IDLE) {
        if (ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp <=
            ParticleAttributes.localTime.numTimePeriodsPassed) {
            ParticleAttributes.node.state = STATE_TYPE_EXECUTE_ACTUATION_COMMAND;
        }
    }
}

/**
 * The core function is called cyclically in the particle loop. It implements the
 * behaviour of the particle.
 */
//extern inline void process(void);

static inline void process(void) {
    // DEBUG_CHAR_OUT('P');
    // __heartBeatToggle();
    // ---------------- init states ----------------

    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_RESET:
            __disableReception();
            constructParticle(&ParticleAttributes);
            ParticleAttributes.node.state = STATE_TYPE_START;
            goto __STATE_TYPE_START;
            break;

        __STATE_TYPE_START:
        case STATE_TYPE_START:
            __initParticle();
            ParticleAttributes.node.state = STATE_TYPE_ACTIVE;
            goto __STATE_TYPE_ACTIVE;
            break;

        __STATE_TYPE_ACTIVE:
        case STATE_TYPE_ACTIVE:
            ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERY;
            __enableDiscoverySensing();
            __enableDiscoveryPulsing();
            SEI;
            goto __STATE_TYPE_NEIGHBOURS_DISCOVERY;
            break;

            // ---------------- boot states: discovery ----------------
        __STATE_TYPE_NEIGHBOURS_DISCOVERY:
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            __handleNeighboursDiscovery();
            break;

        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
//            __discoveryLoopCount();
            ParticleAttributes.node.state = STATE_TYPE_DISCOVERY_PULSING;
            goto __STATE_TYPE_DISCOVERY_PULSING;
            break;

        __STATE_TYPE_DISCOVERY_PULSING:
        case STATE_TYPE_DISCOVERY_PULSING:
//            PARTICLE_DISCOVERY_LOOP_DELAY;
            __handleDiscoveryPulsing();
            break;

        case STATE_TYPE_DISCOVERY_PULSING_DONE:
            __handleDiscoveryPulsingDone();
            break;

            // ---------------- boot states: local enumeration ----------------

            // wait for incoming address from north neighbour
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            __handleWaitForBeingEnumerated();
            break;

        case STATE_TYPE_LOCALLY_ENUMERATED:
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS;
            goto __STATE_TYPE_ENUMERATING_NEIGHBOURS;
            break;

            // ---------------- boot states: neighbour enumeration ----------------

        __STATE_TYPE_ENUMERATING_NEIGHBOURS:
        case STATE_TYPE_ENUMERATING_NEIGHBOURS:
            setInitiatorStateStart(&ParticleAttributes.protocol.ports.east);
            DEBUG_CHAR_OUT('E');
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR;
            goto __STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR;
            break;

            // ---------------- boot states: east neighbour enumeration ----------------

        __STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
            handleEnumerateNeighbour(&ParticleAttributes.directionOrientedPorts.east,
                                     ParticleAttributes.node.address.row,
                                     ParticleAttributes.node.address.column + 1,
                                     STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE);
            __advanceCommunicationProtocolCounters();
            break;

        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE:
            setInitiatorStateStart(&ParticleAttributes.protocol.ports.south);
            DEBUG_CHAR_OUT('e');
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR;
            goto __STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR;
            break;

            // ---------------- boot states: south neighbour enumeration ----------------

        __STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
            handleEnumerateNeighbour(&ParticleAttributes.directionOrientedPorts.south,
                                     ParticleAttributes.node.address.row + 1,
                                     ParticleAttributes.node.address.column,
                                     STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE);
            __advanceCommunicationProtocolCounters();
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE:
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE;
            goto __STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE;
            break;

        __STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE:
        case STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE:
            setInitiatorStateStart(&ParticleAttributes.protocol.ports.north);
            ParticleAttributes.node.state = STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY;
            enableLocalTimeInterrupt();
            goto __STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY;
            break;

            // ---------------- boot states: network geometry detection/announcement ----------------

        __STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY:
        case STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY:
            // on right most and bottom most node
            if ((ParticleAttributes.node.type == NODE_TYPE_TAIL) &&
                (true == ParticleAttributes.protocol.hasNetworkGeometryDiscoveryBreadCrumb)) {
                __handleSendAnnounceNetworkGeometry(STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_DONE);
            } else {
                ParticleAttributes.node.state = STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_DONE;
                goto __STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_DONE;
            }
            break;

        __STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_DONE:
        case STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_DONE:
            ParticleAttributes.node.state = STATE_TYPE_IDLE;
            goto __STATE_TYPE_IDLE;
            break;

        case STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY:
            __handleRelayAnnounceNetworkGeometry(STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY_DONE);
            break;

        case STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY_DONE:
            ParticleAttributes.node.state = STATE_TYPE_IDLE;
            goto __STATE_TYPE_IDLE;
            break;

            // ---------------- working states: sync neighbour ----------------

        case STATE_TYPE_SYNC_NEIGHBOUR:
            __handleSynchronizeNeighbour(STATE_TYPE_SYNC_NEIGHBOUR_DONE);
            break;

        case STATE_TYPE_SYNC_NEIGHBOUR_DONE:
            __handleSynchronizeNeighbourDoneOrRunTest(STATE_TYPE_IDLE);
            break;

            // ---------------- working states: set network geometry----------------
            // todo unused state
        case STATE_TYPE_SEND_SET_NETWORK_GEOMETRY:
            __handleSetNetworkGeometry(ParticleAttributes.protocol.networkGeometry.rows,
                                       ParticleAttributes.protocol.networkGeometry.columns,
                                       STATE_TYPE_IDLE);
            break;

            // ---------------- working states: execute actuation command----------------

        case STATE_TYPE_EXECUTE_ACTUATION_COMMAND:
            handleExecuteActuation(__onActuationDoneCallback);
            break;

            // ---------------- working states: sending package ----------------

        case STATE_TYPE_SENDING_PACKAGE_TO_NORTH:
            __handleSendPackage(&ParticleAttributes.directionOrientedPorts.north,
                                STATE_TYPE_IDLE);
            break;

        case STATE_TYPE_SENDING_PACKAGE_TO_EAST:
            __handleSendPackage(&ParticleAttributes.directionOrientedPorts.east,
                                STATE_TYPE_IDLE);
            break;

        case STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH:
            __handleSendPackage(&ParticleAttributes.directionOrientedPorts.simultaneous,
                                STATE_TYPE_IDLE);
            break;

        case STATE_TYPE_SENDING_PACKAGE_TO_SOUTH:
            __handleSendPackage(&ParticleAttributes.directionOrientedPorts.south,
                                STATE_TYPE_IDLE);
            break;

        case STATE_TYPE_SENDING_PACKAGE_TO_NORTH_THEN_PREPARE_SLEEP:
            __handleSendPackage(&ParticleAttributes.directionOrientedPorts.north,
                                STATE_TYPE_PREPARE_FOR_SLEEP);
            break;

        case STATE_TYPE_SENDING_PACKAGE_TO_EAST_THEN_PREPARE_SLEEP:
            __handleSendPackage(&ParticleAttributes.directionOrientedPorts.east,
                                STATE_TYPE_PREPARE_FOR_SLEEP);
            break;

        case STATE_TYPE_SENDING_PACKAGE_TO_EAST_AND_SOUTH_THEN_PREPARE_SLEEP:
            __handleSendPackage(&ParticleAttributes.directionOrientedPorts.simultaneous,
                                STATE_TYPE_PREPARE_FOR_SLEEP);
            break;

        case STATE_TYPE_SENDING_PACKAGE_TO_SOUTH_THEN_PREPARE_SLEEP:
            __handleSendPackage(&ParticleAttributes.directionOrientedPorts.south,
                                STATE_TYPE_PREPARE_FOR_SLEEP);
            break;


            // ---------------- working states: receiving/interpreting commands ----------------

        __STATE_TYPE_IDLE:
        case STATE_TYPE_IDLE:
            ParticleAttributes.directionOrientedPorts.north.receivePimpl();
            ParticleAttributes.directionOrientedPorts.east.receivePimpl();
            ParticleAttributes.directionOrientedPorts.south.receivePimpl();
            __handleIsActuationCommandPeriod();

            if (ParticleAttributes.localTime.numTimePeriodsPassed > 10) {
                blinkAddressNonblocking();
                blinkTimeIntervalNonblocking();
            }


            break;

            // ---------------- standby states: sleep mode related states ----------------

        case STATE_TYPE_PREPARE_FOR_SLEEP:
            if (ParticleAttributes.directionOrientedPorts.north.txPort->isTransmitting ||
                ParticleAttributes.directionOrientedPorts.east.txPort->isTransmitting ||
                ParticleAttributes.directionOrientedPorts.south.txPort->isTransmitting) {
                break;
            }
            ParticleAttributes.node.state = STATE_TYPE_SLEEP_MODE;
            break;

        case STATE_TYPE_SLEEP_MODE:
            DEBUG_CHAR_OUT('z');
            sleep_enable();
            CLI;
            sleep_cpu();
            sleep_disable();
            DEBUG_CHAR_OUT('Z');
            break;

            // ---------------- erroneous/dead end states ----------------
        case STATE_TYPE_UNDEFINED:
        case STATE_TYPE_ERRONEOUS:
        case STATE_TYPE_STALE:
//        __STATE_TYPE_STALE:
            blinkGenericErrorForever();
            break;
    }
}
