/*
 * @author Raoul Rubien 18.09.2016
 *
 * Periphery types constructor implementation.
 */

#pragma once

#include "Periphery.h"


extern CTOR_ATTRS void constructBlinkAddress(volatile BlinkAddress *o);

CTOR_ATTRS void constructBlinkAddress(volatile BlinkAddress *o) {
    o->blinkColumnCounter = 0;
    o->blinkRowCounter = 0;
    o->blinkAddressBlinkDelay = 0;
    o->lastExecutionTime = 0;
    o->blinkAddressState = LED_BLINK_STATES_START;
}


/**
* constructor function
* @param o the object to construct
**/
extern CTOR_ATTRS void constructPeriphery(volatile Periphery *o);

CTOR_ATTRS void constructPeriphery(volatile Periphery *o) {
    o->loopCount = 0;
    o->countdownUntilDeadEndMode = 500;
    constructBlinkAddress(&o->blinkAddress);
}

