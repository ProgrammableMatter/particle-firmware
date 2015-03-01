#ifndef __particle_core__
#define __particle_core__

#include <inttypes.h>

class OutputCore {

public:

	enum State {
		ERROR, WORKING, IDLE
	};

	/**
	 * changes the indicator led state
	 * @param st the mode to be entered
	 */
	virtual void enterLedState(State st) = 0;

	/**
	 * gives runtime to the core
	 */
	virtual void tick() = 0;
};

#endif
