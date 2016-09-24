/**
 * @author Raoul Rubien 20.07.2016
 *
 * Reception pin change interrupt configuration.
 */

#pragma once

#include <avr/interrupt.h>
#include "common/PortInteraction.h"

#ifdef __AVR_ATtiny1634__ // for production MCU

#  define __RX_INTERRUPT_FLAG_NORTH PCIF2
#  define __RX_INTERRUPT_FLAG_EAST PCIF1
#  define __RX_INTERRUPT_FLAG_SOUTH PCIF0

#  define RX_NORTH_INTERRUPT_CLEAR_PENDING \
    (((GIFR & bit(__RX_INTERRUPT_FLAG_NORTH)) != 0) ? GIFR = bit(__RX_INTERRUPT_FLAG_NORTH):0)

#  define RX_EAST_INTERRUPT_CLEAR_PENDING \
    (((GIFR & bit(__RX_INTERRUPT_FLAG_EAST)) != 0) ? GIFR = bit(__RX_INTERRUPT_FLAG_EAST):0)

#  define RX_SOUTH_INTERRUPT_CLEAR_PENDING \
    (((GIFR & bit(__RX_INTERRUPT_FLAG_SOUTH)) != 0) ? GIFR = bit(__RX_INTERRUPT_FLAG_SOUTH):0)

#  define RX_INTERRUPTS_CLEAR_PENDING \
    RX_NORTH_INTERRUPT_CLEAR_PENDING; \
    RX_EAST_INTERRUPT_CLEAR_PENDING; \
    RX_SOUTH_INTERRUPT_CLEAR_PENDING

#  define RX_INT_ENABLE \
    (GIMSK setBit (bit(PCIE0) | bit(PCIE1) | bit(PCIE2)))

#  define RX_NORTH_INTERRUPT_DISABLE \
    (PCMSK2 unsetBit bit(PCINT17))

#  define RX_NORTH_INTERRUPT_ENABLE \
    (PCMSK2 setBit bit(PCINT17))

#  define RX_SOUTH_INTERRUPT_DISABLE \
    (PCMSK0 unsetBit bit(PCINT4))

#  define RX_SOUTH_INTERRUPT_ENABLE \
    (PCMSK0 setBit bit(PCINT4))

#  define RX_EAST_INTERRUPT_DISABLE \
    (PCMSK1 unsetBit bit(PCINT8))

#  define RX_EAST_INTERRUPT_ENABLE \
    (PCMSK1 setBit bit(PCINT8))

#  define RX_INTERRUPTS_ENABLE \
    RX_NORTH_INTERRUPT_ENABLE; \
    RX_SOUTH_INTERRUPT_ENABLE; \
    RX_EAST_INTERRUPT_ENABLE

#  define RX_INTERRUPTS_DISABLE \
    RX_NORTH_INTERRUPT_DISABLE; \
    RX_SOUTH_INTERRUPT_DISABLE; \
    RX_EAST_INTERRUPT_DISABLE

#  define RX_INTERRUPTS_SETUP \
    RX_INTERRUPTS_CLEAR_PENDING; \
    RX_INT_ENABLE

#else
#  if defined(__AVR_ATmega16__) // for simulator MCU

#  define __RX_INTERRUPT_FLAG_NORTH INTF2
#  define __RX_INTERRUPT_FLAG_EAST INTF1
#  define __RX_INTERRUPT_FLAG_SOUTH INTF0

#  define RX_NORTH_INTERRUPT_CLEAR_PENDING \
    (((GIFR & bit(__RX_INTERRUPT_FLAG_NORTH)) != 0) ? GIFR = bit(__RX_INTERRUPT_FLAG_NORTH):0)

#  define RX_EAST_INTERRUPT_CLEAR_PENDING \
    (((GIFR & bit(__RX_INTERRUPT_FLAG_EAST)) != 0) ? GIFR = bit(__RX_INTERRUPT_FLAG_EAST):0)

#  define RX_SOUTH_INTERRUPT_CLEAR_PENDING \
    (((GIFR & bit(__RX_INTERRUPT_FLAG_SOUTH)) != 0) ? GIFR = bit(__RX_INTERRUPT_FLAG_SOUTH):0)

#  define RX_INTERRUPTS_CLEAR_PENDING \
    RX_NORTH_INTERRUPT_CLEAR_PENDING; \
    RX_EAST_INTERRUPT_CLEAR_PENDING; \
    RX_SOUTH_INTERRUPT_CLEAR_PENDING
//ISC2 - 0: on falling edge, 1: on rising edge
//(ISCx1, ISCx0) - (0,1): on logic change
#    define __RX_INTERRUPTS_SENSE_CONTROL_SETUP \
    MCUCR  setBit (0 << ISC11) | (1 << ISC10) | (0 << ISC01) | (1 << ISC00); \
    MCUCSR setBit (1 << ISC2)

#    define RX_NORTH_INTERRUPT_DISABLE GICR unsetBit bit(INT2);
#    define RX_NORTH_INTERRUPT_ENABLE GICR setBit bit(INT2);

#    define RX_EAST_INTERRUPT_DISABLE GICR unsetBit bit(INT1);
#    define RX_EAST_INTERRUPT_ENABLE GICR setBit bit(INT1);

#    define RX_SOUTH_INTERRUPT_DISABLE GICR unsetBit bit(INT0);
#    define RX_SOUTH_INTERRUPT_ENABLE GICR setBit bit(INT0);

#    define RX_INTERRUPTS_ENABLE GICR setBit ((1 << INT0) | (1 << INT1) | (1 << INT2))
#    define RX_INTERRUPTS_DISABLE GICR unsetBit ((1 << INT0) | (1 << INT1) | (1 << INT2))

#    define RX_INTERRUPTS_SETUP \
    RX_INTERRUPTS_CLEAR_PENDING; \
    __RX_INTERRUPTS_SENSE_CONTROL_SETUP

#  else
#    error
#  endif
#endif

