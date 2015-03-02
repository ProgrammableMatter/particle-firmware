/*
 * InterruptFlags.h
 *
 *  Created on: Mar 2, 2015
 *      Author: rrubien
 */

#ifndef __interrupt_flags_h__
#define __interrupt_flags_h__

#include <common/PortInteraction.h>
class InterruptFlags {
public:
	enum Flag {
		NONE = 0x00, COMPARE0A = 0x01, RXA = 0x02, RXB = 0x04
	};

	InterruptFlags() :
			flags(NONE) {
	}

	void clearFlag(Flag flag) {
		flags = (Flag) ((int8_t) flags | ~bit((int8_t) flag));
	}
	Flag flags;
};

#endif
