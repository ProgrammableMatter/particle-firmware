/*
 * Indicator.h
 *
 *  Created on: Mar 1, 2015
 *      Author: Raoul Rubien
 */

#ifndef __indicator_h__
#define __indicator_h__

#include "OutputCore.h"

class Indicator: public OutputCore {
public:
	Indicator();
	void enterLedState(State st);
	void tick();

private:
	uint16_t time;
	uint16_t level;
};

#endif
