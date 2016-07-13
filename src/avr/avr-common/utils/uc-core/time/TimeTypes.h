/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once
/**
 * Keeps track of number intervals passed since time tracking was activated.
 * A time interval lasts (in theory) exactly 6mS in case of accurate CPU clock.
 */
typedef struct LocalTimeTracking {
    uint16_t numTimePeriodsPassed;
    /**
     * adjustable delay for fine tuning
     */
    uint16_t interruptDelay;
} LocalTimeTracking;

