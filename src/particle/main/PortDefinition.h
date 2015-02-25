// @author: Raoul Rubien 2011

#ifndef __PortDefinition__
#define __PortDefinition__

#include <avr/io.h>

/**
 * more convenient port names
 */

#define AOut PORTA
#define BOut PORTB

#define AIn PINA
#define BIn PINB

#define ADir DDRA
#define BDir DDRB

/**
 * port A tx/rx pins
 */
#define TXA PORTA4
#define RXA_ON PORTA6
#define RXA PINA5

/**
 * port B tx/rx pins
 */
#define TXB    PORTA1
#define RXB_ON PORTA0
#define RXB    PINA2

#endif
