/**
 * Created on: Mar 1, 2015
 * @author: Raoul Rubien
 */

#include "Particle.h"

#include "Globals.h"

extern ParticleState GlobalState;

void particle_tick(void) {
    switch (GlobalState.state) {
        case STATE_TYPE_ACTIVE:
            // enable toggling to north and south
            // switch to -> state discovery
            break;
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            // stay in discovrey exact n[ms]
            // detect neighbours
            // switch to -> sate waiting
            break;
        case STATE_TYPE_WAITING:
            // wait for enumeration command
            // switch to -> state enumerated
            break;
        case STATE_TYPE_ENUMERATING:
            // wait for predecessor's id
            // determine our id
            // send next id to succesor
            // switch to -> state enumerated
            break;
        case STATE_TYPE_RESET:
            // run asm("BREAK")
            // reset internal states, maybe total hw reset
            // switch to -> state active
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
            break;
    }
}
//
//void Particle() {
//}
//
//void Particle_writeBits(Channel ch, uint8_t bytes[], uint8_t numBits) {
//}
//
//void Particle_readBits(Channel ch, uint8_t buffer[], uint8_t size, uint8_t *bits) {
//}
//
//void Particle_powerOn(Channel ch, Line line) {
//}
//
//void Particle_tick() {
//}
