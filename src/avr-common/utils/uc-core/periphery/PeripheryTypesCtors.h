/*
 * @author Raoul Rubien 18.09.2016
 *
 * Periphery types constructor implementation.
 */

#pragma once

#include "PeripheryTypes.h"

/**
* constructor function
* @param o the object to construct
**/
void constructBlinkAddress(BlinkAddress *const o) {
    o->blinkColumnCounter = 0;
    o->blinkRowCounter = 0;
    o->blinkAddressBlinkDelay = 0;
    o->lastExecutionTime = 0;
    o->blinkAddressState = ADDRESS_BLINK_STATES_START;
}

/**
* constructor function
* @param o the object to construct
**/
void constructBlinkTimeInterval(BlinkTimeInterval *const o) {
    o->lastExecutionTime = 0;
    o->localTimeMultiplier = TIME_INTERVAL_BLINK_STATES_PERIOD_MULTIPLIER;
    o->blinkState = TIME_INTERVAL_BLINK_STATES_LED_OFF;
}

/**
* constructor function
* @param o the object to construct
**/

void constructPeriphery(Periphery *const o) {
    o->loopCount = 0;
    constructBlinkAddress(&o->blinkAddress);
    constructBlinkTimeInterval(&o->blinkTimeInterval);
//    // validation code for measuring forward latency
//    o->isTxSouthToggleEnabled = false;
}

