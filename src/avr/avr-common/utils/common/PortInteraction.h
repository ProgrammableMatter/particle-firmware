/**
 * @author: Raoul Rubien 2011
 */

#ifndef __PortInteraction__
#define __PortInteraction__

/**
 * pin definition
 */
#define Pin0 0b00000001
#define Pin1 0b00000010
#define Pin2 0b00000100
#define Pin3 0b00001000
#define Pin4 0b00010000
#define Pin5 0b00100000
#define Pin6 0b01000000
#define Pin7 0b10000000

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
#define getBit   &
#define setBit   |=
#define unsetBit &= ~
#define toggleBit ^=
#define bit      _BV

#endif
