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
 * Read, parse and interpret the specified buffer. The buffer is duplicated, thus
 * reception is possible immediately after copying from reception buffer.
 */
FUNC_ATTRS void __interpretBuffer(volatile RxPort *o) {
    if (!o->isDataBuffered) { // if no data available
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
