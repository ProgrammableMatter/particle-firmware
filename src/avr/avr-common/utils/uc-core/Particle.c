/**
 * @author Raoul Rubien 2015
 */

#include "Particle.h"
#include "Globals.h"
#include "ParticleIoDefinitions.h"
#include "ParticleInterruptDefinitions.h"
#include <util/delay.h>
#include <common/common.h>

extern volatile ParticleState GlobalState;

#define MIN_RX_NEIGHBOUR_SIGNALS_SENSE 5 // minimum signals to be detected until this side is recognized as
// connected to a neighbour
#define MIN_NEIGHBOURS_DISCOVERY_LOOPS 50 // earliest loop when local node discovery may be finished
#define MAX_NEIGHBOURS_DISCOVERY_LOOPS 200 // latest loop when local node discovery is to be aborted
#define MAX_NEIGHBOUR_PULSING_LOOPS 250 // last loop when pulsing to neighbours is to be deactivated


/**
 * Function that is called in an endless loop without delay in between to perform the particl's state
 * changes/work/communication.
 */
void particle_tick(void) {
    unsigned char i;
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
                // TODO switch directly to state enumerating
                GlobalState.state = STATE_TYPE_WAITING;
            }
            break;

        case STATE_TYPE_WAITING:

            //            NODE_TYPE_ORPHAN=(0+1)*2=2, NODE_TYPE_HEAD=4, NODE_TYPE_INTER_NODE=6, NODE_TYPE_TAIL=8
            SREG unsetBit bit(SREG_I);
            forever {
                TESTPOINT_LO;
                TESTPOINT_LO;
                TESTPOINT_LO;
                TESTPOINT_LO;
                for (i = (GlobalState.type + 1) * 2; i > 0; i--) {
                    TESTPOINT_HI;
                    TESTPOINT_LO;
                }
                TESTPOINT_LO;
                TESTPOINT_LO;
                TESTPOINT_LO;
                TESTPOINT_LO;
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
            GlobalState.state = STATE_TYPE_ERRONEOUS;
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
                LED_ON;
                _delay_ms(200);
                LED_OFF;
                _delay_ms(200);
            }
            break;
        default:
            GlobalState.state = STATE_TYPE_ERRONEOUS;
            break;
    }
}
