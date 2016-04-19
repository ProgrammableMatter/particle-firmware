/**
 * @author Raoul Rubien 2016
 */

#ifndef __SIMULATION_MACROS_H
#define __SIMULATION_MACROS_H

#  ifdef __AVR_ATmega16__

#    include <avr/io.h>

#    define SIMULATION
#    define IS_SIMULATION true
#    define IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE ParticleAttributes.node.state = STATE_TYPE_ERRONEOUS
#    define IF_SIMULATION_CHAR_OUT(value) UDR=(value)
#    define IF_SIMULATION_INT16_OUT(value) \
        EEARL=((value) & 0xff00) >> 8; \
        EEDR=((value) & 0x00ff)

/**
 * Write a string to UDR register that is monitored in simulation.
 */
inline void writeToUart(const char *string) {
    if (string != 0) {
        while (*string != 0) {
#    ifdef UDR
            UDR = *string;
        }
        UDR = '\n';
#    else
        UDR0 = *string;
    }
    UDR0 = '\n';
#    endif
    }
}

#  else
#    define IS_SIMULATION false
#    define IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE
#    define IF_SIMULATION_CHAR_OUT(value)
#    define IF_SIMULATION_INT16_OUT(value)
inline void writeToUart(const char *string) { }
#  endif

#endif

