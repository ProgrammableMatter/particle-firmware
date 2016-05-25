/**
 * @author Raoul Rubien 2015
 */
#ifndef __DEFAULT_PARTICLE_ISR_H__
#define __DEFAULT_PARTICLE_ISR_H__

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

#ifdef SIMULATION

#  include "../../simulation/SimulationMacros.h"

#endif

#define TIMER_NEIGHBOUR_SENSE_COUNTER_ON_INTERRUPT_ROLLBACK 12

#  ifdef TRY_INLINE_ISR_RELEVANT
#    define FUNC_ATTRS inline
#  else
#    define FUNC_ATTRS
#  endif

extern volatile ParticleState ParticleAttributes;

FUNC_ATTRS void __handleInputInterrupt(volatile PulseCounter *discoveryPulseCounter, volatile RxPort *rxPort,
                                       const bool isRxHigh) {
    switch (ParticleAttributes.node.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (!isRxHigh) {
                TIMER_NEIGHBOUR_SENSE_PAUSE;
                TIMER_NEIGHBOUR_SENSE_COUNTER -= TIMER_NEIGHBOUR_SENSE_COUNTER_ON_INTERRUPT_ROLLBACK;
                dispatchFallingDiscoveryEdge(discoveryPulseCounter);
                TIMER_NEIGHBOUR_SENSE_RESUME;
            }
            break;

            // on data received
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
        case STATE_TYPE_ENUMERATING_NEIGHBOURS:
        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
        case STATE_TYPE_ENUMERATED:
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

FUNC_ATTRS void __southTxHi(void) {
    SOUTH_TX_HI;
}

FUNC_ATTRS void __southTxLo(void) {
    SOUTH_TX_LO;
}

FUNC_ATTRS void __northTxHi(void) {
    NORTH_TX_HI;
}

FUNC_ATTRS void __northTxLo(void) {
    NORTH_TX_LO;
}

FUNC_ATTRS void __eastTxHi(void) {
    EAST_TX_HI;
}

FUNC_ATTRS void __eastTxLo(void) {
    EAST_TX_LO;
}


/**
 * rectifies/modulates the transmission signal according to the upcoming bit
 */
FUNC_ATTRS void __rectifyTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                         void (txLoImpl)(void)) {
    // TODO: do not rectify if tx-buffer is empty
    if (txPort->retainTransmission == false) {
        if (txPort->buffer.pointer.bitMask &
            txPort->buffer.bytes[txPort->buffer.pointer.byteNumber]) {
            txHiImpl();
        } else {
            txLoImpl();
        }
    }
}

/**
 * modulates the transmission signal according to the current bit and increments the buffer pointer
 */
FUNC_ATTRS void __modulateTransmissionBit(volatile TxPort *txPort, void (*txHiImpl)(void),
                                          void (txLoImpl)(void)) {
    if (txPort->retainTransmission == false) {
        if (isDataEnd(txPort)) {
            // return signal to default
            txLoImpl(); // inverted on receiver side
        } else {
            // write data bit to output (inverted)
            if (txPort->buffer.pointer.bitMask & txPort->buffer.bytes[txPort->buffer.pointer.byteNumber]) {
                txLoImpl();
            } else {
                txHiImpl();
            }
            bufferBitPointerNext(&txPort->buffer.pointer);
        }
    }
    else if (txPort->enableTransmission == true) {
        txPort->retainTransmission = false;
    }
}
/**
 * increments the timeout counters
 */
FUNC_ATTRS void __advanceReceptionTimeoutCounters(void) {

    if (ParticleAttributes.ports.rx.north.isReceiving == 1) {
        ParticleAttributes.ports.rx.north.isDataBuffered = true;
    }
    ParticleAttributes.ports.rx.north.isReceiving >>= 1;
    if (ParticleAttributes.ports.rx.north.isReceiving == false) {
        bufferBitPointerStart(&ParticleAttributes.ports.rx.north.buffer.pointer);

    }

//#ifdef SIMULATION
//    if (ParticleAttributes.ports.rx.north.isReceiving == 0) {
//        IF_SIMULATION_CHAR_OUT('U');
//    }
//#endif

    if (ParticleAttributes.ports.rx.east.isReceiving == 1) {
        ParticleAttributes.ports.rx.east.isDataBuffered = true;
    }
    ParticleAttributes.ports.rx.east.isReceiving >>= 1;
    if (ParticleAttributes.ports.rx.east.isReceiving == false) {
        bufferBitPointerStart(&ParticleAttributes.ports.rx.east.buffer.pointer);
        ParticleAttributes.ports.rx.east.isDataBuffered = true;
    }

    if (ParticleAttributes.ports.rx.south.isReceiving == 1) {
        ParticleAttributes.ports.rx.south.isDataBuffered = true;
    }
    ParticleAttributes.ports.rx.south.isReceiving >>= 1;
    if (ParticleAttributes.ports.rx.south.isReceiving == false) {
        bufferBitPointerStart(&ParticleAttributes.ports.rx.south.buffer.pointer);
        ParticleAttributes.ports.rx.south.isDataBuffered = true;
    }

}

/**
 * north RX pin change interrupt on logical pin change
 * int. #19
 */
ISR(NORTH_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.discoveryPulseCounters.north,
                           &ParticleAttributes.ports.rx.north,
                           NORTH_RX_IS_HI);
    RX_NORTH_INTERRUPT_CLEAR_PENDING;
}

/**
 * east RX pin change interrupt on logical pin change
 * int. #3
 */
ISR(EAST_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.discoveryPulseCounters.east,
                           &ParticleAttributes.ports.rx.east,
                           EAST_RX_IS_HI);
    RX_EAST_INTERRUPT_CLEAR_PENDING;
}

/**
 * south RX pin change interrupt on logical pin change
 * int. #2
 */
ISR(SOUTH_PIN_CHANGE_INTERRUPT_VECT) {
    __handleInputInterrupt(&ParticleAttributes.discoveryPulseCounters.south,
                           &ParticleAttributes.ports.rx.south,
                           SOUTH_RX_IS_HI);
    RX_SOUTH_INTERRUPT_CLEAR_PENDING;
}




/**
 * On timer_counter match with compare register A.
 * In tx/rx states A equals DEFAULT_TX_RX_COMPARE_TOP_VALUE
 * int. #7
 */
ISR(TX_RX_TIMER_TOP_INTERRUPT_VECT) {
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

        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
        case STATE_TYPE_ENUMERATING_NEIGHBOURS:
        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
        case STATE_TYPE_ENUMERATED:
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            // reception
            __resetReceptionCounter();
            __advanceReceptionTimeoutCounters();
            // transmission
            __rectifyTransmissionBit(&ParticleAttributes.ports.tx.north, __northTxHi, __northTxLo);
            __rectifyTransmissionBit(&ParticleAttributes.ports.tx.east, __eastTxHi, __eastTxLo);
            __rectifyTransmissionBit(&ParticleAttributes.ports.tx.south, __southTxHi, __southTxLo);
            break;

        default:

            break;
    }
}


/**
 * On timer_counter compare register B match. If there are bits to transmit, this interrupt
 * generates the signal(s) according to the state.
 * int. #8
 */
ISR(TX_TIMER_CENTER_INTERRUPT_VECT) {
//    IF_SIMULATION_CHAR_OUT('i');
    switch (ParticleAttributes.node.state) {
        // on receive data counter compare
        case STATE_TYPE_WAIT_FOR_BEING_ENUMERATED:
        case STATE_TYPE_ENUMERATING_NEIGHBOURS:
        case STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR:
        case STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR:
        case STATE_TYPE_ENUMERATED:
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            // reception
            __advanceReceptionTimeoutCounters();
            // transmission
            __modulateTransmissionBit(&ParticleAttributes.ports.tx.north, __northTxHi, __northTxLo);
            __modulateTransmissionBit(&ParticleAttributes.ports.tx.east, __eastTxHi, __eastTxLo);
            __modulateTransmissionBit(&ParticleAttributes.ports.tx.south, __southTxHi, __southTxLo);
            break;
        default:
            break;
    }
}


/**
 * On timer 0 compare interrupt. The interrupt is called multiple times (i.e. 8x) per
 * TX_RX_TIMER_TOP_INTERRUPT_VECT. The implementation updates the timeout per reception
 * port. A timeout indicates a fully received transmission.
 * int. #20
 */
ISR(TX_RX_TIMEOUT_INTERRUPT_VECT) {
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

# ifdef SIMULATION

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
 * int #10
 */
ISR(TX_RX_TIMEOUT_OVERFLOW_INTERRUPT_VECT) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
}

/**
 * on tx/rx timer overflow
 * int #9
 */
ISR(TX_RX_OVERFLOW_INTERRUPT_VECT) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
}


ISR(BADISR_vect) {
    IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
}

#  else

EMPTY_INTERRUPT(_VECTOR(0))

EMPTY_INTERRUPT(TX_RX_TIMEOUT_OVERFLOW_INTERRUPT_VECT)

EMPTY_INTERRUPT(TX_RX_OVERFLOW_INTERRUPT_VECT)

EMPTY_INTERRUPT(BADISR_vect)

#  endif


#  ifdef FUNC_ATTRS
#    undef FUNC_ATTRS
#  endif
#endif