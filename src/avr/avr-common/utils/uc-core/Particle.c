/**
 * Created on: Mar 1, 2015
 * @author: Raoul Rubien
 */

#include "Particle.h"
#include "Globals.h"
#include "ParticleIoDefinitions.h"
#include "ParticleTypes.h"
#include <uc-core/ParticleInterruptDefinitions.h>

extern volatile ParticleState GlobalState;

#define MIN_RX_NEIGHBOUR_SIGNALS_SENSE 10 // minimum signals to be detected until this side is recognized as
// connected to a neighbour
#define MIN_NEIGHBOURS_DISCOVERY_LOOPS 50 // earliest loop when local node discovery may be finished
#define MAX_NEIGHBOURS_DISCOVERY_LOOPS 150 // latest loop when local node discovery is to be aborted
#define MAX_NEIGHBOUR_PULSING_LOOPS 300 // last loop when pulsing to neighbours is to be deactivated


void particle_tick(void) {
    static volatile unsigned char loopCount = 0;
    loopCount++;

    LED_TOGGLE;

    switch (GlobalState.state) {
        case STATE_TYPE_ACTIVE:
            GlobalState.state = STATE_TYPE_NEIGHBOURS_DISCOVERY;
            // enable pulsing on north and south tx wires
            ENABLE_TIMER0_INTERRUPT;
            SREG setBit bit(SREG_I); // finally enable interrupts
            // switch to -> state discovery
            break;


        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            // stay in discovrey state for some MAX_NEIGHBOURS_DISCOVERY_LOOPS but at least
            // MIN_NEIGHBOURS_DISCOVERY_LOOPS loops

            // discovery timeout -> abort discovery
            if (loopCount >= MAX_NEIGHBOURS_DISCOVERY_LOOPS) {
                GlobalState.state = STATE_TYPE_DISCOVERY_PULSING;
            } else if (loopCount >= MIN_NEIGHBOURS_DISCOVERY_LOOPS) {
                // detect neighbours
                if (GlobalState.southRxEvents >= MIN_RX_NEIGHBOUR_SIGNALS_SENSE) {
                    // found both neighbours -> abort discovery
                    if (GlobalState.northRxEvents >= MIN_RX_NEIGHBOUR_SIGNALS_SENSE) {
                        GlobalState.type = NODE_TYPE_INTER_NODE;
                        GlobalState.state = STATE_TYPE_DISCOVERY_PULSING;
                    } else {
                        GlobalState.type = NODE_TYPE_HEAD;
                    }
                } else {
                    if (GlobalState.northRxEvents >= MIN_RX_NEIGHBOUR_SIGNALS_SENSE) {
                        GlobalState.type = NODE_TYPE_TAIL;
                    } else {
                        GlobalState.type = NODE_TYPE_ORPHAN;
                    }
                }
            }
            break;

        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
            // prevent exhausting clocks for reception interrupts unless rx is not needed
            DISABLE_RX_INTERRUPTS;
            GlobalState.state = STATE_TYPE_DISCOVERY_PULSING;
            break;

        case STATE_TYPE_DISCOVERY_PULSING:
            // keep pulsing neighbours maximum MAX_NEIGHBOUR_PULSING_LOOPS loops
            if (loopCount >= MAX_NEIGHBOUR_PULSING_LOOPS) {
                DISABLE_TIMER0_INTERRUPT;
                GlobalState.state = STATE_TYPE_WAITING;
            }
            break;

        case STATE_TYPE_WAITING:
            // wait for enumeration command
            // switch to -> state enumerated

            if (GlobalState.type == NODE_TYPE_HEAD) {
                GlobalState.nodeId = 1;
                GlobalState.state = STATE_TYPE_ENUMERATING;
            }
            GlobalState.state = STATE_TYPE_WAIT_FOR_ENUMERATING;
            break;
        case STATE_TYPE_RESET:
            // run asm("BREAK")
            // reset internal states, maybe total hw reset
            // switch to -> state active
            break;
        case STATE_TYPE_WAIT_FOR_ENUMERATING:
            // enable reception
            // receive local id
            // go to enumerating state
            break;
        case STATE_TYPE_ENUMERATING:
            // wait for predecessor's id
            // determine our id
            // send next id to succesor
            // switch to -> state enumerated
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
        default:
            GlobalState.state = STATE_TYPE_ERRONEOUS;
            break;
    }
}
