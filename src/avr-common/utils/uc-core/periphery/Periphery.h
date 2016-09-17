/**
 * @author Raoul Rubien  17.09.2016
 */

#pragma once

#include "uc-core/particle/Globals.h"
#include "uc-core/configuration/IoPins.h"
#include "common/common.h"
#include "uc-core/delay/delay.h"

/**
 * Blinks the current address on status led 1.
 * Blinks row number times, column number times followed by a flashing signal signalizing the information end.
 * A long break is inserted before the blinking starts again.
 * The function never returns.
 */
extern FUNC_ATTRS void blinkAddressForever(void);

FUNC_ATTRS void blinkAddressForever(void) {
    LED_STATUS1_OFF;
    forever {
        for (int8_t row = 0; row < ParticleAttributes.node.address.row; row++) {
            LED_STATUS1_TOGGLE;
            DELAY_MS_196;
            LED_STATUS1_TOGGLE;
            DELAY_MS_196;
        }
        DELAY_MS_500;
        for (int8_t column = 0; column < ParticleAttributes.node.address.column; column++) {
            LED_STATUS1_TOGGLE;
            DELAY_MS_196;
            LED_STATUS1_TOGGLE;
            DELAY_MS_196;
        }
        DELAY_MS_500;

        LED_STATUS1_TOGGLE;
        DELAY_MS_30;
        LED_STATUS1_TOGGLE;
        DELAY_MS_30;
        LED_STATUS1_TOGGLE;
        DELAY_MS_30;
        LED_STATUS1_TOGGLE;

        DELAY_MS_500;
        DELAY_MS_500;
    }
}