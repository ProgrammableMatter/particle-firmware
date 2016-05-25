//
// Created by Raoul Rubien on 25.05.16.
//

#ifndef __PROJECT_INTERPRETER_H__
#define __PROJECT_INTERPRETER_H__

#include "InterpreterTypes.h"

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
    if (!o->isDataBuffered) {
        return;
    }

    // TODO: to improve performance copy just received data, thus received bytes must be counted
    for (uint8_t idx = 0; idx < sizeof(o->buffer.bytes); idx++) {
        ParticleAttributes.interpreter.buffer[idx] = o->buffer.bytes[idx];
    }
    o->isDataBuffered = false;
    o->isOverflowed = false;


    PackageHeader *header = getReceptionPackage(*o, PackageHeader);
    switch (header->headerId) {
        // TODO: interpret packages

        case PACKAGE_HEADER_ID_TYPE_ENUMERATE:
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
