//
// Created by Raoul Rubien on 25.05.16.
//

#pragma once

#include "uc-core/communication/CommunicationTypes.h"

#ifdef TRY_INLINE
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif

/**
 * keeps interpreter states
 */
typedef struct Interpreter {
    uint8_t buffer[sizeof(((PortBuffer *) 0)->bytes)];
} Interpreter; // 8 bytes total

#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif
