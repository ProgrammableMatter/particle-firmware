/**
 * @author Raoul Rubien 2016
 */

#pragma once

/**
 * On timeout the current communication falls back to the communication's start state. The counter
 * is decremented each main loop. A zero value indicates the timeout.
 * Reasonable values are ∈ [128, UINT8_MAX].
 */
#define COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX 128

/**
 * When a time synchronization package is broadcasted, each mcu introduces a lag of
 * approximate 6.5µS. Thus for 8MHz osc: 6.5*8 = ~52clocks.
 *
 * rx: --<|||||||>----
 *        ^
 * tx: -----<|||||||>----
 *           ^ introduced mcu lag
 */
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PER_NODE_INTERRUPT_LAG 52

/**
 * Mean reception to execution latency in between last interrupt and execution of sync command.
 *
 *  rx:   ---<|||||||>--------
 *                   ^ last flank/package interrupt
 *  exec: --------------#-----
 *                      ^ command execution
 */
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PACKAGE_EXECUTION_LAG 2827

/**
 * Mean package reception duration approximate 3.07mS. Thus for 8MHz osc: 3.07*8 = ~25clocks.
 * rx: -----<|||||||>------
 *           ^-----^ package reception duration
 *
 */
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PACKAGE_RECEPTION_DURATION 25

/**
 * In case of manual adjustment needed.
 */
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_MANUAL_ADJUSTMENT 0