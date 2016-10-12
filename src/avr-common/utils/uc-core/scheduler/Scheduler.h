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
 * adds a single shot task
 */
void addSingleShotTask(const uint8_t taskId, void (*const action)(SchedulerTask *const task),
                       const uint16_t timestamp) {
    SchedulerTask *task = &ParticleAttributes.scheduler.tasks[taskId];
    constructSchedulerTask(task);
    task->startAction = action;
    task->startTimestamp = timestamp;
    task->isEnabled = true;
}

void addCyclicTask(const uint8_t taskId, void (*const action)(SchedulerTask *const task),
                   const uint16_t timestamp,
                   const uint16_t separation) {
    SchedulerTask *task = &ParticleAttributes.scheduler.tasks[taskId];
    constructSchedulerTask(task);
    task->isCyclicTask = true;
    task->startAction = action;
    task->startTimestamp = timestamp;
    task->reScheduleDelay = separation;
    task->isEnabled = true;
}

/**
 * increment pending retained counters
 */
static void __advancePendingOverflowingCounters(void) {
    for (uint8_t idx = 0; idx < SCHEDULER_MAX_TASKS; idx++) {
        SchedulerTask *task = &ParticleAttributes.scheduler.tasks[idx];

        if (task->__isCounterRetained) {
            task->startTimestamp += task->reScheduleDelay;
            task->__isCounterRetained = false;
        }
    }
}

/**
 * Assures a task's start action is executed once asap after the desired start timestamp
 * and (on time limited tasks) the task's end action executed once after the end time stamp.
 * If a node state is specified (state limited tasks) the actions are performed only within this states.
 */
void processScheduler(void) {
    uint16_t now = ParticleAttributes.localTime.numTimePeriodsPassed;

    if (now < ParticleAttributes.scheduler.lastCallToScheduler) {
        // on local time tracking overflowed
        __advancePendingOverflowingCounters();
    }

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
                    task->startAction(task);
                }
                task->isStartActionExecuted = true;
            }
            else if (task->endTimestamp <= now &&
                     true == task->isStartActionExecuted &&
                     false == task->isEndActionExecuted) {
                if (task->endAction != NULL) {
                    task->endAction(task);
                }
                task->isEndActionExecuted = true;
                task->isExecuted = true;
                task->isEnabled = false;
            }
        }
            // on cyclic task, re-schedule next timestamp
        else if (task->isCyclicTask) {
            if (task->startTimestamp <= now) {
                if (task->startAction != NULL) {
                    task->startAction(task);
                }
                task->isExecuted = true;

                uint16_t nextExecution = task->startTimestamp + task->reScheduleDelay;
                if (nextExecution < task->startTimestamp) {
                    // on timestamp overflow: the counter is updated is retained until the local
                    // time has also overflowed
                    task->__isCounterRetained = true;
                } else {
                    task->startTimestamp = nextExecution;
                }
            }
        }
            // on single shot task
        else {
            if (task->startTimestamp <= now && false == task->isStartActionExecuted) {
                if (task->startAction != NULL) {
                    task->startAction(task);
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