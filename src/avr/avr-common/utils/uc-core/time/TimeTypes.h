/**
 * @author Raoul Rubien  12.07.16.
 */

#pragma once
/**
 * Keeps track of number intervals passed since time tracking was activated.
 * A time interval lasts (in theory) exactly 6mS in case of accurate CPU clock.
 * The counter overflows at 65000, or each 390 seconds or each 6 minutes and 30 seconds.
 */
typedef struct LocalTime {
    uint16_t numTimeIntervalPassed;
} LocalTime;
