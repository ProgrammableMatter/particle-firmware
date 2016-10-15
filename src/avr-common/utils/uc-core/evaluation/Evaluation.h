/**
 * @author Raoul Rubien 15.10.2016
 *
 * Evaluation related implementation.
 */

#pragma once

#include "uc-core/particle/Globals.h"
//#include "EvaluationTypes.h"

void sendNextSyncTimePackage(SchedulerTask *const task) {
    LED_STATUS2_TOGGLE;
    if (ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled) {
        if (ParticleAttributes.timeSynchronization.totalFastSyncPackagesToTransmit <= 0) {
            // separation on default synchronization
            task->reScheduleDelay = ParticleAttributes.timeSynchronization.syncPackageSeparation;
        } else {
            // separation on fast synchronization
            task->reScheduleDelay = ParticleAttributes.timeSynchronization.fastSyncPackageSeparation;
            ParticleAttributes.timeSynchronization.totalFastSyncPackagesToTransmit--;
        }
        ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
        ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled = false;
    }

    if (task->isLastCall) {
        // on last call enable heat wires task
        ParticleAttributes.scheduler.tasks[SCHEDULER_TASK_ID_HEAT_WIRES].startTimestamp =
                task->startTimestamp + 500;
        enableTask(SCHEDULER_TASK_ID_HEAT_WIRES);
    }
}

static bool __incrementNextHeatWiresAddress(void) {
    if (ParticleAttributes.evaluation.nextHeatWiresAddress.row <
        ParticleAttributes.protocol.networkGeometry.rows) {
        ParticleAttributes.evaluation.nextHeatWiresAddress.row++;
    } else {
        if (ParticleAttributes.evaluation.nextHeatWiresAddress.column <
            ParticleAttributes.protocol.networkGeometry.columns) {
            ParticleAttributes.evaluation.nextHeatWiresAddress.row = 0;
            ParticleAttributes.evaluation.nextHeatWiresAddress.column++;
        } else {
            return false;
        }
    }
    return true;
}

void heatWires(SchedulerTask *const task) {
    if (__incrementNextHeatWiresAddress()) {
        Actuators actuators;
        actuators.northLeft = true;
        actuators.northRight = true;
        NodeAddress nodeAddress;
        nodeAddress.row = 3;
        nodeAddress.column = 1;
        sendHeatWires(&nodeAddress, &actuators, task->startTimestamp + 500, 500);
        task->isEnabled = false;

        // re-enable send sync pkg. task
        ParticleAttributes.scheduler.tasks[SCHEDULER_TASK_ID_SYNC_PACKAGE].startTimestamp =
                task->startTimestamp + 1500;
        setCountLimitedTask(SCHEDULER_TASK_ID_SYNC_PACKAGE, 20);
        enableTask(SCHEDULER_TASK_ID_SYNC_PACKAGE);
    } else {
        ParticleAttributes.scheduler.tasks[SCHEDULER_TASK_ID_SYNC_PACKAGE].startTimestamp =
                task->startTimestamp + 500;
        ParticleAttributes.scheduler.tasks[SCHEDULER_TASK_ID_SYNC_PACKAGE].isCountLimitedTask = false;
        disableTask(SCHEDULER_TASK_ID_HEAT_WIRES);
    }
}
