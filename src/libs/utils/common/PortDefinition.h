#ifndef __PortDefinition__
#define __PortDefinition__

#include <avr/io.h>

// renaming ports and port register

#define AOut PORTA
#define BOut PORTB
#define COut PORTC
#define DOut PORTD

#define AIn PINA
#define BIn PINB
#define CIn PINC
#define DIn PIND

#define ADir DDRA
#define BDir DDRB
#define CDir DDRC
#define DDir DDRD

#endif
