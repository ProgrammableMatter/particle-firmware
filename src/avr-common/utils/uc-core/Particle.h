/*
 * Particle.h
 *
 *  Created on: Mar 1, 2015
 *      Author: Raoul Rubien
 */

#ifndef __particle_h__
#define __particle_h__

#include <inttypes.h>
typedef enum {
	A, B
} Channel;
typedef enum {
	TX, RX
} Line;

/**
 * initializes particle core
 */
void Particle(void);

/**
 * Sends numBits from buffer as the are on channel A.
 * @param ch the channel to operate on
 * @param bytes buffer containing bytes to be sent
 * @param numBits amount of bits to be read from buffer
 */
void Particle_writeBits(Channel ch, uint8_t bytes[], uint8_t numBits);

/**
 * Reads bits received so far from channel A.
 * @param ch the channel to operate on
 * @param buffer where to store bits
 * @param size size of buffer
 * @param bits bits written to buffer
 */
void Particle_readBits(Channel ch, uint8_t buffer[], uint8_t size, uint8_t *bits);

/**
 * Powers the channel's line according to the connected MOSFET (pull up or down).
 * @param ch the channel to operate on
 * @param line the line to power
 */
void Particle_powerOn(Channel ch, Line line);

/**
 * gives runtime to implementation
 */
void Particle_tick(void);

#endif
