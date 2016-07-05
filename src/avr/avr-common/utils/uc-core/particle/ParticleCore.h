/**
 * @author Raoul Rubien 2015
 */

#pragma once

#include "common/common.h"
#include "Globals.h"
#include "ParticleStateTypes.h"
#include "ParticleStateTypesCtors.h"
#include "uc-core/configuration/IoPins.h"
#include "uc-core/delay/delay.h"
#include "uc-core/discovery/Discovery.h"
#include "uc-core/interrupts/Reception.h"
#include "uc-core/interrupts/TimerCounter.h"
#include "uc-core/interrupts/Interrupts.h"
#include "uc-core/configuration/Particle.h"
#include "uc-core/communication-protocol/CommunicationProtocol.h"
#include "uc-core/communication-protocol/CommunicationProtocolTypesCtors.h"
#include "uc-core/communication-protocol/Interpreter.h"


extern FUNC_ATTRS bool __updateAndDetermineNodeType(void);
/**
 * Updates the node type according to the amount of incoming pulses. The type {@link NodeType} is stored to
 * {@link ParticleAttributes.type}.
 * @return true if the node is fully connected, else false
 */
FUNC_ATTRS bool __updateAndDetermineNodeType(void) {
    if (ParticleAttributes.discoveryPulseCounters.north.isConnected) { // N
        if (ParticleAttributes.discoveryPulseCounters.south.isConnected) { // N, S
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) { // N, S, E
                ParticleAttributes.node.type = NODE_TYPE_INTER_HEAD;
                return true;
            } else { // N,S,!E
                ParticleAttributes.node.type = NODE_TYPE_INTER_NODE;
            }
        } else { // N, !S
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) { // N, !S, E
                ParticleAttributes.node.type = NODE_TYPE_INVALID;
            } else { // N, !S, !E
                ParticleAttributes.node.type = NODE_TYPE_TAIL;
            }
        }
    } else { // !N
        if (ParticleAttributes.discoveryPulseCounters.south.isConnected) { // !N, S
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) { // !N, S, E
                ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
            } else { // !N, S, !E
                ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
            }
        } else { // !N, !S
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) { // !N, !S, E
                ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
            } else { // !N, !S, !E
                ParticleAttributes.node.type = NODE_TYPE_ORPHAN;
            }
        }
    }
    return false;
}

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
 * Toggles heartbeat LED
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

extern FUNC_ATTRS void __receiveNorth(void);

FUNC_ATTRS void __receiveNorth(void) {
    manchesterDecodeBuffer(&ParticleAttributes.communication.ports.rx.north);
    interpretRxBuffer(&ParticleAttributes.communication.ports.rx.north,
                      &ParticleAttributes.protocol.ports.north);
}

extern FUNC_ATTRS void __receiveEast(void);

FUNC_ATTRS void __receiveEast(void) {
    manchesterDecodeBuffer(&ParticleAttributes.communication.ports.rx.east);
    interpretRxBuffer(&ParticleAttributes.communication.ports.rx.east,
                      &ParticleAttributes.protocol.ports.east);
}

extern FUNC_ATTRS void __receiveSouth(void);

FUNC_ATTRS void __receiveSouth(void) {
    manchesterDecodeBuffer(&ParticleAttributes.communication.ports.rx.south);
    interpretRxBuffer(&ParticleAttributes.communication.ports.rx.south,
                      &ParticleAttributes.protocol.ports.south);
}

extern FUNC_ATTRS void __handleWaitForBeingEnumerated(volatile CommunicationProtocolPortState *commPortState);
/**
 * Handles the "ait for being enumerated" state.
 */
FUNC_ATTRS void __handleWaitForBeingEnumerated(volatile CommunicationProtocolPortState *commPortState) {
    switch (commPortState->receptionistState) {
        // receive
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_RECEIVE:
            __receiveNorth();
            break;
            // transmit ack with local address
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK:
            constructSendEnumeratedACKWithAddressToParent();
            enableTransmission(&ParticleAttributes.communication.ports.tx.north);
            DEBUG_CHAR_OUT('f');
            commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED;
            break;
            // wait for tx finished
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_TRANSMIT_ACK_WAIT_TX_FINISHED:
            if (ParticleAttributes.communication.ports.tx.north.isTransmitting) {
                break;
            }
            clearReceptionBuffer(&ParticleAttributes.communication.ports.rx.north);
            DEBUG_CHAR_OUT('R');
            commPortState->receptionistState = COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE;
            break;
            // wait for response
        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_WAIT_FOR_RESPONSE:
            __receiveNorth();
            break;

        case COMMUNICATION_RECEPTIONIST_STATE_TYPE_IDLE:
            break;
    }
}

extern FUNC_ATTRS void __handleEnumerateNeighbour(volatile TxPort *txPort,
                                                  volatile CommunicationProtocolPortState *commPortState,
                                                  volatile PulseCounter *discoveryPulseCounter,
                                                  void (receptionImpl)(void),
                                                  uint8_t remoteAddressRow,
                                                  uint8_t remoteAddressColumn,
                                                  StateType endState);
/**
 * Handles "enumerate neighbour" communication states.
 */
FUNC_ATTRS void __handleEnumerateNeighbour(volatile TxPort *txPort,
                                           volatile CommunicationProtocolPortState *commPortState,
                                           volatile PulseCounter *discoveryPulseCounter,
                                           void (receptionImpl)(void),
                                           uint8_t remoteAddressRow,
                                           uint8_t remoteAddressColumn,
                                           StateType endState) {
    switch (commPortState->initiatorState) {
        // transmit new address
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT:
            if (discoveryPulseCounter->isConnected) {
                clearTransmissionPortBuffer(txPort);
                constructSendEnumeratePackage(txPort, remoteAddressRow, remoteAddressColumn);
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
            receptionImpl();
            break;

            // send ack back
        case COMMUNICATION_INITIATOR_STATE_TYPE_TRANSMIT_ACK:
            clearTransmissionPortBuffer(txPort);
            constructSendACKPackage(txPort);
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
            break;

        case COMMUNICATION_INITIATOR_STATE_TYPE_IDLE:
            ParticleAttributes.node.state = endState;
            break;
    }
}

extern FUNC_ATTRS void advanceCommunicationProtocolCounters(void);

/**
 * Advance communication timeout counters.
 */
extern FUNC_ATTRS void advanceCommunicationProtocolCounters(void) {
    if (ParticleAttributes.protocol.ports.north.stateTimeoutCounter > 0) {
        ParticleAttributes.protocol.ports.north.stateTimeoutCounter--;
    }
    if (ParticleAttributes.protocol.ports.east.stateTimeoutCounter > 0) {
        ParticleAttributes.protocol.ports.east.stateTimeoutCounter--;
    }
    if (ParticleAttributes.protocol.ports.south.stateTimeoutCounter > 0) {
        ParticleAttributes.protocol.ports.south.stateTimeoutCounter--;
    }

}

extern FUNC_ATTRS void particleTick(void);
/**
 * This function is called cyclically in the particle loop. It implements the
 * behaviour and state machine of the particle.
 */
FUNC_ATTRS void particleTick(void) {
    DEBUG_CHAR_OUT('P');
    advanceCommunicationProtocolCounters();
    __heartBeatToggle();

    //// ---------------- init states ----------------

    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_START:
            __initParticle();
            ParticleAttributes.node.state = STATE_TYPE_ACTIVE;
            break;

        case STATE_TYPE_RESET:
            __disableReception();
            constructParticleState(&ParticleAttributes);
            ParticleAttributes.node.state = STATE_TYPE_START;
            break;

        case STATE_TYPE_ACTIVE:
            ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERY;
            __enableDiscovery();
            SEI;
            break;

            //// ---------------- discovery states ----------------

        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            __discoveryLoopCount();
            if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOURS_DISCOVERY_LOOPS) {
                // discovery timeout
                __disableDiscoverySensing();
                ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERED;
            } else if (ParticleAttributes.discoveryPulseCounters.loopCount >=
                       MIN_NEIGHBOURS_DISCOVERY_LOOPS) {
                if (__updateAndDetermineNodeType()) {
                    __disableDiscoverySensing();
                    ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERED;
                } else {
                    DISCOVERY_LOOP_DELAY;
                }
                __updateOriginNodeAddress();
            }
            break;

        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
            __discoveryLoopCount();
            ParticleAttributes.node.state = STATE_TYPE_DISCOVERY_PULSING;
            break;

        case STATE_TYPE_DISCOVERY_PULSING:
            if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOUR_PULSING_LOOPS) {
                __disableDiscoveryPulsing();
                ParticleAttributes.node.state = STATE_TYPE_DISCOVERY_PULSING_DONE;
            } else {
                __discoveryLoopCount();
            }
            break;

        case STATE_TYPE_DISCOVERY_PULSING_DONE:
        DELAY_US_150;
            DELAY_US_150;
            DELAY_US_150;
            DELAY_US_150;

            if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
                ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS;
            } else {
                setReceptionistStateStart(&ParticleAttributes.protocol.ports.north);
                ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
                DEBUG_CHAR_OUT('W');
            }
            __enableReception();
            clearReceptionBuffers();
            break;

            //// ---------------- local enumeration states ----------------

            // wait for incoming address from north neighbour
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            __handleWaitForBeingEnumerated(&ParticleAttributes.protocol.ports.north);
            break;

        case STATE_TYPE_LOCALLY_ENUMERATED:
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS;
            break;

            //// ---------------- neighbour enumeration states ----------------

        case STATE_TYPE_ENUMERATING_NEIGHBOURS:
            // wait until neighbours reach state STATE_TYPE_WAIT_FOR_BEING_ENUMERATED and
            // reception times out (discovery signals may be mistakenly interpreted as data)
        DELAY_US_150;
            DELAY_US_150;
            setInitiatorStateStart(&ParticleAttributes.protocol.ports.east);
            DEBUG_CHAR_OUT('E');
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR;
            break;

            //// ---------------- east neighbour enumeration states ----------------

        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
            __handleEnumerateNeighbour(&ParticleAttributes.communication.ports.tx.east,
                                       &ParticleAttributes.protocol.ports.east,
                                       &ParticleAttributes.discoveryPulseCounters.east,
                                       __receiveEast,
                                       ParticleAttributes.node.address.row,
                                       ParticleAttributes.node.address.column + 1,
                                       STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE);
            break;

        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE:
            setInitiatorStateStart(&ParticleAttributes.protocol.ports.south);
            DEBUG_CHAR_OUT('e');
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR;
            break;

            //// ---------------- south neighbour enumeration states ----------------

        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
            __handleEnumerateNeighbour(&ParticleAttributes.communication.ports.tx.south,
                                       &ParticleAttributes.protocol.ports.south,
                                       &ParticleAttributes.discoveryPulseCounters.south,
                                       __receiveSouth,
                                       ParticleAttributes.node.address.row + 1,
                                       ParticleAttributes.node.address.column,
                                       STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE);
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE:
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE;
            break;

            //// ---------------- working states ----------------

        case STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE:
            ParticleAttributes.node.state = STATE_TYPE_IDLE;
            break;

        case STATE_TYPE_IDLE:
            __receiveNorth();
            __receiveEast();
            __receiveSouth();
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
