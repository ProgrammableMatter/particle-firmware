/*
 * @author Raoul Rubien 09.10.2016
 *
 * Alerts types constructor implementation.
 */

#pragma once

#include "AlertsTypes.h"

/**
 * constructor function
 * @param o the object to construct
 */
void constructAlerts(Alerts *const o) {
    o->isRxBufferOverflowEnabled = false;
    o->isRxParityErrorEnabled = false;
    o->isGenericErrorEnabled = false;
}