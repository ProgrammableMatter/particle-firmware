#ifndef _Trigger_h_
#define _Trigger_h_

#include <avr/io.h>
#include <util/delay.h>

//@{
/**
 * Macros to enable waiting for a trigger (pull down an iput) on a specified port-pin.
 */

/**
 * Initialisation function macro to enable waiting on input.
 * @param port the port name (A, B, C, ...)
 * @param pinIn the pin to be listened for (0, 1, ..., 7)
 * @param pinOut the signalizing output pin (0, 1, ..., 7)
 *
 */
#define TRIGGERINIT(port, pinIn, pinOut) { \
    DDR##port  &= ~_BV(PIN##pinIn);        \
    PORT##port |= _BV(PIN##pinIn); /* pull up enabled */ \
    PORT##port &= ~_BV(PIN##pinOut) /* clear out pin */;       \
    DDR##port  |= _BV(PIN##pinOut);        \
}


/**
 * Generates trigger function to enable waiting on input.
 * The generated function signature looks like:
 * void trigger'port''pinIn''pinOut'(void) for ex.
 * void triggerB01(void)
 * @param port the port name (A, B, C, ...)
 * @param pinIn the pin to be listened for (0, 1, ..., 7)
 * @param pinOut the signalizing output pin (0, 1, ..., 7)
 *
 */
#define MKTRIGGERFX(port, pinIn, pinOut) \
void waitForTrigger##port##pinIn##pinOut(void) { \
    PORT##port |= _BV(PIN##pinOut);              \
    while ( PIN##port & _BV(PIN##pinIn) );       \
    _delay_ms(1);                                \
    PORT##port &= ~_BV(PIN##pinOut);             \
    while ( !(PIN##port & _BV(PIN##pinIn)) );    \
    _delay_ms(1);                                \
}

//@}

#endif
