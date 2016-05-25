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
 * Read, parse and interpret the buffer for the specified port. The buffer is duplicated, thus
 * reception is active immediately after copying from reception buffer.
 */
FUNC_ATTRS void interpretRxBuffer(volatile RxPort *o, volatile Interpreter *i) {
    if (!o->isDataBuffered) {
        return;
    }

    // TODO: to improve performance copy just received data, thus received bytes must be counted
    for (uint8_t idx = 0; idx < sizeof(o->buffer.bytes); idx++) {
        i->buffer[idx] = o->buffer.bytes[idx];
    }

    o->isDataBuffered = false;
    o->isOverflowed = false;

    // TODO: interpret data
    // cast received bytes to package
    // interpret package
    // execute
}

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif
