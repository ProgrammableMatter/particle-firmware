/*
 * Indicator.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: Raoul Rubien
 */

#include "Indicator.h"

#include <avr/io.h>
#include <common/common.h>
#include <common/PortADefinition.h>
#include <common/CommPortDefinition.h>

#define LED_PIN 					Pin3
#define SIGNAL_PIN 					Pin7
#define INDICATOR_PORT 				AOut
#define INDICATOR_PORT_DIRECTION 	ADir

#define LED_INIT INDICATOR_PORT_DIRECTION setOut LED_PIN
#define LED_OFF INDICATOR_PORT setLo LED_PIN
#define LED_ON INDICATOR_PORT setHi LED_PIN
#define LED_VALUE INDICATOR_PORT getBit LED_PIN

#define SIGNAL_INIT INDICATOR_PORT_DIRECTION setOut SIGNAL_PIN
#define SIGNAL_LO INDICATOR_PORT setLo SIGNAL_PIN
#define SIGNAL_HI INDICATOR_PORT setHi SIGNAL_PIN
#define SIGNAL_VALUE INDICATOR_PORT getBit SIGNAL_PIN

struct Indicator indicatorState;

void Indicator(void) {
	indicatorState.state = INDICATOR_IDLE;
	indicatorState.counter = 0;
	indicatorState.increment = 1;
	indicatorState.level = 70;

	LED_INIT;
	LED_OFF;

	SIGNAL_INIT;
	SIGNAL_LO;
}

void Indicator_enterSignal(Indicator_Signal signal) {
	indicatorState.state = signal;
}

void Indicator_tick(void) {
     
	if (indicatorState.counter >= UINT8_MAX) {
		indicatorState.increment = -1;
	} else if (indicatorState.counter <= 0) {
		indicatorState.increment = 1;
	}

	indicatorState.counter += indicatorState.increment;

	if (indicatorState.level < indicatorState.counter) {
		LED_OFF;
	} else {
		LED_ON;
	}

    if (SIGNAL_VALUE) {
        SIGNAL_LO;
    }
    else {
        SIGNAL_HI;
    }
}

uint8_t Indicator_getLevel(void) {
	return indicatorState.level;
}

void Indicator_setLevel(uint8_t level) {
	indicatorState.level = level;
}
