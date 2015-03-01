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
	Particle();

	virtual void writeBits(Channel ch, uint8_t bytes[], uint8_t numBits);
	virtual void readBits(Channel ch, uint8_t buffer[], uint8_t size,
			uint8_t *bits);
	virtual void powerOn(Channel ch, Line line);
	virtual void tick();
};

#endif
