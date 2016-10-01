/**
 * @author Raoul Rubien  12.07.2016
 *
 * Local time related arguments.
 */

#pragma once

#define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_75_PERCENT_OF_UINT16_MAX ((uint16_t) 49151)
#define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_80_PERCENT_OF_UINT16_MAX ((uint16_t) 52428)
#define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_85_PERCENT_OF_UINT16_MAX ((uint16_t) 55704)
#define __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_90_PERCENT_OF_UINT16_MAX ((uint16_t) 58982)


/**
 * The default start value for time compare match interrupt.
 * Defines the duration of time periods which can be adjusted at run time.
 */
#define LOCAL_TIME_DEFAULT_INTERRUPT_DELAY __LOCAL_TIME_DEFAULT_INTERRUPT_DELAY_80_PERCENT_OF_UINT16_MAX
