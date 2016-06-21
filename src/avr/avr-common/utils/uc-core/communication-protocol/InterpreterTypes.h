//
// Created by Raoul Rubien on 25.05.16.
//

#pragma once

#include <stdint.h>
#include "uc-core/communication/CommunicationTypes.h"


/**
 * keeps interpreter states
 */
typedef struct Interpreter {
//    uint8_t buffer[sizeof(((PortBuffer *) 0)->bytes)];
    uint8_t buffer[7];
} Interpreter;

