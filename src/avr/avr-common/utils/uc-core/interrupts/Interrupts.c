/**
 * @author Raoul Rubien 2015
 */
#ifndef __DEFAULT_ISR_H
#define __DEFAULT_ISR_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <uc-core/ParticleTypes.h>

#include "Vectors.h"
#include "Reception.h"
#include "TimerCounter.h"

#include "../Globals.h"
#include "../IoDefinitions.h"
#include "../../simulation/SimulationUtils.h"
#include "../discovery/Discovery.h"
#include "../communication/Communication.h"

extern volatile ParticleState ParticleAttributes;

/**
 * south RX pin change interrupt on logical pin change
 */
ISR(SOUTH_PIN_CHANGE_INTERRUPT_VECT) {
    switch (ParticleAttributes.node.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (SOUTH_RX_IS_LO) {
                TIMER_NEIGHBOUR_SENSE_PAUSE;
                dispatchFallingDiscoveryEdge(&ParticleAttributes.discoveryPulseCounters.south);
                TIMER_NEIGHBOUR_SENSE_RESUME;
            }
            break;

            // on data received
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            dispatchReceivedDataEdge(&ParticleAttributes.ports.rx.south, SOUTH_RX_IS_HI);
            break;

        default:
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
            break;
    }
}


/**
 * east RX pin change interrupt on logical pin change
 */
ISR(EAST_PIN_CHANGE_INTERRUPT_VECT) {
    switch (ParticleAttributes.node.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (EAST_RX_IS_LO) {
                TIMER_NEIGHBOUR_SENSE_PAUSE;
                dispatchFallingDiscoveryEdge(&ParticleAttributes.discoveryPulseCounters.east);
                TIMER_NEIGHBOUR_SENSE_RESUME;
            }
            break;

            // on data received
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            dispatchReceivedDataEdge(&ParticleAttributes.ports.rx.east, EAST_RX_IS_HI);
            break;
        default:
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
            break;
    }
}


/**
 * north RX pin change interrupt on logical pin change
 */
ISR(NORTH_PIN_CHANGE_INTERRUPT_VECT) {
    switch (ParticleAttributes.node.state) {
        // on discovery pulse
        case STATE_TYPE_NEIGHBOURS_DISCOVERY:
            if (NORTH_RX_IS_LO) {
                TIMER_NEIGHBOUR_SENSE_PAUSE;
                dispatchFallingDiscoveryEdge(&ParticleAttributes.discoveryPulseCounters.north);
                TIMER_NEIGHBOUR_SENSE_RESUME;
            }
            break;

            // on data received
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            dispatchReceivedDataEdge(&ParticleAttributes.ports.rx.north, NORTH_RX_IS_HI);
            break;

        default:
            IF_SIMULATION_SWITCH_TO_ERRONEOUS_STATE;
            break;
    }
}


/**
 * On timer_counter match with compare register A.
 * In tx/rx states A equals DEFAULT_TX_RX_COMPARE_TOP_VALUE
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

            // on receive data counter compare
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            TIMER_TX_RX_COUNTER = 0;
//            TIMER_TX_RX_SYNC_COUNTER_RESUME;
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
ISR(TX_TIMER_CENTER_INTERRUPT_VECT) {
    switch (ParticleAttributes.node.state) {
        // on receive data counter compare
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            // TODO: if sth. to transmit put (bitMask & data) to the correct output port and >> mask
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
ISR(TX_RX_TIMEOUT_INTERRUPT_VECT) {
    switch (ParticleAttributes.node.state) {
        case STATE_TYPE_IDLE:
        case STATE_TYPE_TX_START:
        case STATE_TYPE_TX_DONE:
        case STATE_TYPE_SCHEDULE_COMMAND:
            TIMER_TX_RX_TIMEOUT_COUNTER_PAUSE;
            TIMER_TX_RX_TIMEOUT_COUNTER = 0;
            if (ParticleAttributes.ports.rx.north.isReceiving != 0) {
                --ParticleAttributes.ports.rx.north.isReceiving;
                UDR = 'N';
            }
            if (ParticleAttributes.ports.rx.east.isReceiving != 0) {
                --ParticleAttributes.ports.rx.east.isReceiving;
                UDR = 'E';
            }
            if (ParticleAttributes.ports.rx.south.isReceiving != 0) {
                --ParticleAttributes.ports.rx.south.isReceiving;
                UDR = 'S';
            }
            TIMER_TX_RX_TIMEOUT_COUNTER_RESUME;
            break;
        default:
            break;
    }
}

#ifdef IS_SIMULATION

const char isrVector0Msg[] PROGMEM = "ISR(_VECTOR(0))";
/**
 * On external pin, power-on reset, brown-out reset, watchdog reset.
 */
ISR(_VECTOR(0)) {
    writeToUart((PGM_P) pgm_read_word(&(isrVector0Msg)));
}

#endif

#endif
