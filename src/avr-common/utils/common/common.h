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

/**
 * memory barrier to suppress code motion around the barrier
 * http://blog.regehr.org/archives/28
 */
#define MEMORY_BARRIER asm volatile ("" : : : "memory")


/**
 * Convenience macro for "no operation" instruction.
 */
#define NOOP asm ("nop")