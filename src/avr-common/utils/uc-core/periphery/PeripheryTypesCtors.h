/*
 * @author Raoul Rubien 18.09.2016
 *
 * Periphery types constructor implementation.
 */

#pragma once

#include "Periphery.h"


extern CTOR_ATTRS void constructBlinkAddress(BlinkAddress *o);
/**
* constructor function
* @param o the object to construct
**/
CTOR_ATTRS void constructBlinkAddress(BlinkAddress *o) {
    o->blinkColumnCounter = 0;
    o->blinkRowCounter = 0;
    o->blinkAddressBlinkDelay = 0;
    o->lastExecutionTime = 0;
    o->blinkAddressState = ADDRESS_BLINK_STATES_START;
}

extern CTOR_ATTRS void constructBlinkTimeInterval(BlinkTimeInterval *o);
/**
* constructor function
* @param o the object to construct
**/
CTOR_ATTRS void constructBlinkTimeInterval(BlinkTimeInterval *o) {
    o->lastExecutionTime = 0;
    o->localTimeMultiplier = TIME_INTERVAL_BLINK_STATES_PERIOD_MULTIPLIER;
    o->blinkState = TIME_INTERVAL_BLINK_STATES_LED_OFF;
}

/**
* constructor function
* @param o the object to construct
**/
extern CTOR_ATTRS void constructPeriphery(Periphery *o);

CTOR_ATTRS void constructPeriphery(Periphery *o) {
    o->loopCount = 0;
    constructBlinkAddress(&o->blinkAddress);
    constructBlinkTimeInterval(&o->blinkTimeInterval);
}

