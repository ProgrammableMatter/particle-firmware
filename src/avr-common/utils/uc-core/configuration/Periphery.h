/**
 * @author Raoul Rubien 2016
 *
 * Periphery related arguments.
 */

#pragma once

//#define ADDRESS_BLINK_STATES_LED_SEPARATION_LONG_BREAK_COUNTER_MAX 28000
/**
 * address blinking: led on duration
 */
#define ADDRESS_BLINK_STATES_LED_ON_COUNTER_MAX ((uint8_t)30)
/**
 * address blinking: led off duration
 */
#define ADDRESS_BLINK_STATES_LED_OFF_COUNTER_MAX ((uint8_t)30)
/**
 * address blinking: separation of rows column blinking
 */
#define ADDRESS_BLINK_STATES_LED_SEPARATION_BREAK_COUNTER_MAX ((uint8_t)90)
/**
 * address blinking: signal quitting the end of column blinking
 */
#define ADDRESS_BLINK_STATES_LED_SEPARATION_FLASH_COUNTER_MAX ((uint8_t)7)
/**
 * address blinking: separation of one address blinking process
 */
#define ADDRESS_BLINK_STATES_LED_SEPARATION_LONG_BREAK_COUNTER_MAX ((uint8_t)140)

/**
 * simple heartbeat blinking: on/off
 */
#define TIME_INTERVAL_BLINK_STATES_PERIOD_MULTIPLIER ((uint8_t)60)