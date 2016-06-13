/**
 * @author Raoul Rubien 2015
 */
#pragma once

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <uc-core/ParticleStateTypes.h>

#include "Vectors.h"
#include "TimerCounter.h"
#include "Reception.h"
#include "uc-core/communication/Transmission.h"
#include "uc-core/Globals.h"
#include "uc-core/discovery/Discovery.h"
#include "uc-core/communication/Reception.h"
#include "uc-core/io-configuration/IoDefinitions.h"
#include "uc-core/communication/ManchesterDecoding.h"
#include "uc-core/communication/ManchesterCoding.h"

#ifdef SIMULATION

#  include "simulation/SimulationMacros.h"

#endif

#define TIMER_NEIGHBOUR_SENSE_COUNTER_ON_INTERRUPT_ROLLBACK 12

#ifdef TRY_INLINE_ISR_RELEVANT
#  define FUNC_ATTRS inline
#else
#  define FUNC_ATTRS
#endif

FUNC_ATTRS void __handleInputInterrupt(volatile PulseCounter *discoveryPulseCounter, volatile RxPort *rxPort,
                                       const bool isRxHigh) {
    switch (ParticleAttributes.node.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (!isRxHigh) {
                dispatchFallingDiscoveryEdge(discoveryPulseCounter);
            }
            break;

        default:
            switch (ParticleAttributes.ports.xmissionState) {
                // on data received
                case STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX:
                case STATE_TYPE_XMISSION_TYPE_ENABLED_RX:
//                    dispatchReceivedDataEdge(rxPort, isRxHigh);
                    appendSnapshot(isRxHigh, &rxPort->snapshotBuffer);
                    break;

                default:
                    break;
            }
            break;
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
        case STATE_TYPE_START:
        case STATE_TYPE_ACTIVE:
            break;

            // on generate discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
        case STATE_TYPE_DISCOVERY_PULSING:
            NORTH_TX_TOGGLE;
            SOUTH_TX_TOGGLE;
            EAST_TX_TOGGLE;
            break;

            // otherwise process transmission and reception
        default:
            switch (ParticleAttributes.ports.xmissionState) {
                case STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX:
                case STATE_TYPE_XMISSION_TYPE_ENABLED_TX:
                case STATE_TYPE_XMISSION_TYPE_ENABLED_RX:
                    rectifyTransmissionBit(&ParticleAttributes.ports.tx.north, __northTxHi, __northTxLo);
                    rectifyTransmissionBit(&ParticleAttributes.ports.tx.east, __eastTxHi, __eastTxLo);
                    rectifyTransmissionBit(&ParticleAttributes.ports.tx.south, __southTxHi, __southTxLo);
                    break;

                default:
                    break;
            }
            break;
    }
}


/**
 * On timer_counter compare register B match. If there are bits to transmit, this interrupt
 * generates the signal(s) according to the state, otherwise only timeout counters are processed.
 * int. #8
 */
ISR(TX_RX_TIMER_CENTER_INTERRUPT_VECT) {
//    IF_SIMULATION_CHAR_OUT('i');
    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_START:
        case STATE_TYPE_ACTIVE:
            break;

            // on generate discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
        case STATE_TYPE_NEIGHBOURS_DISCOVERED:
        case STATE_TYPE_DISCOVERY_PULSING:
            break;

        default:
            switch (ParticleAttributes.ports.xmissionState) {
                case STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX:
                case STATE_TYPE_XMISSION_TYPE_ENABLED_TX:
                case STATE_TYPE_XMISSION_TYPE_ENABLED_RX:
                    modulateTransmissionBit(&ParticleAttributes.ports.tx.north, __northTxHi, __northTxLo);
                    modulateTransmissionBit(&ParticleAttributes.ports.tx.east, __eastTxHi, __eastTxLo);
                    modulateTransmissionBit(&ParticleAttributes.ports.tx.south, __southTxHi, __southTxLo);
                    break;

                default:
                    break;
            }
            break;
    }
}


/**
 * On timer 0 compare interrupt. The interrupt is called multiple times (i.e. 8x) per
 * TX_RX_TIMER_TOP_INTERRUPT_VECT. The implementation updates the timeout per reception
 * port. A timeout indicates a fully received transmission.
 * int. #20
 */
EMPTY_INTERRUPT(TX_RX_TIMEOUT_INTERRUPT_VECT)
//{
//    switch (ParticleAttributes.node.state) {
//        case STATE_TYPE_IDLE:
//        case STATE_TYPE_TX_START:
//        case STATE_TYPE_TX_DONE:
//        case STATE_TYPE_SCHEDULE_COMMAND:
//            break;
//        default:
//            break;
//    }
//}

# ifdef SIMULATION

const char isrVector0Msg[] PROGMEM = "BAD ISR";
/**
 * On external pin, power-on reset, brown-out reset, watchdog reset.
 */
ISR(_VECTOR(0)) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}

/**
 * on tx/rx timeout timer overflow
 * int #10
 */
ISR(TX_RX_TIMEOUT_OVERFLOW_INTERRUPT_VECT) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}

/**
 * on tx/rx timer overflow
 * int #9
 */
ISR(TX_RX_OVERFLOW_INTERRUPT_VECT) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}


ISR(BADISR_vect) {
    IF_DEBUG_SWITCH_TO_ERRONEOUS_STATE;
}

#  else

EMPTY_INTERRUPT(_VECTOR(0))

EMPTY_INTERRUPT(TX_RX_TIMEOUT_OVERFLOW_INTERRUPT_VECT)

EMPTY_INTERRUPT(TX_RX_OVERFLOW_INTERRUPT_VECT)

EMPTY_INTERRUPT(BADISR_vect)

#  endif


#ifdef FUNC_ATTRS
#  undef FUNC_ATTRS
#endif