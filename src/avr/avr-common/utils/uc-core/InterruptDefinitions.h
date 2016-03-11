/**
 * @author Raoul Rubien
 * 23.11.15.
 */
#ifndef PARTICLE_INTERRUPT_DEFINITIONS_H
#define PARTICLE_INTERRUPT_DEFINITIONS_H

#include <avr/interrupt.h>
#include <common/PortInteraction.h>

/////////////////////// define reception pin interrupt macros ///////////////////////

// for production MCU
#ifdef __AVR_ATtiny1634__
#  define RX_INTERRUPTS_CLEAR_PENDING GIFR = bit(PCIF0) | bit(PCIF1) | bit(PCIF2) | bit(INTF0)
#  define RX_INT_ENABLE GIMSK setBit (bit(PCIE0) | bit(PCIE1) | bit(PCIE2))

#  define RX_NORTH_INTERRUPT_DISABLE PCMSK2 unsetBit bit(PCINT17)
#  define RX_NORTH_INTERRUPT_ENABLE PCMSK2 setBit bit(PCINT17)
#  define RX_SOUTH_INTERRUPT_DISABLE PCMSK0 setBit bit(PCINT4)
#  define RX_SOUTH_INTERRUPT_ENABLE PCMSK0 setBit bit(PCINT4)
#  define RX_EAST_INTERRUPT_DISABLE PCMSK1 unsetBit bit(PCINT8)
#  define RX_EAST_INTERRUPT_ENABLE PCMSK1 setBit bit(PCINT8)

#  define RX_INTERRUPTS_ENABLE RX_NORTH_INTERRUPT_ENABLE; RX_SOUTH_INTERRUPT_ENABLE; RX_EAST_INTERRUPT_ENABLE
#  define RX_INTERRUPTS_DISABLE RX_NORTH_INTERRUPT_DISABLE; RX_SOUTH_INTERRUPT_DISABLE; RX_EAST_INTERRUPT_DISABLE

#  define RX_INTERRUPTS_SETUP RX_INTERRUPTS_CLEAR_PENDING; RX_INT_ENABLE

#else
// for simulator MCU
#  ifdef __AVR_ATmega16__
#    define RX_INTERRUPTS_CLEAR_PENDING GIFR = bit(INTF0) | bit(INTF1) | bit(INTF2)

//ISC2 - 0: on falling edge, 1: on rising edge
//(ISCx1, ISCx0) - (0,1): on logic change
#    define __RX_INTERRUPTS_SENSE_CONTROL_SETUP MCUCR  setBit (0 << ISC11) | (1 << ISC10) | (0 << ISC01) | (1 << ISC00); \
MCUCSR setBit (1 << ISC2)

#    define RX_NORTH_INTERRUPT_DISABLE GICR unsetBit bit(INT0);
#    define RX_NORTH_INTERRUPT_ENABLE GICR setBit bit(INT0);
#    define RX_SOUTH_INTERRUPT_DISABLE GICR setBit bit(INT1);
#    define RX_SOUTH_INTERRUPT_ENABLE GICR unsetBit bit(INT1);
#    define RX_EAST_INTERRUPT_DISABLE GICR setBit bit(INT2);
#    define RX_EAST_INTERRUPT_ENABLE GICR unsetBit bit(INT2);

#    define RX_INTERRUPTS_ENABLE GICR setBit ((1 << INT0) | (1 << INT1) | (1 << INT2))
#    define RX_INTERRUPTS_DISABLE GICR unsetBit ((1 << INT0) | (1 << INT1) | (1 << INT2))

#    define RX_INTERRUPTS_SETUP RX_INTERRUPTS_CLEAR_PENDING; __RX_INTERRUPTS_SENSE_CONTROL_SETUP

#  else
#    error
#  endif
#endif

///////////////////////  define timer / counter 0 interrupt macros ///////////////////////

# if  defined(__AVR_ATtiny1634__) || defined(__AVR_ATmega16__)

#  define __PRESCALER_DISCONNECTED (0 << CS02) | (0 << CS01) | (0 << CS00)
#  define __PRESCALER_1 (0 << CS02) | (0 << CS01) | (1 << CS00)
#  define __PRESCALER_8 (0 << CS02) | (1 << CS01) | (0 << CS00)
#  define __PRESCALER_64 (0 << CS02) | (1 << CS01) | (1 << CS00)
#  define __PRESCALER_256 (1 << CS02) | (0 << CS01) | (0 << CS00)
#  define __PRESCALER_1024 (1 << CS02) | (0 << CS01) | (1 << CS00)
#  define __TIMER0_INTERRUPT_CLEAR_PENDING TIFR = ((1 << TOV1) | (1 << OCF1B) | (1 << OCF1A))
//#  define __TIMER0_INTERRUPT_PRESCALER_RESET GTCCR setBit bit(PSR)
// counter output mode - COM1A/B[1:0]: 00 => output disconnected
#  define __TIMER0_INTERRUPT_OUTPUT_MODE_SETUP TCCR1A unsetBit \
    ((1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0))

#  define __TIMER0_INTERRUPT_WAVE_GENERATION_MODE_SETUP TCCR1A unsetBit ((1 << WGM11) | (1 << WGM10)); \
    TCCR1B unsetBit ((1 << WGM13) | (1 << WGM12));
#  define __TIMER0_INTERRUPT_COMPARE_VALUE_SETUP(compareValue) OCR1A = compareValue

#  define __TIMER0_INTERRUPT_PRESCALER_ENABLE TCCR1B setBit(__PRESCALER_8)
#  define __TIMER0_INTERRUPT_PRESCALER_DISABLE TCCR1B unsetBit((1 << CS02) | (0 << CS01) | (1 << CS00))

#  define __TIMER0_INTERRUPT_ENABLE TIMSK setBit bit(OCIE1A)
#  define __TIMER0_INTERRUPT_DISABLE TIMSK unsetBit bit(OCIE1A)
#else
#  error
#endif

#define __DEFAULT_NEIGHBOUR_SENSING_COMPARE_VALUE 0x10
#define TIMER0_NEIGHBOUR_SENSE_SETUP __TIMER0_INTERRUPT_CLEAR_PENDING; \
    __TIMER0_INTERRUPT_OUTPUT_MODE_SETUP; \
    __TIMER0_INTERRUPT_WAVE_GENERATION_MODE_SETUP; \
    __TIMER0_INTERRUPT_COMPARE_VALUE_SETUP(__DEFAULT_NEIGHBOUR_SENSING_COMPARE_VALUE); \
    __TIMER0_INTERRUPT_PRESCALER_DISABLE; \
    __TIMER0_INTERRUPT_ENABLE

#define TIMER0_NEIGHBOUR_SENSE_DISABLE __TIMER0_INTERRUPT_PRESCALER_DISABLE; __TIMER0_INTERRUPT_DISABLE
#define TIMER0_NEIGHBOUR_SENSE_ENABLE __TIMER0_INTERRUPT_ENABLE; __TIMER0_INTERRUPT_PRESCALER_ENABLE

#endif