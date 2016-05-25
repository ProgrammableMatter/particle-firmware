/**
 * @author Raoul Rubien 2015
 */

#ifndef __PARTICLE_H
#define __PARTICLE_H

#include <common/common.h>
#include <uc-core/ParticleTypes.h>
#include "Globals.h"
#include "IoDefinitions.h"
#include "./delay/delay.h"
#include "./discovery/Discovery.h"
#include "./interrupts/Reception.h"
#include "./interrupts/TimerCounter.h"
#include "./interrupts/Interrupts.h"
#include "./ParticleParameters.h"
#include "uc-core/communication-protocol/CommunicationProtocol.h"
#include "uc-core/communication-protocol/Interpreter.h"


#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

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
                ParticleAttributes.node.type = NODE_TYPE_INVALID;
            } else { // !N, !S, !E
                ParticleAttributes.node.type = NODE_TYPE_ORPHAN;
            }
        }
    }
    return false;
}

FUNC_ATTRS void __disableDiscoverySensing(void) {
    RX_INTERRUPTS_DISABLE;
}

FUNC_ATTRS void __disableDiscoveryPulsing(void) {
    TIMER_NEIGHBOUR_SENSE_PAUSE;
    TIMER_NEIGHBOUR_SENSE_DISABLE;
}


/**
 * Sets up ports and interrupts but does not enable the global interrupt (I-flag in SREG)
 */
FUNC_ATTRS void __initParticle(void) {
    RX_INTERRUPTS_SETUP; // configure input pins interrupts
    RX_INTERRUPTS_ENABLE; // enable input pin interrupts
    TIMER_NEIGHBOUR_SENSE_SETUP; // configure timer interrupt for neighbour sensing
}

FUNC_ATTRS void __disableReception(void) {
    RX_NORTH_INTERRUPT_DISABLE;
    RX_EAST_INTERRUPT_DISABLE;
    RX_SOUTH_INTERRUPT_DISABLE;
}

FUNC_ATTRS void __enableReceptionNorth(void) {
    if (ParticleAttributes.discoveryPulseCounters.north.isConnected) {
        RX_NORTH_INTERRUPT_CLEAR_PENDING;
        RX_NORTH_INTERRUPT_ENABLE;
    }
}

FUNC_ATTRS void __enableReceptionEast(void) {
    if (ParticleAttributes.discoveryPulseCounters.east.isConnected) {
        RX_EAST_INTERRUPT_CLEAR_PENDING;
        RX_EAST_INTERRUPT_ENABLE;
    }
}

FUNC_ATTRS void __enableReceptionSouth(void) {
    if (ParticleAttributes.discoveryPulseCounters.south.isConnected) {
        RX_SOUTH_INTERRUPT_CLEAR_PENDING;
        RX_SOUTH_INTERRUPT_ENABLE;
    }
}

FUNC_ATTRS void __enableXmissionTimer(void) {
    TIMER_TX_RX_SETUP;
    TIMER_TX_RX_ENABLE;
}

FUNC_ATTRS void __disableXmissionTimer(void) {
    TIMER_TX_RX_DISABLE;
}

FUNC_ATTRS void __enableReceptionForConnectedPorts(void) {
    RX_INTERRUPTS_ENABLE;
    __enableReceptionNorth();
    __enableReceptionEast();
    __enableReceptionSouth();
}

FUNC_ATTRS void __disableXmission(void) {
    __disableReception();
    __disableXmissionTimer();
}

FUNC_ATTRS void __enableXmission(void) {
    __enableXmissionTimer();
    __enableReceptionForConnectedPorts();
}

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

FUNC_ATTRS void __discoveryLoopCount(void) {
    if (ParticleAttributes.discoveryPulseCounters.loopCount < (UINT8_MAX)) {
        ParticleAttributes.discoveryPulseCounters.loopCount++;
    }
}

FUNC_ATTRS void particleTick(void) {

    __heartBeatToggle();

    // STATE_TYPE_START: state before initialization
    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_START:
            __initParticle();
            ParticleAttributes.node.state = STATE_TYPE_ACTIVE;
            break;

            // STATE_TYPE_ACTIVE: switch to state discovery and enable interrupt
        case STATE_TYPE_ACTIVE:
            // enable pulsing on north and south tx wires
            ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERY;
            TIMER_NEIGHBOUR_SENSE_ENABLE;
            SREG setBit bit(SREG_I); // finally enable interrupts
            break;

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
                    DELAY_US_15;
                    DELAY_US_15;
                }
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
                if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
                    ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_NEIGHBOURS;
                } else {
                    ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
                }
                __enableXmission();
            } else {
                __discoveryLoopCount();
            }
            break;

            // reset state disables timers and switches back to the very first state
            // without restarting the mcu
        case STATE_TYPE_RESET:
            __disableXmission();
            constructParticleState(&ParticleAttributes);
            ParticleAttributes.node.state = STATE_TYPE_START;
            break;

            // wait for incoming particle address from south neighbour
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            interpretRxBuffers();
            break;

        case STATE_TYPE_ENUMERATING_NEIGHBOURS:
            // wait until neighbours reach state STATE_TYPE_WAIT_FOR_BEING_ENUMERATED
            DELAY_US_15;
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR;
            break;

        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) {
                ParticleAttributes.ports.tx.east.enableTransmission = false;
                constructSendEnumeratePackageEast(ParticleAttributes.node.address.row, ParticleAttributes.node
                                                                                               .address.column +
                                                                                       1);
                ParticleAttributes.ports.tx.east.retainTransmission = true;
                ParticleAttributes.ports.tx.east.enableTransmission = true;
            }
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR;
            break;

        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
            if (ParticleAttributes.discoveryPulseCounters.south.isConnected) {
                ParticleAttributes.ports.tx.south.enableTransmission = false;
                constructSendEnumeratePackageSouth(ParticleAttributes.node.address.row + 1,
                                                   ParticleAttributes.node
                                                           .address.column);
                ParticleAttributes.ports.tx.south.retainTransmission = true;
                ParticleAttributes.ports.tx.south.enableTransmission = true;
            }
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATED;
            break;

        case STATE_TYPE_ENUMERATED:
            ParticleAttributes.node.state = STATE_TYPE_IDLE;
            break;

        case STATE_TYPE_IDLE:
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

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif