/**
 * @author Raoul Rubien 2015
 */
#ifndef __DEFAULT_ISR_H
#define __DEFAULT_ISR_H

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <uc-core/ParticleTypes.h>

#include "./Vectors.h"
#include "./TimerCounter.h"
#include "./Reception.h"
#include "../Globals.h"
#include "../discovery/Discovery.h"
#include "../communication/Communication.h"
#include "../IoDefinitions.h"


#  ifdef TRY_INLINE_ISR_RELEVANT
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

FUNC_ATTRS void __handleInputInterrupt(volatile PulseCounter *discoveryPulseCounter, volatile RxPort *rxPort,
                                       const bool isRxHigh) {
    switch (ParticleAttributes.node.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (isRxHigh) {
                TIMER_NEIGHBOUR_SENSE_PAUSE;
                dispatchFallingDiscoveryEdge(discoveryPulseCounter);
                TIMER_NEIGHBOUR_SENSE_RESUME;
            }
            break;

            // on data received
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            dispatchReceivedDataEdge(rxPort, isRxHigh);
            break;

        default:
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
            break;
    }
}

/**
 * Resets the counter. In case the interrupt was shifted (less than 1x compare value), the counter is trimmed,
 * else in case of simulation it switches to erroneous state.
 */
FUNC_ATTRS void __resetReceptionCounter(void) {
//#ifdef SIMULATION
//    if (TIMER_TX_RX_COUNTER > (2 * TIMER_TX_RX_COMPARE_TOP_VALUE)) {
//        IF_SIMULATION_CHAR_OUT('r');
//        IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
//    }
//#endif
    if (TIMER_TX_RX_COUNTER > TIMER_TX_RX_COMPARE_TOP_VALUE) {
        TIMER_TX_RX_COUNTER -= TIMER_TX_RX_COMPARE_TOP_VALUE;
    } else {
        TIMER_TX_RX_COUNTER = 0;
    }
}

/**
 *
 */
FUNC_ATTRS void __advanceTimeoutCounters(void) {
    ParticleAttributes.ports.rx.north.isReceiving >>= 1;

    if (ParticleAttributes.ports.rx.north.isReceiving == 0)
        IF_SIMULATION_CHAR_OUT('U');
    ParticleAttributes.ports.rx.east.isReceiving >>= 1;
    ParticleAttributes.ports.rx.south.isReceiving >>= 1;

}

/**
 * north RX pin change interrupt on logical pin change
 */
ISR(NORTH_PIN_CHANGE_INTERRUPT_VECT) { // int. #19
    __handleInputInterrupt(&ParticleAttributes.discoveryPulseCounters.north,
                           &ParticleAttributes.ports.rx.north,
                           NORTH_RX_IS_HI);
    RX_INTERRUPTS_CLEAR_PENDING_NORTH;
}

/**
 * east RX pin change interrupt on logical pin change
 */
ISR(EAST_PIN_CHANGE_INTERRUPT_VECT) { // int. #3
    __handleInputInterrupt(&ParticleAttributes.discoveryPulseCounters.east, &ParticleAttributes.ports.rx.east,
                           EAST_RX_IS_HI);
    RX_INTERRUPTS_CLEAR_PENDING_EAST;
}

/**
 * south RX pin change interrupt on logical pin change
 */
ISR(SOUTH_PIN_CHANGE_INTERRUPT_VECT) { // int. #2
    __handleInputInterrupt(&ParticleAttributes.discoveryPulseCounters.south,
                           &ParticleAttributes.ports.rx.south,
                           SOUTH_RX_IS_HI);
    RX_INTERRUPTS_CLEAR_PENDING_SOUTH;
}

/**
 * On timer_counter match with compare register A.
 * In tx/rx states A equals DEFAULT_TX_RX_COMPARE_TOP_VALUE
 */
ISR(TX_RX_TIMER_TOP_INTERRUPT_VECT) { // int. #7
    switch (ParticleAttributes.node.state) {
        // on generate discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
        case STATE_TYPE_DISCOVERY_PULSING:
            TIMER_NEIGHBOUR_SENSE_PAUSE;
            TIMER_NEIGHBOUR_SENSE_COUNTER = 0;

            NORTH_TX_TOGGLE;
            SOUTH_TX_TOGGLE;
            EAST_TX_TOGGLE;

            TIMER_NEIGHBOUR_SENSE_RESUME;
            break;

            // on receive data counter compare
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            __resetReceptionCounter();
            __advanceTimeoutCounters();
            // if sth. to transmit put !(bitMask & data) to the output
            break;

        default:
            break;
    }
}

/**
 * On timer_counter compare register B match. If there are bits to transmit, this interrupt
 * generates the signal(s) according to the state.
 */
ISR(TX_TIMER_CENTER_INTERRUPT_VECT) { // int. #8
    switch (ParticleAttributes.node.state) {
        // on receive data counter compare
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            // TODO: if sth. to transmit put (bitMask & data) to the correct output port and >> mask
            __advanceTimeoutCounters();
            break;
        default:
            break;
    }
}


/**
 * On timer 0 compare interrupt. The interrupt is called multiple times (i.e. 8x) per
 * TX_RX_TIMER_TOP_INTERRUPT_VECT. The implementation updates the timeout per reception
 * port. A timeout indicates a fully received transmission.
 */
ISR(TX_RX_TIMEOUT_INTERRUPT_VECT) { // int. #20
    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            break;
        default:
            break;
    }
}

#  ifdef SIMULATION

const char isrVector0Msg[] PROGMEM = "BAD ISR";
/**
 * On external pin, power-on reset, brown-out reset, watchdog reset.
 */
ISR(_VECTOR(0)) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
}

/**
 * on tx/rx timeout timer overflow
 */
ISR(TX_RX_TIMEOUT_OVERFLOW_INTERRUPT) { // int #10
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
}

/**
 * on tx/rx timer overflow
 */
ISR(TX_RX_OVERFLOW_INTERRUPT) { // int #9
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
}

#  endif
#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif
