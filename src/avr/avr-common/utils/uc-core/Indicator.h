/*
 * Indicator.h
 *
 *  Created on: Mar 1, 2015
 *      Author: Raoul Rubien
 */

#ifndef __indicator_h__
#define __indicator_h__

#include <inttypes.h>

typedef enum {
	INDICATOR_ERROR, INDICATOR_WORKING, INDICATOR_IDLE
} Indicator_Signal;

struct Indicator {
	int8_t increment;
	uint8_t counter;
	uint8_t level;
	Indicator_Signal state;
};


/**
 * initcator initializer
 */
void Indicator(void);

/**
 * changes the indicator led state
 * @param st the mode to be entered
 */
void Indicator_enterSignal(Indicator_Signal signal);

/**
 * returns the current indicator duty cycle level
 */
uint8_t Indicator_getLevel(void);

/**
 * set a new indicator duty cycle level
 */
void Indicator_setLevel(uint8_t level);

/**
 * gives runtime to the core
 */
void Indicator_tick(void);

#endif
