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
 * Adds a single shot task to the scheduler.
 * @param taskId the id in the scheduler's array
 * @param task function pointer to execute
 * @param timestamp the 1st (desired) execution timestamp
 */
void addSingleShotTask(const uint8_t taskId, void (*const action)(SchedulerTask *const task),
                       const uint16_t timestamp) {
    SchedulerTask *task = &ParticleAttributes.scheduler.tasks[taskId];
    constructSchedulerTask(task);
    task->startAction = action;
    task->startTimestamp = timestamp;
    task->isEnabled = true;
}

/**
 * Adds a cyclic task to the scheduler.
 * @param taskId the id in the scheduler's array
 * @param task function pointer to execute
 * @param timestamp the 1st (desired) execution timestamp
 * @param separation the delay until the subsequent execution, must be < than UINT16_MAX
 */
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
 * Assures a task's start action is executed once asap after the desired start timestamp
 * and (on time limited tasks) the task's end action executed once after the end time stamp.
 * If a node state is specified (state limited tasks) the actions are performed only within this states.
 * TODO: cyclic task
 */
void processScheduler(void) {
    uint8_t sreg = SREG;
    MEMORY_BARRIER;
    CLI;
    MEMORY_BARRIER;
    uint16_t const now = ParticleAttributes.localTime.numTimePeriodsPassed;
    MEMORY_BARRIER;
    SREG = sreg;


    for (uint8_t idx = 0; idx < SCHEDULER_MAX_TASKS; idx++) {
        SchedulerTask *task = &ParticleAttributes.scheduler.tasks[idx];

        if (now < ParticleAttributes.scheduler.lastCallToScheduler) {
            // on local time tracking overflowed, unlock tasks kept back
            task->__isExecutionRetained = false;
        }

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
                task->startAction(task);
                task->isStartActionExecuted = true;
            }
            else if (task->endTimestamp <= now &&
                     true == task->isStartActionExecuted &&
                     false == task->isEndActionExecuted) {
                task->endAction(task);
                task->isEndActionExecuted = true;
                task->isExecuted = true;
                task->isEnabled = false;
            }
        }
            // on cyclic task, re-schedule next timestamp
        else if (task->isCyclicTask) {
            if (task->startTimestamp <= now && task->__isExecutionRetained == false) {
                task->startAction(task);
                task->isExecuted = true;

                uint16_t nextExecution = now + task->reScheduleDelay;
                if (nextExecution < now) {
                    // on timestamp overflow: the counter is updated is retained until the local
                    // time has also overflowed
                    task->__isExecutionRetained = true;
                }
                task->startTimestamp = nextExecution;
            }
        }
            // on single shot task
        else {
            if (task->startTimestamp <= now && false == task->isStartActionExecuted) {
                task->startAction(task);
                task->isStartActionExecuted = true;
                task->isExecuted = true;
                task->isEnabled = false;
                LED_STATUS1_ON;
            }
        }
    }

    ParticleAttributes.scheduler.lastCallToScheduler = now;
}