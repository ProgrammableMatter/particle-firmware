/**
 * @author Raoul Rubien 2016
 *
 * Communication protocol related arguments.
 */

#pragma once

/**
 * On timeout the current communication falls back to the communication's start state. The counter
 * is decremented each main loop. A zero value indicates the timeout.
 * Reasonable values are ∈ [128, UINT8_MAX].
 */
#define COMMUNICATION_PROTOCOL_TIMEOUT_COUNTER_MAX ((uint8_t)250)

#define COMMUNICATION_PROTOCOL_RETRANSMISSION_COUNTER_MAX ((uint8_t)3)

/**
 * When a time synchronization package is broadcasted, each mcu introduces a lag of
 * approximate 6.5µS. Thus for 8MHz osc: 0.0065*8 = ~0.052clocks.
 *
 * rx: --<|||||||>----
 *        ^
 * tx: -----<|||||||>----
 *           ^ introduced mcu lag
 */
// 32,64
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PER_NODE_INTERRUPT_LAG ((uint16_t)33)

/**
 * Mean package reception duration approximate = ~57344 mcu clocks, or more accurate:
 * 8bytes*8bits*tx_clock_phase_delay => 8*8*1024 = 65536
 *
 * rx: -----<|||||||>------
 *           ^-----^ sync. package reception duration
 */
//#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PACKAGE_RECEPTION_DURATION ((uint16_t)57320)
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PACKAGE_RECEPTION_DURATION ((uint16_t)0)

/**
 * Mean reception to execution latency in between last interrupt and execution of sync command.
 *
 *  tx (1,1):   -----<|||||||>--------
 *                           ^ last flank/package interrupt
 *  receiver exec: --------------#-----
 *                               ^ command execution
 * ~334us
 */
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_PACKAGE_EXECUTION_LAG ((uint16_t)2520)

/**
 * In case of manual adjustment needed.
 */
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_MANUAL_POSITIVE_ADJUSTMENT ((uint16_t)1476)
#define COMMUNICATION_PROTOCOL_TIME_SYNCHRONIZATION_MANUAL_NEGATIVE_ADJUSTMENT ((uint16_t)0)
