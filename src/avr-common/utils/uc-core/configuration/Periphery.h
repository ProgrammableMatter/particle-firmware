/**
 * @author Raoul Rubien 2016
 *
 * Periphery related arguments.
 */

#pragma once

/**
 * Heartbeat LED toggles every main-loop count.
 */
#define HEARTBEAT_LOOP_COUNT_TOGGLE ((uint8_t)250)


/**
 * address blinking: led on duration
 */
#define BLINK_ADDRESS_LED_ON_COUNTER_MAX 7000
/**
 * address blinking: led off duration
 */
#define BLINK_ADDRESS_LED_OFF_COUNTER_MAX 7000
/**
 * address blinking: separation of rows column blinking
 */
#define BLINK_ADDRESS_LED_SEPARATION_BREAK_COUNTER_MAX 17000
/**
 * address blinking: signal quitting the end of column blinking
 */
#define BLINK_ADDRESS_LED_SEPARATION_FLASH_COUNTER_MAX 1500
/**
 * address blinking: separation of one address blinking process
 */
#define BLINK_ADDRESS_LED_SEPARATION_LONG_BREAK_COUNTER_MAX 28000
