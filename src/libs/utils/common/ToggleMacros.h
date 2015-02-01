#ifndef _Toggle_h_
#define _Toggle_h_

#include <avr/io.h>

//@{
/**
 * Generates toggle function to enable toggle on a specific PORTxn.
 * Not realized by setting PINxn since it deaktivates the pull up for the whole port.
 * The created funtion signature looks like:
 * toggle'port''pin'(void) for ex.
 * toggleB0(void);
 * @param port the port name
 * @param the pin number (0, 1, ..., 7)
 */

#define TOGGLEINVARIANT(port, pin) {  \
    if ( PORT##port & _BV(PIN##pin) ) \
        PORT##port &= ~_BV(PIN##pin); \
    else                              \
        PORT##port |= _BV(PIN##pin);  \
}

#define TOGGLEINIT(port, pin) { \
    PORT##port &= ~_BV(pin);    \
    DDR##port |= _BV(pin);      \
}

//@}

#endif
