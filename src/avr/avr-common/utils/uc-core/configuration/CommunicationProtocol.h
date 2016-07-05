/**
 * @author Raoul Rubien 2016
 */

#pragma once

/**
 * On timeout the current communication falls back to the communication's start state. The counter
 * is decremented each main loop. A zero value indicates the timeout.
 */
#define COMMUNICATION_STATE_TIMEOUT_COUNTER 128