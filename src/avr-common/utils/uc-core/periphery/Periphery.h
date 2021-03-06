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
#include "uc-core/scheduler/SchedulerTypes.h"

#ifndef PERIPHERY_REMOVE_IMPL

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
static void __blinkBufferOverflowErrorForever(uint8_t portNumber) {
    if (ParticleAttributes.alerts.isRxBufferOverflowEnabled == false) {
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

void blinkReceptionBufferOverflowErrorForever(RxPort *const rxPort) {
    uint8_t portNumber = 5;
    if (rxPort == ParticleAttributes.directionOrientedPorts.north.rxPort) {
        portNumber = 1;
    } else if (rxPort == ParticleAttributes.directionOrientedPorts.east.rxPort) {
        portNumber = 2;
    } else if (rxPort == ParticleAttributes.directionOrientedPorts.south.rxPort) {
        portNumber = 3;
    }
    __blinkBufferOverflowErrorForever(2 * portNumber);
}

void blinkReceptionSnapshotBufferOverflowErrorForever(RxSnapshotBuffer *const snapshotBuffer) {
    uint8_t portNumber = 5;
    if (snapshotBuffer == &ParticleAttributes.directionOrientedPorts.north.rxPort->snapshotsBuffer) {
        portNumber = 1;
    } else if (snapshotBuffer == &ParticleAttributes.directionOrientedPorts.east.rxPort->snapshotsBuffer) {
        portNumber = 2;
    } else if (snapshotBuffer == &ParticleAttributes.directionOrientedPorts.south.rxPort->snapshotsBuffer) {
        portNumber = 3;
        __blinkBufferOverflowErrorForever(portNumber);
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

void blinkLed1Forever(void) {
    if (ParticleAttributes.alerts.isGenericErrorEnabled == false) {
        __ledsOff();
        LED_STATUS1_ON;
        LED_STATUS2_ON;
        return;
    }

    __disableInterruptsForBlockingBlinking();
    __blinkLedNForever(1);
}

void blinkLed2Forever(void) {
    if (ParticleAttributes.alerts.isGenericErrorEnabled == false) {
        __ledsOff();
        LED_STATUS2_ON;
        return;
    }
    __disableInterruptsForBlockingBlinking();
    __blinkLedNForever(2);
}

void blinkLed3Forever(void) {
    if (ParticleAttributes.alerts.isGenericErrorEnabled == false) {
        __ledsOff();
        LED_STATUS3_ON;
        return;
    }
    __disableInterruptsForBlockingBlinking();
    __blinkLedNForever(3);
}

void blinkLed4Forever(void) {
    if (ParticleAttributes.alerts.isGenericErrorEnabled == false) {
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
void blinkParityErrorForever(bool parityBit) {
    if (ParticleAttributes.alerts.isRxParityErrorEnabled == false) {
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
    uint8_t sreg = SREG;
    MEMORY_BARRIER;
    CLI;
    MEMORY_BARRIER;
    const uint16_t now = ParticleAttributes.localTime.numTimePeriodsPassed;
    MEMORY_BARRIER;
    SREG = sreg;
    MEMORY_BARRIER;

    if (ParticleAttributes.periphery.blinkTimeInterval.lastExecutionTime == now) {
        return;
    }
    ParticleAttributes.periphery.blinkTimeInterval.lastExecutionTime = now;

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

    uint8_t sreg = SREG;
    MEMORY_BARRIER;
    CLI;
    MEMORY_BARRIER;
    const uint16_t now = ParticleAttributes.localTime.numTimePeriodsPassed;
    MEMORY_BARRIER;
    SREG = sreg;
    MEMORY_BARRIER;

    if (ParticleAttributes.periphery.blinkAddress.lastExecutionTime == now) {
        return;
    }
    ParticleAttributes.periphery.blinkAddress.lastExecutionTime = now;

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

/**
 * Puts all LEDs into a defined state. Since leds are toggled
 * during initialization process their state is undetermined.
 */
void setupLedsState(SchedulerTask *task) {
    __ledsOff();
    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN ||
        ParticleAttributes.node.type == NODE_TYPE_INTER_HEAD) {
        LED_STATUS1_ON;
    }
    if (ParticleAttributes.node.type == NODE_TYPE_TAIL) {
        LED_STATUS4_ON;
    }
    task->isEnabled = false;
}


/**
 * Toggles the heartbeat LED.
 */
void heartBeatToggle(SchedulerTask *task) {
    LED_STATUS4_TOGGLE;
    task->isEnabled = true;
}

#else
#define blinkReceptionSnapshotBufferOverflowErrorForever(...)
#define blinkReceptionBufferOverflowErrorForever(...)
#define blinkGenericErrorForever(...)
#define blinkLed1Forever(...)
#define blinkLed2Forever(...)
#define blinkLed3Forever(...)
#define blinkLed4Forever(...)
#define blinkInterruptErrorForever(...)
#define blinkParityErrorForever(...)
#define ledsOnForever(...)
#define blinkTimeIntervalNonblocking(...)
#define blinkAddressNonblocking(...)
#define blinkKnightRidersKittForever(...)
#define setupLedsBeforeProcessing(...)
#endif
