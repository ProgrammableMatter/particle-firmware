/**
 * @author Raoul Rubien  20.07.2016
 */

#pragma once

#include <inttypes.h>
/**
 * maximum duty cycle
 */
#define ACTUATION_COMPARE_VALUE_POWER_MAXIMUM (UINT8_MAX)
/**
 * 75% duty cycle
 */
#define ACTUATION_COMPARE_VALUE_POWER_STRONG ((UINT8_MAX / 4) * 3)
/**
 * 50% duty cycle
 */
#define ACTUATION_COMPARE_VALUE_POWER_MEDIUM (UINT8_MAX / 2)
/**
 * 25% duty cycle
 */
#define ACTUATION_COMPARE_VALUE_POWER_WEAK (UINT8_MAX / 4)
