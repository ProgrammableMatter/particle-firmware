/**
 * @author Raoul Rubien  20.07.2016
 *
 * Actuation related arguments.
 */

#pragma once

#include <inttypes.h>

/**
 * ~75% actuation duty cycle
 */
#define ACTUATION_COMPARE_VALUE_POWER_STRONG ((UINT8_MAX / 4) * 3)
/**
 * ~50% actuation duty cycle
 */
#define ACTUATION_COMPARE_VALUE_POWER_MEDIUM (UINT8_MAX / 2)
/**
 * ~25% actuation duty cycle
 */
#define ACTUATION_COMPARE_VALUE_POWER_WEAK (UINT8_MAX / 4)
