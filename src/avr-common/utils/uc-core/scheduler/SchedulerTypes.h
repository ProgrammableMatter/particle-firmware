/*
 * @author Raoul Rubien 18.09.2016
 *
 * Scheduler state definition.
 */

#pragma once

#include <stdint.h>
#include "uc-core/particle/types/ParticleStateTypes.h"
#include "uc-core/configuration/Scheduler.h"

typedef struct SchedulerTask {
    uint16_t startTimestamp;
    uint16_t endTimestamp;
    StateType state;
    void (*startAction)(void);
    void (*endAction)(void);
    uint8_t isTimeLimited : 1;
    uint8_t isStateLimited : 1;
    uint8_t isEnabled : 1;
    uint8_t isExecuted : 1;
    uint8_t isStarted : 1;
    uint8_t isStartActionExecuted : 1;
    uint8_t isEndActionExecuted : 1;
    uint8_t __pad : 3;
} SchedulerTask;

typedef struct Scheduler {
    SchedulerTask tasks[SCHEDULER_MAX_TASKS];
    uint16_t lastCallToScheduler;

} Scheduler;


