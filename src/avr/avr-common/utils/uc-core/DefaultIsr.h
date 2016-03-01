/**
 * @author Raoul Rubien 2015
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Globals.h"
#include "ParticleIoDefinitions.h"
#include "ParticleInterruptDefinitions.h"

extern volatile ParticleState GlobalState;

#define TIMER0_ON_INTERRUPT_SHIFT_BACK 2


#ifdef __AVR_ATtiny20__
/**
 * Since ATtiny20 has no flag signaling which pin contributed to the PCINT0_vect it is needed to track the pin
 * changes manuallay to determine that cases.
 */
static unsigned char PinStateBeforeInterrupt = 0;
#  define __NORTH_RX_FLAG                0b00000001
#  define __SOUTH_RX_FLAG                0b00000010
#  define __IS_RX_INITIALIZED_FLAG       0b00000100

#  define IS_NORTH_RX_FLAG_SET (0 != (PinStateBeforeInterrupt getBit __NORTH_RX_FLAG))
#  define IS_SOUTH_RX_FLAG_SET (0 != (PinStateBeforeInterrupt getBit __SOUTH_RX_FLAG))
#  define SET_NORTH_RX_FLAG PinStateBeforeInterrupt setBit __NORTH_RX_FLAG
#  define UNSET_NORTH_RX_FLAG PinStateBeforeInterrupt unsetBit __NORTH_RX_FLAG
#  define SET_SOUTH_RX_FLAG PinStateBeforeInterrupt setBit __SOUTH_RX_FLAG
#  define UNSET_SOUTH_RX_FLAG PinStateBeforeInterrupt unsetBit __SOUTH_RX_FLAG
#  define IS_RX_FLAG_INITIALIZED (0 != (PinStateBeforeInterrupt getBit __IS_RX_INITIALIZED_FLAG))
#  define SET_RX_FLAG_INITIALIZED PinStateBeforeInterrupt setBit __IS_RX_INITIALIZED_FLAG

/**
 * pin on port A change interrupt request
 */
ISR(PCINT0_vect) { // attiny 20
    DISABLE_TIMER0_PRESCALER;

    // filter north rx events on falling edge
    if (IS_RX_FLAG_INITIALIZED) {
        // record nortx rx events
        // on falling edge: if previous tracked edge is high and current is low
        if (IS_NORTH_RX_FLAG_SET && NORTH_RX_IS_LO) {
            GlobalState.northRxEvents++;
            UNSET_NORTH_RX_FLAG;
        } // update rising edge
        else if (NORTH_RX_IS_HI) {
            SET_NORTH_RX_FLAG;
        }
        // record south rx events
        // on falling edge: if previous tracked edge is high and current is low
        if (IS_SOUTH_RX_FLAG_SET && SOUTH_RX_IS_LO) {
            GlobalState.southRxEvents++;
            UNSET_SOUTH_RX_FLAG;
        } // update rising edge
        else if (SOUTH_RX_IS_HI) {
            SET_SOUTH_RX_FLAG;
        }

    }
        // consume one edge of south or north rx to determine the next "last" flank states
    else {
        if (NORTH_RX_IS_LO) {
            UNSET_NORTH_RX_FLAG;
        } else {
            SET_NORTH_RX_FLAG;
        }

        if (SOUTH_RX_IS_LO) {
            UNSET_SOUTH_RX_FLAG;
        } else {
            SET_SOUTH_RX_FLAG;
        }
        SET_RX_FLAG_INITIALIZED;
    }

    // Shift counter by the approximate clock ticks it takes from ENABLE_TIMER0_PRESCALE until sei instruction
    // back.
    TCNT0 = TCNT0 - TIMER0_ON_INTERRUPT_SHIFT_BACK;

    ENABLE_TIMER0_PRESCALER;
}

#elif __AVR_ATmega16__
/**
 * pin change interrupt 0 - INT0
 * This interrupt routine is triggered on falling edges of the north reception pin. In other words on received
 * falling edges from the north neighbour.
 */
ISR(INT0_vect) { // atmega 16
    DISABLE_TIMER0_PRESCALER;
//    SREG unsetBit bit(SREG_I);
    if (GlobalState.northRxEvents == 255) {
        asm("BREAK");
    }
    GlobalState.northRxEvents++;
    // Shift counter by the approximate clock ticks it takes from ENABLE_TIMER0_PRESCALE until sei instruction
    // back.
    TCNT0 = TCNT0 - TIMER0_ON_INTERRUPT_SHIFT_BACK;
//    GIFR = bit(INTF0);
    ENABLE_TIMER0_PRESCALER;
}

/**
 * This interrupt routine is triggered on falling edges from the south reception pin. In other words on received
 * falling edged from the south neighbour.
 */
ISR(INT1_vect) { // atmega 16
    DISABLE_TIMER0_PRESCALER;
//    SREG unsetBit bit(SREG_I);
    if (GlobalState.southRxEvents == 255) {
        asm("BREAK");
    }
    GlobalState.southRxEvents++;
    // Shift counter by the approximate clock ticks it takes from ENABLE_TIMER0_PRESCALE until sei instruction
    // back.
    TCNT0 = TCNT0 - TIMER0_ON_INTERRUPT_SHIFT_BACK;
//    GIFR = bit(INTF1);
    ENABLE_TIMER0_PRESCALER;
}

#endif

// timer/counter0 match handling for attiny20/atmega16

#ifdef __AVR_ATtiny20__
/**
 * timer/counter 0 match on compare with register A - TCNT0 compare with OCR0A
 * This interrupt routine is triggered when the counter equals to OCR0A value.
 */

ISR(TIM0_COMPA_vect) {
    DISABLE_TIMER0_PRESCALER;
    TCNT0 = 0;

    NORTH_TX_TOGGLE;
    SOUTH_TX_TOGGLE;

    //    TIFR setBit (bit(OCF0B) | bit (OCF0A) | bit(TOV0));
    ENABLE_TIMER0_PRESCALER;
}


#elif __AVR_ATmega16__
/**
 * timer/counter 0 compare - TIMER0 compare with OCR0
 * This interrupt routine is triggered when the counter equals to OCR0 value.
 */
ISR(TIMER0_COMP_vect) {
    DISABLE_TIMER0_PRESCALER;
//    SREG unsetBit bit(SREG_I);
    TCNT0 = 0;

    NORTH_TX_TOGGLE;
    SOUTH_TX_TOGGLE;

//    TIFR = bit(OCF0);
    ENABLE_TIMER0_PRESCALER;
}
#endif

// other unhandled interrupts should execute asm("BREAK") to be notified in avrora's particle-monitor
#ifdef __AVR_ATmega16__

/**
 * Timer/Counter0 Overflow
 */
ISR(TIMER0_OVF_vect) {
//    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Input Capture
 */
ISR(TIMER1_CAPT_vect) {
//    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Compare Match A
 */
ISR(TIMER1_COMPA_vect) {
//    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Compare Match B
 */
ISR(TIMER1_COMPB_vect) {
//    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Overflow
 */
ISR(TIMER1_OVF_vect) {
//    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * External Pin, Power-on Reset, Brown-Out Reset, Watchdog Reset
 */
ISR(_VECTOR(0)) {
//    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

#endif