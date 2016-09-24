/**
 * @author Raoul Rubien  12.07.2016
 *
 * Actuation types definition.
 */

#pragma once

/**
 * Describes all possible output modes.
 */
typedef enum {
    HEATING_LEVEL_TYPE_MAXIMUM = 0x0,
    HEATING_LEVEL_TYPE_STRONG = 0x1,
    HEATING_LEVEL_TYPE_MEDIUM = 0x2,
    HEATING_LEVEL_TYPE_WEAK = 0x3,
} HeatingLevelType;

/**
 * Describes the actuation command execution state.
 */
typedef enum {
    ACTUATION_STATE_TYPE_IDLE,
    ACTUATION_STATE_TYPE_START,
    ACTUATION_STATE_TYPE_WORKING,
    ACTUATION_STATE_TYPE_RELAXATION_PAUSE,
    ACTUATION_STATE_TYPE_DONE,
} ActuationStateType;

/**
 * Describes which wires are to be actuated. South and east wires will be
 * actuated passively (pulled up or low), whereas north wires will be pulsed
 * according to the power configuration.
 * Only north wires are driven by IDR, thus volatile.
 */
typedef struct {
    /**
     * north left wire
     */
    volatile uint8_t northLeft : 1;
    /**
     * north right wire
     */
    volatile uint8_t northRight : 1;
    /**
     * east left wire: in case of folding along x-axis
     */
    uint8_t eastLeft : 1;
    /**
     * east right wire: in case of folding along x-axis
     */
    uint8_t eastRight : 1;
    /**
     * south left wire
     */
    uint8_t southLeft :1;
    /**
     * south right wire
     */
    uint8_t southRight :1;

    uint8_t __pad : 2;
} Actuators;

/**
 * Describes the heating mode/output power.
 */
typedef struct {
    uint8_t dutyCycleLevel : 2;
    uint8_t __pad : 6;
} HeatingMode;

/**
 * describes a local time stamp
 */
typedef struct {
    uint16_t periodTimeStamp;
} LocalTime;

/**
 * Describes an actuation command.
 */
typedef struct {
    /**
     * actuator flags
     */
    Actuators actuators;
    /**
     * power level configuration for all actuators
     */
    HeatingMode actuationPower;
    /**
     * execution state machine states
     */
    ActuationStateType executionState;
    /**
     * actuation start time
     */
    LocalTime actuationStart;
    /**
     * actuation end time
     */
    LocalTime actuationEnd;
    /**
     * scheduled flag: must be set to enable scheduling;
     * is unset when execution has finished
     */
    uint8_t isScheduled : 1;
    uint8_t __pad : 7;
} ActuationCommand;
