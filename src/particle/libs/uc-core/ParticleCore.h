#ifndef __particle_core__
#define __particle_core__

#include <inttypes.h>

class ParticleCore {

public:

	enum Channel {
		A, B
	};
	enum Line {
		TX, RX
	};

	/**
	 * Sends numBits from buffer as the are on channel A.
	 * @param ch the channel to operate on
	 * @param bytes buffer containing bytes to be sent
	 * @param numBits amount of bits to be read from buffer
	 */
	virtual void writeBits(Channel ch, uint8_t bytes[], uint8_t numBits) = 0;

	/**
	 * Reads bits received so far from channel A.
	 * @param ch the channel to operate on
	 * @param buffer where to store bits
	 * @param size size of buffer
	 * @param bits bits written to buffer
	 */
	virtual void readBits(Channel ch, uint8_t buffer[], uint8_t size,
			uint8_t *bits) = 0;

	/**
	 * Powers the channel's line according to the connected MOSFET (pull up or down).
	 * @param ch the channel to operate on
	 * @param line the line to power
	 */
	virtual void powerOn(Channel ch, Line line) = 0;

	/**
	 * gives runtime to implementation
	 */
	virtual void tick() = 0;

};

#endif
