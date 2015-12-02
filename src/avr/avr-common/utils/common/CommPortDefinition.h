/**
 * @author Raoul Rubien 2015
 */

// TODO deprecated
#ifndef PORT_DEFINITION_H
#define PORT_DEFINITION_H

#include <avr/io.h>
#include "PortADefinition.h"

#define TX_RX_PORT 		PORTA
#define TX_RX_DIRECTION DDRA

/**
 * port A tx/rx pins
 */

#define TXA_PIN 		4
#define RXA_PIN 		5
#define RXA_POWER_PIN 	6

#define TXA 			PORTA4
#define RXA 			PINA5
#define RXA_POWER 		PORTA6

/**
 * port B tx/rx pins
 */
#define TXB_PIN 		1
#define RXB_PIN 		2
#define RXB_POWER_PIN	0

#define TXB    			PORTA1
#define RXB    			PINA2
#define RXB_POWER 		PORTA0

#endif
