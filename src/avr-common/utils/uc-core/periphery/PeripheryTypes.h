/*
 * @author Raoul Rubien 18.09.2016
 *
 * Particle state definition.
 */

#pragma once

#include <stdint.h>

typedef enum LedBlinkStates {
    LED_BLINK_STATES_START,
    LED_BLINK_STATES_BLINK_ROW,
    LED_BLINK_STATES_ROW_ON,
    LED_BLINK_STATES_ROW_OFF,
    LED_BLINK_STATES_PAUSE,
    LED_BLINK_STATES_BLINK_COLUMN,
    LED_BLINK_STATES_COLUMN_ON,
    LED_BLINK_STATES_COLUMN_OFF,
    LED_BLINK_STATES_PAUSE2,
    LED_BLINK_STATES_QUIT_SIGNAL,
    LED_BLINK_STATES_END,
} LedBlinkStates;

typedef struct BlinkAddress {
    uint16_t blinkRowCounter;
    uint16_t blinkColumnCounter;
    uint16_t blinkAddressBlinkDelay;
    uint16_t lastExecutionTime;
    LedBlinkStates blinkAddressState;
} BlinkAddress;
/**
 * Counters/resources needed for non vital platform's periphery such as LEDs, test points and alike.
 */
typedef struct Periphery {
    // particle main loop counter
    uint8_t loopCount;
    uint16_t countdownUntilDeadEndMode;
    BlinkAddress blinkAddress;
} volatile Periphery;

