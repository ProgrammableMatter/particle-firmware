//
// Created by Raoul Rubien on 25.05.16.
//

#ifndef __PROJECT_INTERPRETER_H__
#define __PROJECT_INTERPRETER_H__

#include "InterpreterTypes.h"
#include "CommunicationProtocolTypes.h"

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

/**
 * returns true if the buffer keeps reasonable number of bits, else false and
 * releases the buffer
 */
FUNC_ATTRS uint8_t __isReasonableBufferSizeOrClearBuffer(volatile RxPort *o) {
    // TODO: replace hardcoded values by macros
    if (o->isReceiving == false) {
        if (o->isDataBuffered) {
            if (o->buffer.pointer.bitMask == (1 << 2)) { // packages with bit mask 0x04
                // 2, 3, 4, 6, 7
                // !1, !5, !8
                if (o->buffer.pointer.byteNumber != 1 && o->buffer.pointer.byteNumber != 5 &&
                    o->buffer.pointer.byteNumber != 8) {
                    return true;
                }
            } else if (o->buffer.pointer.bitMask == (1 << 6)) { // packages with bit mask 0x40
                // 1, 3, 5
                if (o->buffer.pointer.byteNumber == 1 || o->buffer.pointer.byteNumber == 3 ||
                    o->buffer.pointer.byteNumber == 5) {
                    return true;
                }
            } else if (o->buffer.pointer.bitMask == (1 << 7)) { // packages with bit mask 0x80
                // 0, 2, 4
                if (o->buffer.pointer.byteNumber == 0 || o->buffer.pointer.byteNumber == 2 ||
                    o->buffer.pointer.byteNumber == 4) {
                    return true;
                }
            }
            // TODO: possible race condition: testing & clearing flags vs. reception
            // release buffer
            o->isOverflowed = false;
            o->isDataBuffered = false;
            IF_SIMULATION_CHAR_OUT('p');
        }
    }
    IF_SIMULATION_CHAR_OUT('P');
    return false;
}

/**
 * Read, parse and interpret the specified buffer. The buffer is duplicated, thus
 * reception is possible immediately after copying from reception buffer.
 */
FUNC_ATTRS void __interpretBuffer(volatile RxPort *o) {
    if (!__isReasonableBufferSizeOrClearBuffer(o)) {
        return;
    }

    // copy buffer
    for (uint8_t idx = 0; idx < sizeof(o->buffer.bytes); idx++) {
        ParticleAttributes.interpreter.buffer[idx] = o->buffer.bytes[idx];
    }
    // prepare reception buffer for reception
    o->isDataBuffered = false;
    o->isOverflowed = false;

    Package *package = (Package *) ParticleAttributes.interpreter.buffer;
    // interpret package header id
    switch (package->asHeader.headerId) {
        case PACKAGE_HEADER_ID_TYPE_ENUMERATE:
            ParticleAttributes.node.address.row = package->asDedicatedHeader.addressRow0;
            ParticleAttributes.node.address.column = package->asDedicatedHeader.addressColumn0;
            ParticleAttributes.node.state = STATE_TYPE_LOCALLY_ENUMERATED;
            break;
        case
            PACKAGE_HEADER_ID_TYPE_HEAT_WIRES:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_HEAT_WIRES_RANGE:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_FORWARDING_ON:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_FORWARDING_OFF:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_DISCLOSE:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_RESPONSE:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_NETWORK_GEOMETRY_REQUEST:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_RESET:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_VERBOSE_TOGGLE:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_PING_REQUEST:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_PING_RESPONSE:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_HEATING_MODE:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_RESERVED1:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_RESERVED2:
            break;
        case
            PACKAGE_HEADER_ID_TYPE_EXTENDED_HEADER:
            break;
        default:
            break;
    }
}


/**
 * interpret reception buffers of all connected ports
 */
FUNC_ATTRS void interpretRxBuffers(void) {
    if (ParticleAttributes.discoveryPulseCounters.north.isConnected) {
        __interpretBuffer(&ParticleAttributes.ports.rx.north);
    }
    if (ParticleAttributes.discoveryPulseCounters.east.isConnected) {
        __interpretBuffer(&ParticleAttributes.ports.rx.east);
    }
    if (ParticleAttributes.discoveryPulseCounters.south.isConnected) {
        __interpretBuffer(&ParticleAttributes.ports.rx.south);
    }
}

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif
