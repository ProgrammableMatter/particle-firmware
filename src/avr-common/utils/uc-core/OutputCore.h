#ifndef __output_core__
#define __output_core__

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
	void enterLedState(State st) {
	}

	/**
	 * gives runtime to the core
	 */
	void tick() {
	}
};

#endif
