/**
 * @author Raoul Rubien  17.09.2016
 */

#pragma once

#include "uc-core/particle/Globals.h"
#include "uc-core/configuration/IoPins.h"
#include "common/common.h"
#include "uc-core/delay/delay.h"
#include "uc-core/configuration/Periphery.h"
#include "uc-core/configuration/interrupts/ReceptionPCI.h"

//extern FUNC_ATTRS void __disableInterruptsForBlockingBlinking(void);

static FUNC_ATTRS void __disableInterruptsForBlockingBlinking(void) {
    RX_NORTH_INTERRUPT_DISABLE;
    RX_EAST_INTERRUPT_DISABLE;
    RX_SOUTH_INTERRUPT_DISABLE;
}

//extern FUNC_ATTRS void __blinkAddressBlocking(void);
/**
 * Blinks row times, column times followed by a quitting flash signal.
 */
static FUNC_ATTRS void __blinkAddressBlocking(void) {
    LED_STATUS1_OFF;
    DELAY_MS_500;
    for (int8_t row = 0; row < ParticleAttributes.node.address.row; row++) {
        LED_STATUS1_TOGGLE;
        DELAY_MS_196;
        LED_STATUS1_TOGGLE;
        DELAY_MS_196;
    }
    DELAY_MS_196;
    DELAY_MS_196;
    for (int8_t column = 0; column < ParticleAttributes.node.address.column; column++) {
        LED_STATUS1_TOGGLE;
        DELAY_MS_196;
        LED_STATUS1_TOGGLE;
        DELAY_MS_196;
    }
    DELAY_MS_196;

    LED_STATUS1_TOGGLE;
    DELAY_MS_30;
    DELAY_MS_30;
    LED_STATUS1_TOGGLE;
    DELAY_MS_30;
    DELAY_MS_30;
    LED_STATUS1_TOGGLE;
    DELAY_MS_30;
    DELAY_MS_30;
    LED_STATUS1_TOGGLE;
}

/**
 * Blinks the current address on status led endlessly.
 */
extern FUNC_ATTRS void blinkAddressForever(void);

FUNC_ATTRS void blinkAddressForever(void) {
    __disableInterruptsForBlockingBlinking();
    LED_STATUS1_ON;
    DELAY_MS_500;
    forever {
        __blinkAddressBlocking();
        DELAY_MS_500;
    }
}


extern FUNC_ATTRS void blinkReceptionBufferOverflowErrorForever(void);
/**
 * Blinks LEDs to indicate the error state.
 * This function never returns.
 */
FUNC_ATTRS void blinkReceptionBufferOverflowErrorForever(void) {
    __disableInterruptsForBlockingBlinking();
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


extern FUNC_ATTRS void blinkGenericErrorForever(void);
/**
 * Blinks LEDs to indicate the error state.
 * This function never returns.
 */
FUNC_ATTRS void blinkGenericErrorForever(void) {
    __disableInterruptsForBlockingBlinking();
    forever {
        LED_STATUS1_OFF;
        LED_STATUS2_OFF;
        LED_STATUS3_OFF;
        DELAY_MS_196;
        LED_STATUS1_ON;
        LED_STATUS2_ON;
        LED_STATUS3_ON;
        DELAY_MS_196;
    }
}


extern FUNC_ATTRS void blinkParityErrorForever(bool parityBit);
/**
 * Blink LEDs to indicate a parity error state.
 * Sequence on parityBit == true: led 1, led2, led3
 * otherwise: led 3, led 2, led 1.
 * This function never returns.
 */
FUNC_ATTRS void blinkParityErrorForever(bool parityBit) {
    __disableInterruptsForBlockingBlinking();
    LED_STATUS1_OFF;
    LED_STATUS2_OFF;
    LED_STATUS3_OFF;

    if (parityBit == true) {
        forever {
            LED_STATUS1_ON;
            DELAY_MS_500;
            LED_STATUS1_OFF;
            LED_STATUS2_ON;
            DELAY_MS_500;
            LED_STATUS2_OFF;
            LED_STATUS3_ON;
            DELAY_MS_500;
            LED_STATUS3_OFF;
        }
    } else {
        forever {
            LED_STATUS3_ON;
            DELAY_MS_500;
            LED_STATUS3_OFF;
            LED_STATUS2_ON;
            DELAY_MS_500;
            LED_STATUS2_OFF;
            LED_STATUS1_ON;
            DELAY_MS_500;
            LED_STATUS1_OFF;
        }
    }
}

extern FUNC_ATTRS void ledsOnForever(void);

FUNC_ATTRS void ledsOnForever(void) {
    __disableInterruptsForBlockingBlinking();
    LED_STATUS1_ON;
    LED_STATUS2_ON;
    LED_STATUS3_ON;
    LED_STATUS4_ON;
    LED_STATUS5_ON;
    LED_STATUS6_ON;
    forever;
}

extern FUNC_ATTRS void blinkTimeIntervalNonblocking(void);

FUNC_ATTRS void blinkTimeIntervalNonblocking(void) {
    if (ParticleAttributes.periphery.blinkTimeInterval.lastExecutionTime ==
        ParticleAttributes.localTime.numTimePeriodsPassed) {
        return;
    }
    ParticleAttributes.periphery.blinkTimeInterval.lastExecutionTime =
            ParticleAttributes.localTime.numTimePeriodsPassed;

    switch (ParticleAttributes.periphery.blinkTimeInterval.blinkState) {

        case TIME_INTERVAL_BLINK_STATES_LED_ON:
            LED_STATUS2_ON;
            if (ParticleAttributes.periphery.blinkTimeInterval.localTimeMultiplier-- > 1) {
                return;
            }
            ParticleAttributes.periphery.blinkTimeInterval.localTimeMultiplier = TIME_INTERVAL_BLINK_STATES_PERIOD_MULTIPLIER;
            ParticleAttributes.periphery.blinkTimeInterval.blinkState = TIME_INTERVAL_BLINK_STATES_LED_OFF;
            break;

        case TIME_INTERVAL_BLINK_STATES_LED_OFF:
            LED_STATUS2_OFF;
            if (ParticleAttributes.periphery.blinkTimeInterval.localTimeMultiplier-- > 1) {
                return;
            }
            ParticleAttributes.periphery.blinkTimeInterval.localTimeMultiplier = TIME_INTERVAL_BLINK_STATES_PERIOD_MULTIPLIER;
            ParticleAttributes.periphery.blinkTimeInterval.blinkState = TIME_INTERVAL_BLINK_STATES_LED_ON;
            break;
        default:
            break;
    }

}


extern FUNC_ATTRS void blinkAddressNonblocking(void);

FUNC_ATTRS void blinkAddressNonblocking(void) {

    if (ParticleAttributes.periphery.blinkAddress.lastExecutionTime ==
        ParticleAttributes.localTime.numTimePeriodsPassed) {
        return;
    }
    ParticleAttributes.periphery.blinkAddress.lastExecutionTime = ParticleAttributes.localTime.numTimePeriodsPassed;

    switch (ParticleAttributes.periphery.blinkAddress.blinkAddressState) {

        case ADDRESS_BLINK_STATES_START:
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_BLINK_ROW;
            break;

        case ADDRESS_BLINK_STATES_BLINK_ROW:
            ParticleAttributes.periphery.blinkAddress.blinkRowCounter = ParticleAttributes.node.address.row;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_ROW_ON;
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_ON_COUNTER_MAX;
            break;

        case ADDRESS_BLINK_STATES_ROW_ON:
            LED_STATUS3_ON;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_OFF_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_ROW_OFF;
            break;

        case ADDRESS_BLINK_STATES_ROW_OFF:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }

            if (ParticleAttributes.periphery.blinkAddress.blinkRowCounter-- > 1) {
                ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_ON_COUNTER_MAX;
                ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_ROW_ON;
                return;
            }

            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_SEPARATION_BREAK_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_PAUSE;
            break;

        case ADDRESS_BLINK_STATES_PAUSE:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }

            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_BLINK_COLUMN;
            break;

        case ADDRESS_BLINK_STATES_BLINK_COLUMN:
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_ON_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkColumnCounter = ParticleAttributes.node.address.column;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_COLUMN_ON;
            break;

        case ADDRESS_BLINK_STATES_COLUMN_ON:
            LED_STATUS3_ON;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_OFF_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_COLUMN_OFF;
            break;

        case ADDRESS_BLINK_STATES_COLUMN_OFF:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }

            if (ParticleAttributes.periphery.blinkAddress.blinkColumnCounter-- > 1) {
                ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_ON_COUNTER_MAX;
                ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_COLUMN_ON;
                return;
            }

            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_SEPARATION_BREAK_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_PAUSE2;
            break;

        case ADDRESS_BLINK_STATES_PAUSE2:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }
            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay = ADDRESS_BLINK_STATES_LED_SEPARATION_FLASH_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_QUIT_SIGNAL;
            break;

        case ADDRESS_BLINK_STATES_QUIT_SIGNAL:
            LED_STATUS3_ON;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }

            ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay =
                    ADDRESS_BLINK_STATES_LED_SEPARATION_LONG_BREAK_COUNTER_MAX;
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_END;
            break;

        case ADDRESS_BLINK_STATES_END:
            LED_STATUS3_OFF;
            if (ParticleAttributes.periphery.blinkAddress.blinkAddressBlinkDelay-- > 0) {
                return;
            }
            ParticleAttributes.periphery.blinkAddress.blinkAddressState = ADDRESS_BLINK_STATES_START;
            break;
        default:
            break;
    }
}