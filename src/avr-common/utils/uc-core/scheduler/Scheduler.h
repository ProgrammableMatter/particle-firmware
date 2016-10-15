/**
 * @author Raoul Rubien 11.10.2016
 *
 * Scheduler related implementation.
 */

#pragma once

#include "Scheduler.h"
#include "uc-core/particle/Globals.h"
#include "common/common.h"
#include "uc-core/particle/types/ParticleStateTypes.h"


void setNodeTypeLimitedTask(uint8_t taskId, NodeType nodeType) {
    SchedulerTask *task = &ParticleAttributes.scheduler.tasks[taskId];
    task->isNodeTypeLimited = true;
    task->nodeType = nodeType;
}

void setStateTypeLimtedTask(uint8_t taskId, StateType stateType) {
    SchedulerTask *task = &ParticleAttributes.scheduler.tasks[taskId];
    task->isStateLimited = true;
    task->state = stateType;
}

void setCountLimitedTask(uint8_t taskId, uint16_t numCalls) {
    SchedulerTask *task = &ParticleAttributes.scheduler.tasks[taskId];
    task->isCountLimitedTask = true;
    task->numCalls = numCalls;
}

void disableTask(uint8_t taskId) {
    ParticleAttributes.scheduler.tasks[taskId].isEnabled = false;
}

void enableTask(uint8_t taskId) {
    ParticleAttributes.scheduler.tasks[taskId].isEnabled = true;
}


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

static void __onCountLimitedTaskDecrementCounter(SchedulerTask *const task) {
    if (task->isCountLimitedTask) {
        --task->numCalls;
        if (task->numCalls <= 0) {
            task->isLastCall = true;
        }
    }
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

        // on node type limited task
        if (task->isNodeTypeLimited) {
            if (task->nodeType != ParticleAttributes.node.type) {
                continue;
            }
        }

        // on count limited task
        if (task->isCountLimitedTask) {
            if (task->numCalls <= 0) {
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
                __onCountLimitedTaskDecrementCounter(task);
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
                __onCountLimitedTaskDecrementCounter(task);
                task->startAction(task);
                task->isStartActionExecuted = true;
                task->isExecuted = true;
                task->isEnabled = false;
            }
        }
    }

    ParticleAttributes.scheduler.lastCallToScheduler = now;
}
