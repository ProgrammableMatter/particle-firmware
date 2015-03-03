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

#define LED_INIT INDICATOR_PORT_DIRECTION setOut LED_PIN;
#define LED_OFF INDICATOR_PORT setLo LED_PIN
#define LED_ON INDICATOR_PORT setHi LED_PIN

#define SIGNAL_INIT INDICATOR_PORT_DIRECTION setOut SIGNAL_PIN
#define SIGNAL_LO INDICATOR_PORT setLo SIGNAL_PIN
#define SIGNAL_HI INDICATOR_PORT setHi SIGNAL_PIN

Indicator::Indicator() :
		time(0), level(30000) {
	LED_INIT
	;
	LED_ON;

	SIGNAL_INIT;
	SIGNAL_LO;
}

void Indicator::enterLedState(State st) {
}

void Indicator::tick() {
	if (level <= time++) {
		LED_OFF;
	} else {
		LED_ON;
	}
}
