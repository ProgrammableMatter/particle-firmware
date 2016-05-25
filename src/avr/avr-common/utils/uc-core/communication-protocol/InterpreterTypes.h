//
// Created by Raoul Rubien on 25.05.16.
//

#ifndef __PROJECT_INTERPRETER_TYPES_H__
#define __PROJECT_INTERPRETER_TYPES_H__

#include <uc-core/communication/CommunicationTypes.h>

#  ifdef TRY_INLINE
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

/**
 * keeps interpreter states
 */
typedef struct {
    uint8_t buffer[sizeof(((PortBuffer *) 0)->bytes)];
} Interpreter;

#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif
