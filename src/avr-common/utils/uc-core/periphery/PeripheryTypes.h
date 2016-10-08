/*
 * @author Raoul Rubien 18.09.2016
 *
 * Particle state definition.
 */

#pragma once

#include <stdint.h>

typedef enum AddressBlinkStates {
    ADDRESS_BLINK_STATES_START,
    ADDRESS_BLINK_STATES_BLINK_ROW,
    ADDRESS_BLINK_STATES_ROW_ON,
    ADDRESS_BLINK_STATES_ROW_OFF,
    ADDRESS_BLINK_STATES_PAUSE,
    ADDRESS_BLINK_STATES_BLINK_COLUMN,
    ADDRESS_BLINK_STATES_COLUMN_ON,
    ADDRESS_BLINK_STATES_COLUMN_OFF,
    ADDRESS_BLINK_STATES_PAUSE2,
    ADDRESS_BLINK_STATES_QUIT_SIGNAL,
    ADDRESS_BLINK_STATES_END,
} AddressBlinkStates;

typedef struct BlinkAddress {
    uint8_t blinkRowCounter;
    uint8_t blinkColumnCounter;
    uint8_t blinkAddressBlinkDelay;
    uint16_t lastExecutionTime;
    AddressBlinkStates blinkAddressState;
} BlinkAddress;

typedef enum TimeIntervalBlinkStates {
    TIME_INTERVAL_BLINK_STATES_LED_ON,
    TIME_INTERVAL_BLINK_STATES_LED_OFF,
} TimeIntervalBlinkStates;

typedef struct BlinkTimeInterval {
    uint8_t localTimeMultiplier;
    uint16_t lastExecutionTime;
    TimeIntervalBlinkStates blinkState;
} BlinkTimeInterval;

/**
 * Counters/resources needed for non vital platform's periphery such as LEDs, test points and alike.
 */
typedef struct Periphery {
    // particle main loop counter
    uint8_t loopCount;
    BlinkAddress blinkAddress;
    BlinkTimeInterval blinkTimeInterval;
//    // validation code for emasuring forward latency
//    volatile uint8_t isTxSouthToggleEnabled : 1;
//    uint8_t _pad: 7;
} Periphery;

