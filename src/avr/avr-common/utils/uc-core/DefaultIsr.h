/**
 * @author Raoul Rubien 2015
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#include <uc-core/Globals.h>
#include <uc-core/ParticleIoDefinitions.h>
#include <uc-core/ParticleInterruptDefinitions.h>

extern volatile ParticleState GlobalState;

#define TIMER0_ON_INTERRUPT_SHIFT_BACK 2


#ifdef __AVR_ATtiny20__
/**
 * Since ATtiny20 has no flag signaling which pin contributed to the PCINT0_vect it is needed to track the pin
 * changes manuallay to determine that cases.
 */
static unsigned char PinStateBeforeInterrupt = 0;
#  define NORTH_RX_FLAG = 0b00000001
#  define SOUTH_RX_FLAG = 0b00000010
#  define IS_NORTH_RX_FLAG PinStateBeforeInterrupt getBit(NORTH_RX_FLAG)
#  define IS_SOUTH_RX_FLAG PinStateBeforeInterrupt getBit(SOUTH_RX_FLAG)
#  define SET_NORTH_RX_FLAG PinStateBeforeInterrupt setBit(NORTH_RX_FLAG)
#  define SET_SOUTH_RX_FLAG PinStateBeforeInterrupt setBit(SOUTH_RX_FLAG)

/**
 * pin on port A change interrupt request
 */
ISR( PCINT0_vect) { // attiny 20
    // TODO @attiny20 there is no flag that indicates the interrupt-pin contrubuting to this interrupt ...
}

#elif __AVR_ATmega16__
/**
 * pin change interrupt 0 - INT0
 * This interrupt routine is triggered on falling edges of the north reception pin. In other words on received
 * falling edges from the north neighbour.
 */
ISR(INT0_vect) { // atmega 16
    DISABLE_TIMER0_PRESCALER;
    SREG unsetBit bit(SREG_I);
    if (GlobalState.northRxEvents == 255) {
        asm("BREAK");
    }
    GlobalState.northRxEvents++;
    // Shift counter by the approximate clock ticks it takes from ENABLE_TIMER0_PRESCALE until sei instruction
    // back.
    TCNT0 = TCNT0 - TIMER0_ON_INTERRUPT_SHIFT_BACK;
    // TODO clearing flag really necessary?
    GIFR = bit(INTF0);
    ENABLE_TIMER0_PRESCALER;
}

/**
 * This interrupt routine is triggered on falling edges from the south reception pin. In other words on received
 * falling edged from the south neighbour.
 */
ISR(INT1_vect) { // atmega 16
    DISABLE_TIMER0_PRESCALER;
    SREG unsetBit bit(SREG_I);
    if (GlobalState.southRxEvents == 255) {
        asm("BREAK");
    }
    GlobalState.southRxEvents++;
    // Shift counter by the approximate clock ticks it takes from ENABLE_TIMER0_PRESCALE until sei instruction
    // back.
    TCNT0 = TCNT0 - TIMER0_ON_INTERRUPT_SHIFT_BACK;
    // TODO clearing flag really necessary?
    GIFR = bit(INTF1);
    ENABLE_TIMER0_PRESCALER;
}

#endif

// timer/counter0 match handling for attiny20/atmega16

#ifdef __AVR_ATtiny20__
/**
 * timer/counter 0 match on compare with register A - TCNT0 compare with OCR0A
 * This interrupt routine is triggered when the counter equals to OCR0A value.
 */
ISR( TIM0_COMPA_vect) {
    // TODO timer0 compare a implementation missing
}


#elif __AVR_ATmega16__
/**
 * timer/counter 0 compare - TIMER0 compare with OCR0
 * This interrupt routine is triggered when the counter equals to OCR0 value.
 */
ISR(TIMER0_COMP_vect) {
    asm("BREAK");
    DISABLE_TIMER0_PRESCALER;
    SREG unsetBit bit(SREG_I);
    TCNT0 = 0;

    NORTH_TX_TOGGLE;
    SOUTH_TX_TOGGLE;
//   TODO instead of toggling 2x in this isr toggle just once but adjust {loop counter, min pulsing events, compare0A}
//    NORTH_TX_TOGGLE;
//    SOUTH_TX_TOGGLE;

    // TODO clearing flag really necessary?
    TIFR = bit(OCF0);
    ENABLE_TIMER0_PRESCALER;
}
#endif

// other unhandled interrupts should execute asm("BREAK") to be notified in avrora's particle-monitor
#ifdef __AVR_ATmega16__

/**
 * Timer/Counter0 Overflow
 */
ISR(TIMER0_OVF_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Input Capture
 */
ISR(TIMER1_CAPT_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Compare Match A
 */
ISR(TIMER1_COMPA_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Compare Match B
 */
ISR(TIMER1_COMPB_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * Timer/Counter1 Overflow
 */
ISR(TIMER1_OVF_vect) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

/**
 * External Pin, Power-on Reset, Brown-Out Reset, Watchdog Reset
 */
ISR(_VECTOR(0)) {
    SREG unsetBit bit(SREG_I);
    asm("BREAK");
}

#endif