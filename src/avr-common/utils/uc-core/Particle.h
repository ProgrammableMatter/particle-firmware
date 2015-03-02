/*
 * Particle.h
 *
 *  Created on: Mar 1, 2015
 *      Author: Raoul Rubien
 */

#ifndef __particle_h__
#define __particle_h__

#include "ParticleCore.h"

class Particle: public ParticleCore {
public:

	void writeBits(Channel ch, uint8_t bytes[], uint8_t numBits);
	void readBits(Channel ch, uint8_t buffer[], uint8_t size,
			uint8_t *bits);
	void powerOn(Channel ch, Line line);
	void tick();
};

#endif
