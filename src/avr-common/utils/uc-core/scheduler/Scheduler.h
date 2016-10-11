/**
 * @author Raoul Rubien 11.10.2016
 *
 * Scheduler related implementation.
 */

#pragma once

#include "Scheduler.h"
#include "uc-core/particle/Globals.h"
#include "common/common.h"

/**
 * Assures a task's start action is executed once asap after the desired start timestamp
 * and (on time limited tasks) the task's end action executed once after the end time stamp.
 * If a node state is specified (state limited tasks) the actions are performed only within this states.
 */
void processScheduler(void) {
    uint16_t now = ParticleAttributes.localTime.numTimePeriodsPassed;

    for (uint8_t idx = 0; idx < SCHEDULER_MAX_TASKS; idx++) {
        SchedulerTask *task = &ParticleAttributes.scheduler.tasks[idx];

        // on disabled task
        if (!task->isEnabled) {
            continue;
        }

        // on state limited task
        if (task->isStateLimited) {
            if (task->state != ParticleAttributes.node.state) {
                continue;
            }
        }

        // on time limited task
        if (task->isTimeLimited) {
            LED_STATUS3_ON;

            if (task->startTimestamp <= now && false == task->isStartActionExecuted) {

                if (task->startAction != NULL) {
                    task->startAction();
                }
                task->isStartActionExecuted = true;
            }
            else if (task->endTimestamp <= now &&
                     true == task->isStartActionExecuted &&
                     false == task->isEndActionExecuted) {
                if (task->endAction != NULL) {
                    task->endAction();
                }
                task->isEndActionExecuted = true;
                task->isExecuted = true;
                task->isEnabled = false;
            }
        }
        else {
            // on single shot task
            if (task->startTimestamp <= now && false == task->isStartActionExecuted) {
                if (task->startAction != NULL) {
                    task->startAction();
                }
                task->isStartActionExecuted = true;
                task->isExecuted = true;
                task->isEnabled = false;
                LED_STATUS1_ON;
            }
        }
    }

    ParticleAttributes.scheduler.lastCallToScheduler = now;
}