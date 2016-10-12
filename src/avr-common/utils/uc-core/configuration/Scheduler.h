/**
 * @author Raoul Rubien 11.10.2016
 *
 * Scheduler related arguments.
 */

#pragma once

/**
 * Size of the task array the scheduler keeps track of.
 */
#define SCHEDULER_MAX_TASKS ((uint8_t)4)

/**
 * Task array id arguments:
 */
#define SCHEDULER_TASK_ID_ENABLE_ALERTS 0
#define SCHEDULER_TASK_ID_SETUP_LEDS 1
#define SCHEDULER_TASK_ID_SYNC_PACKAGE 2
#define SCHEDULER_TASK_ID_HEARTBEAT_LED_TOGGLE 3
