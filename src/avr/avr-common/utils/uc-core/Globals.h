/**
 * @author Raoul Rubien
 * on 21.11.15
 */

#ifndef PROJECT_GLOBALS_H
#define PROJECT_GLOBALS_H

#include "ParticleTypes.h"

volatile ParticleState GlobalState;

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
