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

static void __disableInterruptsForBlockingBlinking(void) {
    RX_NORTH_INTERRUPT_DISABLE;
    RX_EAST_INTERRUPT_DISABLE;
    RX_SOUTH_INTERRUPT_DISABLE;
}

static void __ledsOff(void) {
    LED_STATUS1_OFF;
    LED_STATUS2_OFF;
    LED_STATUS3_OFF;
    LED_STATUS4_OFF;
}

static void __ledsOn(void) {
    LED_STATUS1_ON;
    LED_STATUS2_ON;
    LED_STATUS3_ON;
    LED_STATUS4_ON;
}

///**
// * Blinks row times, column times followed by a quitting flash signal.
// */
//static void __blinkAddressBlocking(void) {
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
//
///**
// * Blinks the current address on status led endlessly.
// */
//void blinkAddressForever(void) {
//    __disableInterruptsForBlockingBlinking();
//    LED_STATUS1_ON;
//    DELAY_MS_500;
//    forever {
//        __blinkAddressBlocking();
//        DELAY_MS_500;
//    }
//}

/**
 * Blinks LEDs to indicate the error state.
 * This function never returns.
 */
void blinkReceptionBufferOverflowErrorForever(const Alerts *const alerts, uint8_t portNumber) {
    if (alerts->isRxBufferOverflowEnabled == false) {
        __ledsOff();
        LED_STATUS2_ON;
        LED_STATUS3_ON;
        return;
    }

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

        for (uint8_t i = 0; i < portNumber; i++) {
            LED_STATUS4_OFF;
            DELAY_MS_196;
            LED_STATUS4_ON;
            DELAY_MS_196;
        }
    }
}

/**
 * Blinks LEDs to indicate the error state.
 * This function never returns.
 */
void blinkGenericErrorForever(void) {
    __disableInterruptsForBlockingBlinking();
    forever {
        __ledsOff();
        DELAY_MS_196;
        __ledsOn();
        DELAY_MS_196;
    }
}

static void __blinkLedNForever(uint8_t n) {
    __disableInterruptsForBlockingBlinking();
    __ledsOn();
    forever {
        DELAY_MS_196;
        switch (n) {
            case 1:
                LED_STATUS1_TOGGLE;
                break;
            case 2:
                LED_STATUS2_TOGGLE;
                break;
            case 3:
                LED_STATUS3_TOGGLE;
                break;
            case 4:
                LED_STATUS4_TOGGLE;
                break;
        }
        DELAY_MS_196;
    }
}

void blinkLed1Forever(const Alerts *const alerts) {
    if (alerts->isGenericErrorEnabled == false) {
        __ledsOff();
        LED_STATUS1_ON;
        LED_STATUS2_ON;
        return;
    }

    __disableInterruptsForBlockingBlinking();
    __blinkLedNForever(1);
}

void blinkLed2Forever(const Alerts *alerts) {
    if (alerts->isGenericErrorEnabled == false) {
        __ledsOff();
        LED_STATUS2_ON;
        return;
    }
    __disableInterruptsForBlockingBlinking();
    __blinkLedNForever(2);
}

void blinkLed3Forever(const Alerts *const alerts) {
    if (alerts->isGenericErrorEnabled == false) {
        __ledsOff();
        LED_STATUS3_ON;
        return;
    }
    __disableInterruptsForBlockingBlinking();
    __blinkLedNForever(3);
}

void blinkLed4Forever(const Alerts *const alerts) {
    if (alerts->isGenericErrorEnabled == false) {
        __ledsOff();
        LED_STATUS4_ON;
        return;
    }
    __disableInterruptsForBlockingBlinking();
    __blinkLedNForever(4);
}

void blinkInterruptErrorForever(void) {
    __disableInterruptsForBlockingBlinking();
    forever {
        LED_STATUS1_OFF;
        LED_STATUS2_ON;
        LED_STATUS3_OFF;
        LED_STATUS4_ON;
        DELAY_MS_196;
        LED_STATUS1_ON;
        LED_STATUS2_OFF;
        LED_STATUS3_ON;
        LED_STATUS4_OFF;
        DELAY_MS_196;
    }
}

/**
 * Blink LEDs to indicate a parity error state.
 * Sequence on parityBit == true: led 1, led2, led3
 * otherwise: led 3, led 2, led 1.
 * This function never returns.
 */
void blinkParityErrorForever(const Alerts *const alerts, bool parityBit) {
    if (alerts->isRxParityErrorEnabled == false) {
        __ledsOff();
        LED_STATUS1_ON;
        LED_STATUS2_ON;
        return;
    }

    __disableInterruptsForBlockingBlinking();
    __ledsOff();

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

void ledsOnForever(void) {
    __disableInterruptsForBlockingBlinking();
    __ledsOn();
    forever;
}


void blinkTimeIntervalNonblocking(void) {
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

void blinkAddressNonblocking(void) {

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

///**
// * Toggles the heartbeat LED.
// */
//void heartBeatToggle(void) {
////    if (ParticleAttributes.periphery.loopCount++ == 0) {
//    if (ParticleAttributes.periphery.loopCount == 0) {
//        LED_STATUS4_TOGGLE;
//        ParticleAttributes.periphery.loopCount--;
//    }
//}

void blinkKnightRidersKittForever(void) {
    __disableInterruptsForBlockingBlinking();

#define __kittDelay DELAY_MS_196

    forever {
        LED_STATUS1_ON;
        LED_STATUS2_OFF;
        LED_STATUS3_OFF;
        LED_STATUS4_OFF;
        __kittDelay;

        LED_STATUS1_ON;
        LED_STATUS2_ON;
        LED_STATUS3_OFF;
        LED_STATUS4_OFF;
        __kittDelay;

        LED_STATUS1_OFF;
        LED_STATUS2_ON;
        LED_STATUS3_ON;
        LED_STATUS4_OFF;
        __kittDelay;

        LED_STATUS1_OFF;
        LED_STATUS2_OFF;
        LED_STATUS3_ON;
        LED_STATUS4_ON;
        __kittDelay;

        LED_STATUS1_OFF;
        LED_STATUS2_OFF;
        LED_STATUS3_OFF;
        LED_STATUS4_ON;
        __kittDelay;


        LED_STATUS1_OFF;
        LED_STATUS2_OFF;
        LED_STATUS3_ON;
        LED_STATUS4_ON;
        __kittDelay;


        LED_STATUS1_OFF;
        LED_STATUS2_ON;
        LED_STATUS3_ON;
        LED_STATUS4_OFF;
        __kittDelay;

        LED_STATUS1_ON;
        LED_STATUS2_ON;
        LED_STATUS3_OFF;
        LED_STATUS4_OFF;
        __kittDelay;
    }
}