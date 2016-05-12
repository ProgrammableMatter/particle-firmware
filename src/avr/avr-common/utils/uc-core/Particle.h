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
#include "./interrupts/Reception.h"
#include "./discovery/Discovery.h"
#include "./interrupts/TimerCounter.h"
#include "./ParticleParameters.h"

#include "./communication-protocol/CommunicationProtocolTypes.h"

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
FUNC_ATTRS bool __updateNodeType(void) {
    uint8_t hasNorthNeighbour =
            (uint8_t) ((ParticleAttributes.discoveryPulseCounters.north.counter >=
                        MIN_RX_NEIGHBOUR_SIGNALS_SENSE) ? 1 : 0);
    uint8_t hasSouthNeighbour =
            (uint8_t) ((ParticleAttributes.discoveryPulseCounters.south.counter >=
                        MIN_RX_NEIGHBOUR_SIGNALS_SENSE) ? 1 : 0);
    uint8_t hasEastNeighbour =
            (uint8_t) ((ParticleAttributes.discoveryPulseCounters.east.counter >=
                        MIN_RX_NEIGHBOUR_SIGNALS_SENSE) ? 1 : 0);
    if (hasNorthNeighbour) { // N
        if (hasSouthNeighbour) { // N, S
            if (hasEastNeighbour) { // N, S, E
                ParticleAttributes.node.type = NODE_TYPE_INTER_HEAD;
                return true;
            } else { // N,S,!E
                ParticleAttributes.node.type = NODE_TYPE_INTER_NODE;
            }
        } else { // N, !S
            if (hasEastNeighbour) { // N, !S, E
                ParticleAttributes.node.type = NODE_TYPE_INVALID;
                return true;
            } else { // N, !S, !E
                ParticleAttributes.node.type = NODE_TYPE_TAIL;
            }

        }
    } else { // !N
        if (hasSouthNeighbour) { // !N, S
            if (hasEastNeighbour) { // !N, S, E
                ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
            } else { // !N, S, !E
                ParticleAttributes.node.type = NODE_TYPE_ORIGIN;
            }
        } else { // !N, !S
            if (hasEastNeighbour) { // !N, !S, E
                ParticleAttributes.node.type = NODE_TYPE_INVALID;
            } else { // !N, !S, !E
                ParticleAttributes.node.type = NODE_TYPE_ORPHAN;
            }
        }
    }
    return false;
}


/**
 * Sets up ports and interrupts but does not enable the global interrupt (I-flag in SREG)
 */
FUNC_ATTRS void __initParticle(void) {
    RX_INTERRUPTS_SETUP; // configure input pins interrupts
    RX_INTERRUPTS_ENABLE; // enable input pin interrupts
    TIMER_NEIGHBOUR_SENSE_SETUP; // configure timer interrupt for neighbour sensing
}


/**
 * Sets up reception timer/counter interrupt and enables the interrupt.
 */
FUNC_ATTRS void __enableReception(void) {
    TIMER_TX_RX_SETUP; // set up reception and timeout counter interrupts
    TIMER_TX_RX_ENABLE; // enable reception counter interrupt
    TIMER_TX_RX_TIMEOUT_ENABLE; // enable timeout counter interrupt
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
            TIMER_NEIGHBOUR_SENSE_ENABLE;
            SREG setBit bit(SREG_I); // finally enable interrupts
            break;

            // STATE_TYPE_NEIGHBOURS_DISCOVERY: stay in discovery state for
            // MAX_NEIGHBOURS_DISCOVERY_LOOPS but at least MIN_NEIGHBOURS_DISCOVERY_LOOPS loops
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            __discoveryLoopCount();
            if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOURS_DISCOVERY_LOOPS) {
                // discovery timeout
                ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERED;
            } else if (ParticleAttributes.discoveryPulseCounters.loopCount >=
                       MIN_NEIGHBOURS_DISCOVERY_LOOPS) {
                if (__updateNodeType()) {
                    ParticleAttributes.node.state = STATE_TYPE_NEIGHBOURS_DISCOVERED;
                }
            }
            break;

            // prevent exhausting cpu clocks for reception interrupts unless rx is not needed but keep pulsing
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
            __discoveryLoopCount();
            RX_INTERRUPTS_DISABLE;
            ParticleAttributes.node.state = STATE_TYPE_DISCOVERY_PULSING;
            break;

            // keep pulsing to neighbours until maximum MAX_NEIGHBOUR_PULSING_LOOPS loops is reached,
            // then switch to enumeration state
        case STATE_TYPE_DISCOVERY_PULSING:
            if (ParticleAttributes.discoveryPulseCounters.loopCount >= MAX_NEIGHBOUR_PULSING_LOOPS) {
                TIMER_NEIGHBOUR_SENSE_DISABLE;

                if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
                    ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR;
                } else {
                    ParticleAttributes.node.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
                }
            }
            break;

            // reset state disables timers and switches back to the very first state
        case STATE_TYPE_RESET:
            TIMER_NEIGHBOUR_SENSE_DISABLE;
            TIMER_TX_RX_DISABLE;
            ParticleAttributes.node.state = STATE_TYPE_ACTIVE;
            break;

            // wait for incoming particle address from south neighbour
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
            __enableReception();
            // wait for the start bit;
            // on start bit received
            //      * enable reception interrupts on both (falling and rising) edge
            //      * start to alternate exactly 16 times the "receiving"
            //      clock as expected in the differential manchester coding
            //      * log transition occurrence of interrupt for each of the 16 clocks
            // now we have received the local id -> is parity valid
            //      true: switch state to STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR
            //      false: switch state to STATE_TYPE_ERRONEOUS
            break;
        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:

            if (ParticleAttributes.discoveryPulseCounters.south.isConnected) {
                // enumerate south
            }
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR;


            // active enumerating, send start bit and send neighbour id
            // enable receive interrupts on rising edges only
            // send start bit
            // wait (with timeout) for neighbours "receive"-clock
            //      * on neighbours "receive"-clock rising edges
            //          - do transmit a transition (toggle) to indicate the "1" byte
            //          - else to indicate a "0" byte do nothing
            // after exactly 16 "receive"-clock cycles switch state to STATE_TYPE_ENUMERATED

            break;
        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:

            if (ParticleAttributes.discoveryPulseCounters.east.isConnected) {
                // enumerate south neighbour
            }
            ParticleAttributes.node.state = STATE_TYPE_ENUMERATED;
            break;
        case STATE_TYPE_ENUMERATED:
            // switch to -> state idle
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