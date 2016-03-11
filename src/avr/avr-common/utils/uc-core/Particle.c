/**
 * @author Raoul Rubien 2015
 */

#include "Particle.h"
#include "Globals.h"
#include "IoDefinitions.h"
#include "InterruptDefinitions.h"
#include <util/delay.h>
#include <common/common.h>
#include <uc-core/ParticleTypes.h>

extern volatile ParticleState ParticleAttributes;

static unsigned char updateNodeType(void);

#define MIN_RX_NEIGHBOUR_SIGNALS_SENSE 5 // minimum signals to be detected until this side is recognized as connected to a neighbour
#define MIN_NEIGHBOURS_DISCOVERY_LOOPS 50 // earliest loop when local node discovery may be finished
#define MAX_NEIGHBOURS_DISCOVERY_LOOPS 200 // latest loop when local node discovery is to be aborted
#define MAX_NEIGHBOUR_PULSING_LOOPS 250 // last loop when pulsing to neighbours is to be deactivated

/**
 * Function that is called in an endless loop without delay in between to perform the particle's state
 * changes, work and communication.
 */
void particleTick(void) {
    LED_HEARTBEAT_TOGGLE;
    ParticleAttributes.rxDiscoveryPulseCounters.loopCount++;

    switch (ParticleAttributes.state) {

        // STATE_TYPE_ACTIVE: switch to state discovery and enable interrupt
        case STATE_TYPE_ACTIVE:
            ParticleAttributes.state = STATE_TYPE_NEIGHBOURS_DISCOVERY;
            // enable pulsing on north and south tx wires
            TIMER0_NEIGHBOUR_SENSE_ENABLE;
            SREG setBit bit(SREG_I); // finally enable interrupts
            break;

            // STATE_TYPE_NEIGHBOURS_DISCOVERY: stay in discovery state for
            // MAX_NEIGHBOURS_DISCOVERY_LOOPS but at least MIN_NEIGHBOURS_DISCOVERY_LOOPS loops
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (ParticleAttributes.rxDiscoveryPulseCounters.loopCount >= MAX_NEIGHBOURS_DISCOVERY_LOOPS) {
                // discovery timeout
                ParticleAttributes.state = STATE_TYPE_NEIGHBOURS_DISCOVERED;
            } else if (ParticleAttributes.rxDiscoveryPulseCounters.loopCount >=
                       MIN_NEIGHBOURS_DISCOVERY_LOOPS) {
                if (updateNodeType() == 1) {
                    ParticleAttributes.state = STATE_TYPE_NEIGHBOURS_DISCOVERED;
                }
            }
            break;

            // prevent exhausting cpu clocks for reception interrupts unless rx is not needed but keep pulsing
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
        RX_INTERRUPTS_DISABLE;
            ParticleAttributes.state = STATE_TYPE_DISCOVERY_PULSING;
            break;

            // keep pulsing to neighbours until maximum MAX_NEIGHBOUR_PULSING_LOOPS loops is reached
        case STATE_TYPE_DISCOVERY_PULSING:
            if (ParticleAttributes.rxDiscoveryPulseCounters.loopCount >= MAX_NEIGHBOUR_PULSING_LOOPS) {
                TIMER0_NEIGHBOUR_SENSE_DISABLE;
                // TODO switch directly to state enumerating
                ParticleAttributes.state = STATE_TYPE_WAITING;
            }
            break;

        case STATE_TYPE_WAITING:
            //            NODE_TYPE_ORPHAN=(0+1)*2=2, NODE_TYPE_HEAD=4, NODE_TYPE_INTER_NODE=6, NODE_TYPE_TAIL=8
            SREG unsetBit bit(SREG_I);
            forever {
            }

            // wait for enumeration command
            // switch to -> state enumerated

            //            if (GlobalState.type == NODE_TYPE_HEAD) {
            //                GlobalState.nodeId = 1;
            //                GlobalState.state = STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR;
            //            }
            //            GlobalState.state = STATE_TYPE_WAIT_FOR_BEING_ENUMERATED;
            break;
        case STATE_TYPE_RESET:
            //            asm("BREAK");
            ParticleAttributes.state = STATE_TYPE_ERRONEOUS;
            // reset internal states, maybe total hw reset
            // switch to -> state active

            break;
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED: // passive enumerating mode, waiting for local id
            // enable reception interrupts on falling edge
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
        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR: // active enumerating, send start bit and send neighbour id
            // enable receive interrupts on rising edges only
            // send start bit
            // wait (with timeout) for neighbours "receive"-clock
            //      * on neighbours "receive"-clock rising edges
            //          - do transmit a transition (toggle) to indicate the "1" byte
            //          - else to indicate a "0" byte do nothing
            // after exactly 16 "receive"-clock cycles switch state to STATE_TYPE_ENUMERATED
            break;
        case STATE_TYPE_ENUMERATED:
            // switch to -> state idle
            break;
        case STATE_TYPE_IDLE:
            // wait for command or
            // are scheduled to be executed?
            // on command switch to -> state interprete command
            // on cheduled switch to -> state execute command
            break;
        case STATE_TYPE_INTERPRETE_COMMAND:
            // interprete command
            // schedule command -> switch to state schedule command
            // execute command -> switch to state execute command
            break;
        case STATE_TYPE_RX_A:
            break;
        case STATE_TYPE_RX_B:
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
                _delay_ms(200);
                LED_STATUS0_OFF;
                _delay_ms(200);
            }
            break;
        default:
            ParticleAttributes.state = STATE_TYPE_ERRONEOUS;
            break;
    }
}

/**
 * Updates the node type according to the amount of incoming pulses. The type {@link NodeType} is stored to
 * {@link ParticleAttributes.type}.
 * @return 1 if the node is fully connected, else 0
 */
static unsigned char updateNodeType(void) {
    unsigned char hasNorthNeighbour =
            (unsigned char) ((ParticleAttributes.rxDiscoveryPulseCounters.north >=
                              MIN_RX_NEIGHBOUR_SIGNALS_SENSE) ? 1 : 0);
    unsigned char hasSouthNeighbour =
            (unsigned char) ((ParticleAttributes.rxDiscoveryPulseCounters.south >=
                              MIN_RX_NEIGHBOUR_SIGNALS_SENSE) ? 1 : 0);
    unsigned char hasEastNeighbour =
            (unsigned char) ((ParticleAttributes.rxDiscoveryPulseCounters.east >=
                              MIN_RX_NEIGHBOUR_SIGNALS_SENSE) ? 1 : 0);

    if (hasNorthNeighbour) {
        if (hasSouthNeighbour) {
            if (hasEastNeighbour) {
                ParticleAttributes.type = NODE_TYPE_INTER_HEAD;
                return 1;
            } else {
                ParticleAttributes.type = NODE_TYPE_INTER_NODE;
            }
        } else {
            ParticleAttributes.type = NODE_TYPE_INVALID;
        }
    } else {
        if (hasSouthNeighbour) {
            if (hasEastNeighbour) {
                ParticleAttributes.type = NODE_TYPE_ORIGIN;
            } else {
                ParticleAttributes.type = NODE_TYPE_INVALID;
            }
        } else {
            if (hasEastNeighbour) {
                ParticleAttributes.type = NODE_TYPE_INVALID;
            } else {
                ParticleAttributes.type = NODE_TYPE_ORPHAN;
            }
        }
    }
    return 0;
}

/**
 * Take a current reception signal snapshot for later sense control since the MCU is not capable of filtering flank the
 * direction on pin change interrupt. The interrupt is triggered at any logical change.
 */
void particleSnapshotRxFlanks(void) {
    ParticleAttributes.rxInterruptFlankStates.north = NORTH_RX_IS_HI;
    ParticleAttributes.rxInterruptFlankStates.south = SOUTH_RX_IS_HI;
    ParticleAttributes.rxInterruptFlankStates.east = EAST_RX_IS_HI;
    ParticleAttributes.rxInterruptFlankStates.isInitialized = 1;
}
