/**
 * @author Raoul Rubien  17.09.2016
 */

#pragma once

#include "uc-core/particle/Globals.h"
#include "uc-core/configuration/IoPins.h"
#include "common/common.h"
#include "uc-core/delay/delay.h"

extern FUNC_ATTRS void blinkAddress(void);
/**
 * Blinks row times, column times followed by a quitting flash signal.
 */
FUNC_ATTRS void blinkAddress(void) {
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
    forever {
        blinkAddress();
        DELAY_MS_500;
    }
}


/**
 * Decrements the count down until zero.
 */
extern FUNC_ATTRS void advanceDedEndCountDown(void);

FUNC_ATTRS void advanceDedEndCountDown(void) {
    if (ParticleAttributes.periphery.countdownUntilDeadEndMode != 0) {
        ParticleAttributes.periphery.countdownUntilDeadEndMode--;
    }
}

