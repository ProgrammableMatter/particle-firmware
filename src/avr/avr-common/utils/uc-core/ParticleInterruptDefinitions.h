/**
 * @author Raoul Rubien
 * 23.11.15.
 */
#ifndef PROJECT_PARTICLEINTERRUPTDEFINITIONS_H
#define PROJECT_PARTICLEINTERRUPTDEFINITIONS_H
/**
 * @author Raoul Rubien 2015
 */

#include <avr/interrupt.h>
#include <common/PortInteraction.h>

/**
 * define reception pin interrupt macros
 */
// for production MCU
#ifdef __AVR_ATtiny20__
// clear pending interrupt flags
#  define __SETUP_CLEAR_PENDING_INT0_INT1_INT  GIFR = bit(PCIF0) | bit(PCIF1) | bit(INTF0)
// ISC01=1 and ISC0=0 => falling edge on PCINT0
#  define __SETUP_SENSE_CONTROL MCUCR setBit bit(ISC01)
// unset per default: MCUCR unsetBit bit(ISC00);
// enable for pin 2 and 5
#  define ENABLE_RX_INTERRUPTS PCMSK0 setBit (bit(PCINT2) | bit(PCINT5))
// enable for pin 2 and 5
#  define DISABLE_RX_INTERRUPTS PCMSK0 setBit (bit(PCINT2) | bit(PCINT5))
// enable pin interrupt on port a
#  define __SETUP_ENABLE_PORTA_ON_PIN_CHANGE_INT GIMSK setBit bit(PCIE0)

#  define SETUP_RX_INTERRUPTS __SETUP_CLEAR_PENDING_INT0_INT1_INT; __SETUP_SENSE_CONTROL; \
    __SETUP_ENABLE_PORTA_ON_PIN_CHANGE_INT

#  define DISABLE_RX_NORTH_INTERRUPT PCMSK0 unsetBit bit(PCINT2)
#  define ENABLE_RX_NORTH_INTERRUPT PCMSK0 setBit bit(PCINT2)
#  define DISABLE_RX_SOUTH_INTERRUPT PCMSK0 unsetBit bit(PCINT5)
#  define ENABLE_RX_SOUTH_INTERRUPT PCMSK0 setBit bit(PCINT5)

// for simulator MCU
#elif __AVR_ATmega16__
# define __SETUP_CLEAR_PENDING_INT0_INT1_INT GIFR = bit(INTF0) | bit(INTF1) | bit(INTF2)
// ISCx1=1 and ISCx0=0 => falling edge on PCINT0
#  define __SETUP_SENSE_CONTROL MCUCR setBit(bit(ISC01) | bit(ISC11));
// unset per default; MCUCR unsetBit(bit(ISC00) | bit(ISC10));
// enable interrupts on PD2 (INT0) and PD3 (INT1)
#  define ENABLE_RX_INTERRUPTS GICR setBit(bit(INT0) | bit(INT1));
#  define DISABLE_RX_INTERRUPTS GICR unsetBit(bit(INT0) | bit(INT1));

#  define SETUP_RX_INTERRUPTS __SETUP_CLEAR_PENDING_INT0_INT1_INT; __SETUP_SENSE_CONTROL

#  define DISABLE_RX_NORTH_INTERRUPT GICR unsetBit bit(INT0);
#  define ENABLE_RX_NORTH_INTERRUPT GICR setBit bit(INT0);
#  define DISABLE_RX_SOUTH_INTERRUPT GICR setBit bit(INT1);
#  define ENABLE_RX_SOUTH_INTERRUPT GICR unsetBit bit(INT1);

#else
#  error
#endif



/**
 * define timer / counter 0 interrup macros
 */
#define __DEFAULT_NEIGHBOUR_SENSING_PRESCALER_COMPARE 0x10
#ifdef __AVR_ATtiny20__
// clear pending interrupts
#  define __SETUP_CLEAR_PENDING_TIMER0_INT TIFR = (bit(OCF0A) | bit(TOV0))
// prescaler reset
#  define __SETUP_PRESCALER_RESET GTCCR setBit bit(PSR)
// set prescaler for timer/counter 0
// CS0[2:0]: 000 - stopped, 001 - 1/1, 010 - 1/8, 011 - 1/64, 100 - 1/156, 101 - 1/1024
#  define __DISABLE_PRESCALER TCCR0B unsetBit(bit(CS02) | bit(CS01) | bit(CS00))
#  define __SETUP_PRESCALER_1 TCCR0B setBit bit(CS00)
#  define __SETUP_PRESCALER_2 TCCR0B setBit bit(CS01)
#  define __SETUP_PRESCALER_3 TCCR0B setBit (bit(CS01) | bit(CS00))
#  define __SETUP_PRESCALER_4 TCCR0B setBit bit(CS02)
#  define __SETUP_PRESCALER_5 TCCR0B setBit (bit(CS02) | bit(CS01))


// counter output mode
// COM0A[1:0]: 00 - output disconnected, 01 - toggle OC0, 10 - clear OC0, 11 - set OC0
// set per default: TCCR0A unsetBit (bit(COM01) | bit(COM00));

// set timer/counter0 compare value
#  define __SETUP_TIMER0_NEIGHBOUR_SENSE_COMPARE OCR0A = __DEFAULT_NEIGHBOUR_SENSING_PRESCALER_COMPARE

// enable/disable timer/coutner 0 interrupt
#  define ENABLE_TIMER0_INTERRUPT TIMSK setBit bit(OCIE0A)
#  define DISABLE_TIMER0_INTERRUPT TIMSK unsetBit bit(OCIE0A)

#elif __AVR_ATmega16__
// clear pending interrupts
#define __SETUP_CLEAR_PENDING_TIMER0_INT TIFR = (bit(OCF0) | bit(TOV0))
// prescaler reset
#  define __SETUP_PRESCALER_RESET SFIOR setBit bit(PSR10)
// set prescaler for timer/counter 0
// CS0[2:0]: 000 - stopped, 001 - 1/1, 010 - 1/8, 011 - 1/64, 100 - 1/156, 101 - 1/1024
#  define __DISABLE_PRESCALER TCCR0 unsetBit(bit(CS02) | bit(CS01) | bit(CS00))
#  define __SETUP_PRESCALER_1 TCCR0 setBit bit(CS00)
#  define __SETUP_PRESCALER_2 TCCR0 setBit bit(CS01)
#  define __SETUP_PRESCALER_3 TCCR0 setBit (bit(CS01) | bit(CS00))
#  define __SETUP_PRESCALER_4 TCCR0 setBit bit(CS02)
#  define __SETUP_PRESCALER_5 TCCR0 setBit (bit(CS02) | bit(CS01))


// counter output mode
// COM0[1:0]: 00 - output disconnected, 01 - toggle OC0, 10 - clear OC0, 11 - set OC0
// set per default: TCCR0 unsetBit (bit(COM01) | bit(COM00));

// set timer/counter0 compare value
#  define __SETUP_TIMER0_NEIGHBOUR_SENSE_COMPARE OCR0 = __DEFAULT_NEIGHBOUR_SENSING_PRESCALER_COMPARE

// enable/disable timer/coutner 0 interrupt
#  define ENABLE_TIMER0_INTERRUPT TIMSK setBit bit(OCIE0)
#  define DISABLE_TIMER0_INTERRUPT TIMSK unsetBit bit(OCIE0)

#else
#  error
#endif

#  define __ENABLE_DEFAULT_NEIGHBOUR_SENSING_PRESCALER __SETUP_PRESCALER_2

// detach prescaler: pause timer counting
#  define DISABLE_TIMER0_PRESCALER __DISABLE_PRESCALER
// attach prescaler: enable timer counting
#  define ENABLE_TIMER0_PRESCALER __ENABLE_DEFAULT_NEIGHBOUR_SENSING_PRESCALER

// setup timer/compare 0 interrupt for neighbour sensing
#  define SETUP_TIMER0_NEIGHBOUR_SENSE_INTERRUPT __SETUP_CLEAR_PENDING_TIMER0_INT; __SETUP_PRESCALER_RESET; \
    __ENABLE_DEFAULT_NEIGHBOUR_SENSING_PRESCALER; __SETUP_TIMER0_NEIGHBOUR_SENSE_COMPARE

#endif
