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