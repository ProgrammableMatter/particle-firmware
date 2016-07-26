/**
 * @author Raoul Rubien 2011
 */

#pragma once

#include <inttypes.h>
#include "PortInteraction.h"
#include <stdbool.h>

#define forever while(1)

#define SEI SREG setBit bit(SREG_I)
#define CLI SREG unsetBit bit(SREG_I)