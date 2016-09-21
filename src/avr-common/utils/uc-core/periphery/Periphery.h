/**
 * @author Raoul Rubien  17.09.2016
 */

#pragma once

#include "uc-core/particle/Globals.h"
#include "uc-core/configuration/IoPins.h"
#include "common/common.h"
#include "uc-core/delay/delay.h"
#include "uc-core/configuration/Periphery.h"


//extern FUNC_ATTRS void blinkAddress(void);
///**
// * Blinks row times, column times followed by a quitting flash signal.
// */
//FUNC_ATTRS void blinkAddress(void) {
//    LED_STATUS1_OFF;
//    DELAY_MS_500;
//    for (int8_t row = 0; row < ParticleAttributes.node.address.row; row++) {
//        LED_STATUS1_TOGGLE;
//        DELAY_MS_196;
//        LED_STATUS1_TOGGLE;
//        DELAY_MS_196;
//    }
//    DELAY_MS_196;
//    DELAY_MS_196;
//    for (int8_t column = 0; column < ParticleAttributes.node.address.column; column++) {
//        LED_STATUS1_TOGGLE;
//        DELAY_MS_196;
//        LED_STATUS1_TOGGLE;
//        DELAY_MS_196;
//    }
//    DELAY_MS_196;
//
//    LED_STATUS1_TOGGLE;
//    DELAY_MS_30;
//    DELAY_MS_30;
//    LED_STATUS1_TOGGLE;
//    DELAY_MS_30;
//    DELAY_MS_30;
//    LED_STATUS1_TOGGLE;
//    DELAY_MS_30;
//    DELAY_MS_30;
//    LED_STATUS1_TOGGLE;
//}

///**
// * Blinks the current address on status led endlessly.
// */
//extern FUNC_ATTRS void blinkAddressForever(void);
//
//FUNC_ATTRS void blinkAddressForever(void) {
//    LED_STATUS1_ON;
//    DELAY_MS_500;
//    forever {
//        blinkAddress();
//        DELAY_MS_500;
//    }
//}


///**
// * Decrements the count down until zero.
// */
//extern FUNC_ATTRS void advanceDeadEndCountDown(void);
//
//FUNC_ATTRS void advanceDeadEndCountDown(void) {
//    if (ParticleAttributes.periphery.countdownUntilDeadEndMode != 0) {
//        ParticleAttributes.periphery.countdownUntilDeadEndMode--;
//        DELAY_US_15;
//    }
//}

extern FUNC_ATTRS void blinkErrorForever(void);
/**
 * Blinks LEDs to indicate the error state.
 * This function never returns.
 */
FUNC_ATTRS void blinkErrorForever(void) {
    forever {
        LED_STATUS1_OFF;
        LED_STATUS2_ON;
        LED_STATUS3_OFF;
        DELAY_MS_196;
        LED_STATUS1_ON;
        LED_STATUS2_OFF;
        LED_STATUS3_ON;
        DELAY_MS_196;
    }
}

//extern FUNC_ATTRS void ledsOnForever(void);
//
//FUNC_ATTRS void ledsOnForever(void) {
//    LED_STATUS1_ON;
//    LED_STATUS2_ON;
//    LED_STATUS3_ON;
//    LED_STATUS4_ON;
//    LED_STATUS5_ON;
//    LED_STATUS6_ON;
//    forever;
//}


extern FUNC_ATTRS void blinkAddressNonblocking(void);

FUNC_ATTRS void blinkAddressNonblocking(void) {

    if (ParticleAttributes.periphery.blinkAddress.lastExecutionTime ==
        ParticleAttributes.localTime.numTimePeriodsPassed) {
        return;
    }

    switch (ParticleAttributes.periphery.blinkAddress.blinkAddressState) {

        case LED_BLINK_STATES_START:
            ParticleAttributes.periphery.blinkAddress.lastExecutionTime = ParticleAttributes.localTime.numTimePeriodsPassed;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_BLINK_ROW;
            break;

        case LED_BLINK_STATES_BLINK_ROW:
            ParticleAttributes.periphery.blinkAddress.blinkRowCounter = ParticleAttributes.node.address.row;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_ROW_ON;
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_ON_COUNTER_MAX;
            break;

        case LED_BLINK_STATES_ROW_ON:
            LED_STATUS3_ON;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_OFF_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_ROW_OFF;
            break;

        case LED_BLINK_STATES_ROW_OFF:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }

            if (ParticleAttributes.periphery.blinkAddress.blinkRowCounter-- > 1) {
                ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_ON_COUNTER_MAX;
                ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_ROW_ON;
                return;
            }

            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_SEPARATION_BREAK_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_PAUSE;
            break;

        case LED_BLINK_STATES_PAUSE:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }

            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_BLINK_COLUMN;
            break;

        case LED_BLINK_STATES_BLINK_COLUMN:
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_ON_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkColumnCounter = ParticleAttributes.node.address.column;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_COLUMN_ON;
            break;

        case LED_BLINK_STATES_COLUMN_ON:
            LED_STATUS3_ON;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_OFF_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_COLUMN_OFF;
            break;

        case LED_BLINK_STATES_COLUMN_OFF:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }

            if (ParticleAttributes.periphery.blinkAddress.blinkColumnCounter-- > 1) {
                ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_ON_COUNTER_MAX;
                ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_COLUMN_ON;
                return;
            }

            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_SEPARATION_BREAK_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_PAUSE2;
            break;

        case LED_BLINK_STATES_PAUSE2:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = BLINK_ADDRESS_LED_SEPARATION_FLASH_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_QUIT_SIGNAL;
            break;

        case LED_BLINK_STATES_QUIT_SIGNAL:
            LED_STATUS3_ON;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }

            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay =
                    BLINK_ADDRESS_LED_SEPARATION_LONG_BREAK_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_END;
            break;

        case LED_BLINK_STATES_END:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = LED_BLINK_STATES_START;
            break;
        default:
            break;
    }
}