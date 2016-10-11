/*
 * @author Raoul Rubien 09.10.2016
 *
 * Alerts state definition.
 */

#pragma once

#include <stdint.h>

/**
 * alert switches to en-/disable alerts on runtime
 */
typedef struct Alerts {
    uint8_t isRxBufferOverflowEnabled : 1;
    uint8_t isRxParityErrorEnabled : 1;
    uint8_t isGenericErrorEnabled : 1;
    uint8_t __pad  : 5;
} Alerts;