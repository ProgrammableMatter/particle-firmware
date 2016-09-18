/*
 * @author Raoul Rubien 18.09.2016
 *
 * Periphery types constructor implementation.
 */

#pragma once

#include "Periphery.h"

/**
* constructor function
* @param o the object to construct
**/
extern CTOR_ATTRS void constructPeriphery(volatile Periphery *o);

CTOR_ATTRS void constructPeriphery(volatile Periphery *o) {
    o->loopCount = 0;
    o->countdownUntilDeadEndMode = 500;
}