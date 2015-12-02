/**
 * @author Raoul Rubien 2015
 */

#ifndef PARTICLE_GLOBALS_H
#define PARTICLE_GLOBALS_H

#include <common/PortInteraction.h>
#include <uc-core/ParticleTypes.h>

volatile ParticleState GlobalState;

/**
 * mask for rx-counters in ParticleState.rxCounters
 */
#define __RX_NORTH_BIT_COUNTER_MASK 0b00001111
#define __RX_SOUTH_BIT_COUNTER_MASK 0b11110000

/**
 * convenience macros for retreiving and incrementing rx-counters
 */

#define RX_NORTH_BIT_COUNTER_GET (GlobalState.rxBitCounter getBit __RX_NORTH_BIT_COUNTER_MASK)
#define RX_NORTH_BIT_COUNTER_INCREMENT (GlobalState.rxBitCounter = \
    (RX_NORTH_BIT_COUNTER_GET + 1) | (GlobalState.rxBitCounter getBit __RX_SOUTH_BIT_COUNTER_MASK))
#define RX_NORTH_BIT_COUNTER_RESET (GlobalState.rxBitCounter = \
        (GlobalState.rxBitCounter getBit __RX_SOUTH_BIT_COUNTER_MASK))

#define RX_SOUTH_BIT_COUNTER_GET ((GlobalState.rxBitCounter getBit __RX_SOUTH_BIT_COUNTER_MASK)>>4)


#define RX_SOUTH_BIT_COUNTER_INCREMENT (GlobalState.rxBitCounter = \
    (((GlobalState.rxBitCounter>>4) + 1)<<4) | (GlobalState.rxBitCounter getBit __RX_NORTH_BIT_COUNTER_MASK))
#define RX_SOUTH_BIT_COUNTER_RESET (GlobalState.rxBitCounter = \
        (GlobalState.rxBitCounter getBit __RX_NORTH_BIT_COUNTER_MASK))

/**
 * fags in ParticleState.flags register
 */
#define __STATE_TYPE_FLAG_RECORD_RX_NORTH_ENABLE 0b00000001
#define __STATE_TYPE_FLAG_RECORD_RX_SOUTH_ENABLE 0b00000010

/**
 * convenience macros for en-/disabling ParticleState.flags[1:0]
 */
#define ENABLE_GLOBAL_STATE_RECORD_RX_NORTH GlobalState.flags setBit __STATE_TYPE_FLAG_RECORD_RX_NORTH_ENABLE
#define DISABLE_GLOBAL_STATE_RECORD_RX_NORTH GlobalState.flags unsetBit __STATE_TYPE_FLAG_RECORD_RX_NORTH_ENABLE
#define ENABLE_GLOBAL_STATE_RECORD_RX_SOUTH GlobalState.flags setBit __STATE_TYPE_FLAG_RECORD_RX_SOUTH_ENABLE
#define DISABLE_GLOBAL_STATE_RECORD_RX_SOUTH GlobalState.flags unsetBit __STATE_TYPE_FLAG_RECORD_RX_SOUTH_ENABLE

#define ENABLE_GLOBAL_STATE_RECORD_RX GlobalState.flags setBit (__STATE_TYPE_FLAG_RECORD_RX_NORTH_ENABLE | \
    __STATE_TYPE_FLAG_RECORD_RX_SOUTH_ENABLE)
#define DISABLE_GLOBAL_STATE_RECORD_RX GlobalState.flags unsetBit (__STATE_TYPE_FLAG_RECORD_RX_NORTH_ENABLE | \
    __STATE_TYPE_FLAG_RECORD_RX_SOUTH_ENABLE

#define GLOBAL_STATE_IS_RECORD_RX_NORTH_ENABLED (0 != (GlobalState.flags getBit \
    __STATE_TYPE_FLAG_RECORD_RX_NORTH_ENABLE))
#define GLOBAL_STATE_IS_RECORD_RX_SOUTH_ENABLED (0 != (GlobalState.flags getBit \
    __STATE_TYPE_FLAG_RECORD_RX_SOUTH_ENABLE))

#endif
