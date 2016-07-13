/**
 * @author Raoul Rubien 2015
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
#include "uc-core/configuration/interrupts/Reception.h"
#include "uc-core/configuration/interrupts/TimerCounter.h"
#include "uc-core/interrupts/Interrupts.h"
#include "uc-core/configuration/Particle.h"
#include "uc-core/configuration/Periphery.h"
#include "uc-core/communication-protocol/CommunicationProtocol.h"
#include "uc-core/communication-protocol/CommunicationProtocolTypesCtors.h"
#include "uc-core/communication-protocol/CommunicationProtocolPackageCtors.h"
//#include "uc-core/communication-protocol/Interpreter.h"
#include "uc-core/actuation/Actuation.h"
#include "Commands.h"

extern FUNC_ATTRS void __disableDiscoverySensing(void);

FUNC_ATTRS void __disableDiscoverySensing(void) {
    RX_INTERRUPTS_DISABLE;
}

extern FUNC_ATTRS void __disableDiscoveryPulsing(void);

FUNC_ATTRS void __disableDiscoveryPulsing(void) {
    TIMER_NEIGHBOUR_SENSE_PAUSE;
    TIMER_NEIGHBOUR_SENSE_DISABLE;
    NORTH_TX_LO;
    EAST_TX_LO;
    SOUTH_TX_LO;
}

extern FUNC_ATTRS void __enableDiscovery(void);

FUNC_ATTRS void __enableDiscovery(void) {
    TIMER_NEIGHBOUR_SENSE_ENABLE;
    TIMER_NEIGHBOUR_SENSE_RESUME;
}

extern FUNC_ATTRS void __initParticle(void);
/**
 * Sets up ports and interrupts but does not enable the global interrupt (I-flag in SREG)
 */
FUNC_ATTRS void __initParticle(void) {
    RX_INTERRUPTS_SETUP; // configure input pins interrupts
    RX_INTERRUPTS_ENABLE; // enable input pin interrupts
    TIMER_NEIGHBOUR_SENSE_SETUP; // configure timer interrupt for neighbour sensing
    LOCAL_TIME_INTERRUPT_COMPARE_DISABLE;
    sleep_disable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // set sleep mode to power down
}

extern FUNC_ATTRS void __disableReceptionPinChangeInterrupts(void);

FUNC_ATTRS void __disableReceptionPinChangeInterrupts(void) {
    RX_NORTH_INTERRUPT_DISABLE;
    RX_EAST_INTERRUPT_DISABLE;
    RX_SOUTH_INTERRUPT_DISABLE;
}

extern FUNC_ATTRS void __enableReceptionHardwareNorth(void);

FUNC_ATTRS void __enableReceptionHardwareNorth(void) {
    if (ParticleAttributes.discoveryPulseCounters.north.isConnected) {
        RX_NORTH_INTERRUPT_CLEAR_PENDING;
        RX_NORTH_INTERRUPT_ENABLE;
    }
}

extern FUNC_ATTRS void __enableReceptionHardwareEast(void);

FUNC_ATTRS void __enableReceptionHardwareEast(void) {
    if (ParticleAttributes.discoveryPulseCounters.east.isConnected) {
        RX_EAST_INTERRUPT_CLEAR_PENDING;
        RX_EAST_INTERRUPT_ENABLE;
    }
}

extern FUNC_ATTRS void __enableReceptionHardwareSouth(void);

FUNC_ATTRS void __enableReceptionHardwareSouth(void) {
    if (ParticleAttributes.discoveryPulseCounters.south.isConnected) {
        RX_SOUTH_INTERRUPT_CLEAR_PENDING;
        RX_SOUTH_INTERRUPT_ENABLE;
    }
}

extern FUNC_ATTRS void __enableTxRxTimer(void);

FUNC_ATTRS void __enableTxRxTimer(void) {
    TIMER_TX_RX_COUNTER_SETUP;
    TIMER_TX_RX_COUNTER_ENABLE;
//    TIMER_TX_RX_ENABLE_COMPARE_INTERRUPT;
}

extern FUNC_ATTRS void __disableTransmission(void);

FUNC_ATTRS void __disableTransmission(void) {
    TIMER_TX_RX_DISABLE_COMPARE_INTERRUPT;
}

extern FUNC_ATTRS void __enableReceptionHardwareForConnectedPorts(void);

FUNC_ATTRS void __enableReceptionHardwareForConnectedPorts(void) {
    RX_INTERRUPTS_SETUP;
    RX_INTERRUPTS_ENABLE;
    __enableReceptionHardwareNorth();
    __enableReceptionHardwareEast();
    __enableReceptionHardwareSouth();

}

extern FUNC_ATTRS void __disableReception(void);

FUNC_ATTRS void __disableReception(void) {
    ParticleAttributes.communication.xmissionState = STATE_TYPE_XMISSION_TYPE_DISABLED_TX_RX;
    __disableReceptionPinChangeInterrupts();
}

extern FUNC_ATTRS void __enableReception(void);

FUNC_ATTRS void __enableReception(void) {
    __enableTxRxTimer();
    __enableReceptionHardwareForConnectedPorts();
    ParticleAttributes.communication.xmissionState = STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX;
}

extern FUNC_ATTRS void __heartBeatToggle(void);
/**
 * toggle heartbeat LED
 */
FUNC_ATTRS void __heartBeatToggle(void) {
    ParticleAttributes.periphery.loopCount++;
    if (ParticleAttributes.periphery.loopCount > HEARTBEAT_LOOP_COUNT_TOGGLE) {
        LED_HEARTBEAT_TOGGLE;
        ParticleAttributes.periphery.loopCount = 0;
    }
}

extern FUNC_ATTRS void __discoveryLoopCount(void);

FUNC_ATTRS void __discoveryLoopCount(void) {
    if (ParticleAttributes.discoveryPulseCounters.loopCount < (UINT8_MAX)) {
        ParticleAttributes.discoveryPulseCounters.loopCount++;
    }
}

extern FUNC_ATTRS  void __updateOriginNodeAddress(void);

FUNC_ATTRS void __updateOriginNodeAddress(void) {
    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.node.address.row = 1;
        ParticleAttributes.node.address.column = 1;
    }
}

extern FUNC_ATTRS void __handleWaitForBeingEnumerated(void);
/**
 * Handles the "ait for being enumerated" state.
 */
FUNC_ATTRS void __handleWaitForBeingEnumerated(void) {
    volatile CommunicationProtocolPortState *commPortState = &ParticleAttributes.protocol.ports.north;
    switch (commPortState->receptionistState) {
        // receive
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE:
            ParticleAttributes.directionOrientedPorts.north.receive();
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
            ParticleAttributes.directionOrientedPorts.north.receive();
            break;

        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE:
            break;
    }
}

extern FUNC_ATTRS void __handleSynchronizeNeighbour(volatile CommunicationProtocolPortState *commPortState,
                                                    StateType endState);
/**
 * Handles neighbour synchronization communication states.
 */
FUNC_ATTRS void __handleSynchronizeNeighbour(volatile CommunicationProtocolPortState *commPortState,
                                             StateType endState) {
    volatile TxPort *txPort = ParticleAttributes.communication.ports.tx.simultaneous;
    switch (commPortState->initiatorState) {
        // transmit local time simultaneously on east and south ports
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            clearTransmissionPortBuffer(txPort);
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

extern FUNC_ATTRS void __handleRelayAnnounceNetworkGeometry(StateType endState);

/**
 * Handles relaying network geometry communication states.
 */
FUNC_ATTRS void __handleRelayAnnounceNetworkGeometry(StateType endState) {
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


extern FUNC_ATTRS void __handleSendAnnounceNetworkGeometry(StateType endState);
/**
 * Handles network geometry announcement states.
 */
FUNC_ATTRS void __handleSendAnnounceNetworkGeometry(StateType endState) {
    volatile TxPort *txPort = &ParticleAttributes.communication.ports.tx.north;
    volatile CommunicationProtocolPortState *commPortState = &ParticleAttributes.protocol.ports.north;

    switch (ParticleAttributes.protocol.ports.north.initiatorState) {
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            clearTransmissionPortBuffer(txPort);
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

extern FUNC_ATTRS void __advanceCommunicationProtocolCounters(void);

/**
 * Advance communication timeout counters.
 */
extern FUNC_ATTRS void __advanceCommunicationProtocolCounters(void) {
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

extern FUNC_ATTRS void __handleNeighboursDiscovery(void);
/**
 * Handles discovery states, classifies the local node type and switches to next state.
 */
FUNC_ATTRS void __handleNeighboursDiscovery(void) {
    __discoveryLoopCount();
    if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOURS_DISCOVERY_LOOPS) {
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
        } else {
            PARTICLE_DISCOVERY_LOOP_DELAY;
        }
        __updateOriginNodeAddress();
    }
}


extern FUNC_ATTRS void __handleDiscoveryPulsing(void);

/**
 * Handles the post discovery extended pulsing period with subsequent switch to next state.
 */
FUNC_ATTRS void __handleDiscoveryPulsing(void) {
    if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOUR_PULSING_LOOPS) {
        __disableDiscoveryPulsing();
        ParticleAttributes.node.state = STATE_TYPE_DISCOVERY_PULSING_DONE;
    } else {
        __discoveryLoopCount();
    }
}

extern FUNC_ATTRS void __handleDiscoveryPulsingDone(void);
/**
 * Handle the discovery to enumeration state switch.
 */
FUNC_ATTRS void __handleDiscoveryPulsingDone(void) {
    PARTICLE_DISCOVERY_PULSE_DONE_POST_DELAY;

    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS;
    } else {
        setReceptionistStateStart(&ParticleAttributes.protocol.ports.north);
        ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
        DEBUG_CHAR_OUT('W');
    }
    __enableReception();
    clearReceptionBuffers();
}

extern FUNC_ATTRS void __handleSetNetworkGeometry(uint8_t rows, uint8_t cols, StateType endState);
/**
 * set the set network geometry package and switches to the given state
 */
FUNC_ATTRS void __handleSetNetworkGeometry(uint8_t rows, uint8_t cols, StateType endState) {
    // TODO refactoring needed
    volatile CommunicationProtocolPortState *commPortState = &ParticleAttributes.protocol.ports.east;
    volatile TxPort *txPort = ParticleAttributes.communication.ports.tx.simultaneous;

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

extern FUNC_ATTRS void __onActuationDoneCallback(void);
/**
 * callback when actuation command has finished
 */
FUNC_ATTRS void __onActuationDoneCallback(void) {
    ParticleAttributes.node.state = STATE_TYPE_IDLE;
}

extern FUNC_ATTRS void __enableLocalTimeInterrupt(void);
/**
 * enables the local time interrupt using current adjustment argument
 */
FUNC_ATTRS void __enableLocalTimeInterrupt(void) {
    LOCAL_TIME_INTERRUPT_COMPARE_VALUE = ParticleAttributes.localTime.interruptDelay;
    LOCAL_TIME_INTERRUPT_COMPARE_ENABLE;
}

extern inline void particleTick(void);

/**
 * This function is called cyclically in the particle loop. It implements the
 * behaviour and state machine of the particle.
 */
inline void particleTick(void) {
//    DEBUG_CHAR_OUT('P');
    __heartBeatToggle();

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
            __enableDiscovery();
            SEI;
            goto __STATE_TYPE_NEIGHBOURS_DISCOVERY;
            break;

            // ---------------- discovery states ----------------
        __STATE_TYPE_NEIGHBOURS_DISCOVERY:
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            __handleNeighboursDiscovery();
            break;

        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
            __discoveryLoopCount();
            ParticleAttributes.node.state = STATE_TYPE_DISCOVERY_PULSING;
            goto __STATE_TYPE_DISCOVERY_PULSING;
            break;

        __STATE_TYPE_DISCOVERY_PULSING:
        case STATE_TYPE_DISCOVERY_PULSING:
            __handleDiscoveryPulsing();
            break;

        case STATE_TYPE_DISCOVERY_PULSING_DONE:
            __handleDiscoveryPulsingDone();
            break;

            // ---------------- local enumeration states ----------------

            // wait for incoming address from north neighbour
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            __handleWaitForBeingEnumerated();
            break;

        case STATE_TYPE_LOCALLY_ENUMERATED:
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS;
            goto __STATE_TYPE_ENUMERATING_NEIGHBOURS;
            break;

            // ---------------- neighbour enumeration states ----------------

        __STATE_TYPE_ENUMERATING_NEIGHBOURS:
        case STATE_TYPE_ENUMERATING_NEIGHBOURS:
            setInitiatorStateStart(&ParticleAttributes.protocol.ports.east);
            DEBUG_CHAR_OUT('E');
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR;
            goto __STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR;
            break;

            // ---------------- east neighbour enumeration states ----------------

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

            // ---------------- south neighbour enumeration states ----------------

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
            __enableLocalTimeInterrupt();
            goto __STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY;
            break;

            // ---------------- network geometry detection/announcement states ----------------

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

            // ---------------- working states ----------------

        case STATE_TYPE_SYNC_NEIGHBOUR:
            __handleSynchronizeNeighbour(
                    &ParticleAttributes.protocol.ports.east,
                    STATE_TYPE_SYNC_NEIGHBOUR_DONE);
            break;

        case STATE_TYPE_SYNC_NEIGHBOUR_DONE:
//            if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
//                ParticleAttributes.protocol.networkGeometry.rows = 2;
//                ParticleAttributes.protocol.networkGeometry.columns = 2;
//                setNewNetworkGeometry();
//                DELAY_MS_1;
//            } else {
            ParticleAttributes.node.state = STATE_TYPE_IDLE;
            goto __STATE_TYPE_IDLE;

//            __TIMER1_OVERFLOW_INTERRUPT_ENABLE;
//            DEBUG_INT16_OUT(TIMER_TX_RX_COUNTER_VALUE);
            break;

        case STATE_TYPE_SEND_SET_NETWORK_GEOMETRY:
            __handleSetNetworkGeometry(ParticleAttributes.protocol.networkGeometry.rows,
                                       ParticleAttributes.protocol.networkGeometry.columns,
                                       STATE_TYPE_IDLE);
            break;

        case STATE_TYPE_EXECUTE_ACTUATION_COMMAND:
            handleExecuteActuation(__onActuationDoneCallback);
            break;

        __STATE_TYPE_IDLE:
        case STATE_TYPE_IDLE:
            ParticleAttributes.directionOrientedPorts.north.receive();
            ParticleAttributes.directionOrientedPorts.east.receive();
            ParticleAttributes.directionOrientedPorts.south.receive();
            break;

        case STATE_TYPE_SLEEP_MODE:
            DEBUG_CHAR_OUT('z');
            sleep_enable();
            CLI;
            sleep_cpu();
            sleep_disable();
            DEBUG_CHAR_OUT('Z');
            break;

        case STATE_TYPE_UNDEFINED:
        case STATE_TYPE_ERRONEOUS:
        case STATE_TYPE_STALE:
            forever {
                LED_STATUS0_ON;
                DELAY_MS_196;
                LED_STATUS0_OFF;
                DELAY_MS_196;
            }
            break;
    }
}
