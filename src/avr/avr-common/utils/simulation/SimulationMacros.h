/**
 * @author Raoul Rubien 2016
 */

#ifndef __SIMULATION_MACROS_H__
#define __SIMULATION_MACROS_H__

#  ifdef SIMULATION

#    include <avr/io.h>

#    define IS_SIMULATION true
#    define IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE \
    SREG unsetBit bit(SREG_I); \
    ParticleAttributes.node.state = STATE_TYPE_ERRONEOUS
#    define DEBUG_CHAR_OUT(value) UDR=(value)
#    define DEBUG_INT16_OUT(value) \
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

extern inline void writeToUart(const char *);

#  else
#    define IS_SIMULATION false
#    define IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE
#    define DEBUG_CHAR_OUT(value)
#    define DEBUG_INT16_OUT(value)

#define UNUSED(x) (void)(x)
inline void writeToUart(const char *string) { UNUSED(string); }
#undef UNUSED
extern inline void writeToUart(const char *string);
#  endif

#endif

