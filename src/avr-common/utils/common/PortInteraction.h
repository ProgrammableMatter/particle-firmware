/**
 * @author Raoul Rubien 2011
 */

#pragma once

/**
 * pin definition
 */
#define Pin0 1
#define Pin1 2
#define Pin2 4
#define Pin3 8
#define Pin4 16
#define Pin5 32
#define Pin6 64
#define Pin7 128

/**
 * better readable expressions for manipulating port direction
 */
#define setIn  &= ~
#define setOut |=
#define setHi  |=
#define pullUp |=
#define setLo  &= ~

/**
 * some bit operators expressed nicer
 * <p>
 * int foo setBit bit(2);
 * <p>
 * AOut unsetBit Pin0;
 */
#define getBit    &
#define setBit    |=
#define unsetBit  &= ~
#define toggleBit ^=
#define bit       _BV
