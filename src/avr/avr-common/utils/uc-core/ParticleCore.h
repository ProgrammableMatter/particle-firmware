/**
 * @author Raoul Rubien 2015
 */

#pragma once

#include "common/common.h"
#include "uc-core/ParticleStateTypes.h"
#include "Globals.h"
#include "ParticleStateTypesCtors.h"
#include "uc-core/io-configuration/IoDefinitions.h"
#include "uc-core/delay/delay.h"
#include "uc-core/discovery/Discovery.h"
#include "uc-core/interrupts/Reception.h"
#include "uc-core/interrupts/TimerCounter.h"
#include "uc-core/interrupts/Interrupts.h"
#include "uc-core/fw-configuration/ParticleParameters.h"
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

extern FUNC_ATTRS void __disableReceptionHardware(void);

FUNC_ATTRS void __disableReceptionHardware(void) {
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

extern FUNC_ATTRS void __enableTxRxTimerHardware(void);

FUNC_ATTRS void __enableTxRxTimerHardware(void) {
    TIMER_TX_RX_SETUP;
    TIMER_TX_RX_ENABLE;
}

extern FUNC_ATTRS void __disableTxRxTimerHardware(void);

FUNC_ATTRS void __disableTxRxTimerHardware(void) {
    TIMER_TX_RX_DISABLE;
}

extern FUNC_ATTRS void __enableReceptionHardwareForConnectedPorts(void);

FUNC_ATTRS void __enableReceptionHardwareForConnectedPorts(void) {
    RX_INTERRUPTS_SETUP;
    RX_INTERRUPTS_ENABLE;
    __enableReceptionHardwareNorth();
    __enableReceptionHardwareEast();
    __enableReceptionHardwareSouth();

}

extern FUNC_ATTRS void __disableTxRx(void);

FUNC_ATTRS void __disableTxRx(void) {
    ParticleAttributes.ports.xmissionState = STATE_TYPE_XMISSION_TYPE_DISABLED_TX_RX;
    __disableReceptionHardware();
    __disableTxRxTimerHardware();
}

extern FUNC_ATTRS void __enableTxRx(void);

FUNC_ATTRS void __enableTxRx(void) {
    __enableTxRxTimerHardware();
    __enableReceptionHardwareForConnectedPorts();
    ParticleAttributes.ports.xmissionState = STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX;
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

extern FUNC_ATTRS void particleTick(void);
/**
 * This function is called cyclically in the particle loop. It implements the
 * behaviour and state machine of the particle.
 */
FUNC_ATTRS void particleTick(void) {

    __heartBeatToggle();

    //// ---------------- init states ----------------

    // STATE_TYPE_START: state before initialization
    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_START:
            __initParticle();
            ParticleAttributes.node.state = STATE_TYPE_ACTIVE;
            break;

            // reset state disables timers and switches back to the very first state
            // without restarting the mcu
        case STATE_TYPE_RESET:
            __disableTxRx();
            constructParticleState(&ParticleAttributes);
            ParticleAttributes.node.state = STATE_TYPE_START;
            break;

        case STATE_TYPE_ACTIVE:
            ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERY;
            __enableDiscovery();
            SREG setBit bit(SREG_I);
            break;

            //// ---------------- discovery states ----------------

            // STATE_TYPE_NEIGHBOURS_DISCOVERY: stay in discovery state for
            // MAX_NEIGHBOURS_DISCOVERY_LOOPS but at least MIN_NEIGHBOURS_DISCOVERY_LOOPS loops
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
                    // TODO: find the lower bound and decide how log a good delay is
                    DELAY_US_15;
                    DELAY_US_15;
                }
                __updateOriginNodeAddress();
            }
            break;

            // prevent exhausting cpu clocks for reception interrupts unless rx is not needed but keep pulsing
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
            __discoveryLoopCount();
            ParticleAttributes.node.state = STATE_TYPE_DISCOVERY_PULSING;
            break;

            // keep pulsing to neighbours until maximum MAX_NEIGHBOUR_PULSING_LOOPS loops is reached,
            // then switch to enumeration state
        case STATE_TYPE_DISCOVERY_PULSING:
            if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOUR_PULSING_LOOPS) {
                __disableDiscoveryPulsing();
                DELAY_US_150;
                DELAY_US_150;
                DELAY_US_150;
                DELAY_US_150;
                if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
                    ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS;
                } else {
                    ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
                }
                clearReceptionBuffers();
                __enableTxRx();
            } else {
                __discoveryLoopCount();
            }
            break;

            //// ---------------- local enumeration states ----------------

            // wait for incoming particle address from north neighbour

        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            interpretRxBuffer(&ParticleAttributes.ports.rx.north);
            break;

        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_SEND_ACK_RESPONSE_TO_PARENT:
            constructSendEnumeratedACKWithAddressToParent();
            releaseTransmissionPortBufferForTransmission(&ParticleAttributes.ports.tx.north);
            ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_WAIT_UNTIL_ACK_RESPONSE_TO_PARENT_TRANSMISSION_FINISHED;
            break;

        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_WAIT_UNTIL_ACK_RESPONSE_TO_PARENT_TRANSMISSION_FINISHED:
            if (isPortTransmitting(&ParticleAttributes.ports.tx.north)) {
                break;
            }
            clearReceptionBuffer(&ParticleAttributes.ports.rx.north);
            ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_ACK_RESPONSE_FROM_PARENT;
            break;

        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_ACK_RESPONSE_FROM_PARENT:
            interpretRxBuffer(&ParticleAttributes.ports.rx.north);
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
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR;
            break;

            //// ---------------- east neighbour enumeration states ----------------

        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) {
                prepareTransmissionPortBuffer(&ParticleAttributes.ports.tx.east);
                constructSendEnumeratePackageEast(
                        ParticleAttributes.node.address.row,
                        ParticleAttributes.node.address.column + 1);
                releaseTransmissionPortBufferForTransmission(&ParticleAttributes.ports.tx.east);
                ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_WAIT_UNTIL_TRANSMISSION_FINISHED;
            } else {
                ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE;
            }
            break;

        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_WAIT_UNTIL_TRANSMISSION_FINISHED:
            if (isPortTransmitting(&ParticleAttributes.ports.tx.east)) {
                break;
            }
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_ACK_RESPONSE_FROM_EAST;
            break;

        case STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_ACK_RESPONSE_FROM_EAST:
            interpretRxBuffer(&ParticleAttributes.ports.rx.east);
            break;

        case STATE_TYPE_ENUMERATING_EAST_SEND_ACK_RESPONSE_TO_EAST:
            prepareTransmissionPortBuffer(&ParticleAttributes.ports.tx.east);
            constructSendACKPackage(&ParticleAttributes.ports.tx.east);
            releaseTransmissionPortBufferForTransmission(&ParticleAttributes.ports.tx.east);
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_WAIT_UNTIL_TRANSMISSION_FINISHED;
            break;


        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE:
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR;
            break;

            //// ---------------- south neighbour enumeration states ----------------

        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
            if (ParticleAttributes.discoveryPulseCounters.south.isConnected) {
                prepareTransmissionPortBuffer(&ParticleAttributes.ports.tx.south);
                constructSendEnumeratePackageSouth(
                        ParticleAttributes.node.address.row + 1,
                        ParticleAttributes.node.address.column);
                releaseTransmissionPortBufferForTransmission(&ParticleAttributes.ports.tx.south);
                ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_WAIT_UNTIL_TRANSMISSION_FINISHED;
            } else {
                ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE;
            }
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_WAIT_UNTIL_TRANSMISSION_FINISHED:
            if (isPortTransmitting(&ParticleAttributes.ports.tx.south)) {
                break;
            }
            clearReceptionBuffer(&ParticleAttributes.ports.rx.south);
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_FROM_SOUTH;
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_FROM_SOUTH:
            interpretRxBuffer(&ParticleAttributes.ports.rx.south);
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_SEND_ACK_RESPONSE_TO_SOUTH:
            prepareTransmissionPortBuffer(&ParticleAttributes.ports.tx.south);
            constructSendACKPackage(&ParticleAttributes.ports.tx.south);
            releaseTransmissionPortBufferForTransmission(&ParticleAttributes.ports.tx.south);
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_SEND_ACK_RESPONSE_TO_SOUTH_WAIT_UNTIL_TRANSMISSION_FINISHED;
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_SEND_ACK_RESPONSE_TO_SOUTH_WAIT_UNTIL_TRANSMISSION_FINISHED:
            if (isPortTransmitting(&ParticleAttributes.ports.tx.south)) {
                break;
            }
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE;
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE:
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE;
            break;

            //// ---------------- working states ----------------

        case STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE:
            ParticleAttributes.node.state = STATE_TYPE_IDLE;
            break;

        case STATE_TYPE_IDLE:
            manchesterDecodeBuffer(&ParticleAttributes.ports.rx.north);
            manchesterDecodeBuffer(&ParticleAttributes.ports.rx.east);
            manchesterDecodeBuffer(&ParticleAttributes.ports.rx.south);
            interpretRxBuffer(&ParticleAttributes.ports.rx.north);
            interpretRxBuffer(&ParticleAttributes.ports.rx.east);
            interpretRxBuffer(&ParticleAttributes.ports.rx.south);
            // if new rx buffer available -> interpret
            // if scheduled command available -> execute
            break;

        case STATE_TYPE_INTERPRET_COMMAND:
            // interpret command
            // schedule command -> switch to state schedule command
            // execute command -> switch to state execute command
            break;

            // setup and start transmission
        case STATE_TYPE_TX_START:
            // TODO: setup tx timer/counter
            break;

            // nothing to transmit any more
        case STATE_TYPE_TX_DONE:
            ParticleAttributes.node.state = STATE_TYPE_IDLE;
            break;

        case STATE_TYPE_FORWARD_PKG:
            break;

        case STATE_TYPE_SCHEDULE_COMMAND:
            // schedule command
            // switch to -> state idle
            break;

        case STATE_TYPE_COMMAND_SCHEDULED_ACK:
            break;

        case STATE_TYPE_EXECUTE_COMMAND:
            // execute command
            // switch to -> state idle
            break;

        case STATE_TYPE_ERRONEOUS:
            forever {
                LED_STATUS0_ON;
                DELAY_MS_196;
                LED_STATUS0_OFF;
                DELAY_MS_196;
            }
            break;

        default:
            ParticleAttributes.node.state = STATE_TYPE_ERRONEOUS;
            break;
    }
}
