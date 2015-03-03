// @author: Raoul Rubien 2015

#ifndef __port_a_efinition__
#define __port_a_efinition__

#include <avr/io.h>

#ifdef __AVR_ATtiny20__
#define AOut PORTA
#define AIn PINA
#define ADir DDRA
#else
// mock other free atmega8 port for debugging
#define AOut PORTC
#define AIn PINC
#define ADir DDRC
#endif

#endif
