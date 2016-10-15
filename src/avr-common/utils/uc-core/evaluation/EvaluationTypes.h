/*
 * @author Raoul Rubien 15.10.2016
 *
 * Evaluation state definition.
 */

#pragma once

#include <stdint.h>

#include "uc-core/particle/types/NodeAddressTypes.h"

typedef struct Evaluation {
    NodeAddress nextHeatWiresAddress;
} Evaluation;
