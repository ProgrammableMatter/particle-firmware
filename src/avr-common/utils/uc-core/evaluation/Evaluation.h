/**
 * @author Raoul Rubien 15.10.2016
 *
 * Evaluation related implementation.
 */

#pragma once

#include "uc-core/particle/Globals.h"

void heatWiresTask(SchedulerTask *const task);

#ifdef EVALUATION_ENABLE_FLUCTUATE_CPU_FREQUENCY_ON_PURPOSE

/**
 * Increments/Decrements the current RC oscillator calibration value in between predefined boundaries.
 * Oscillating the MCU frequency is needed for evaluation purpose to prove the (receiver's) clock skew and
 * clock synchronization adjusts automatically to the newly observed (transmitter's) frequency.
 * One call to the function reflects one increment/decrement of the predefined step. The increment/decrement
 * is suspended once when the direction (increment -> decrement, decrement -> increment) is changed.
 */
void switchToNewOsccalValue(void) {
    if (ParticleAttributes.evaluation.oscCalibration.isDecrementing) {
        if (EVALUATION_OSC_CALIBRATION_REGISTER > ParticleAttributes.evaluation.oscCalibration.minOscCal) {
            EVALUATION_OSC_CALIBRATION_REGISTER -= ParticleAttributes.evaluation.oscCalibration.step;
        } else {
            ParticleAttributes.evaluation.oscCalibration.isDecrementing = false;
        }
    } else {
        if (EVALUATION_OSC_CALIBRATION_REGISTER < ParticleAttributes.evaluation.oscCalibration.maxOscCal) {
            EVALUATION_OSC_CALIBRATION_REGISTER += ParticleAttributes.evaluation.oscCalibration.step;
        } else {
            ParticleAttributes.evaluation.oscCalibration.isDecrementing = true;
        }
    }
}

#else
#  define switchToNewOsccalValue()
#endif

/**
 * Updates the task interval according to consumed sync packages in fast sync. mode
 * (switches to slow sync. mode).
 */
static void __updateSendSyncTimePackageTaskInterval(SchedulerTask *const task) {
    if (ParticleAttributes.timeSynchronization.totalFastSyncPackagesToTransmit <= 0) {
        // separation on default synchronization
        task->reScheduleDelay = ParticleAttributes.timeSynchronization.syncPackageSeparation;
    } else {
        // separation on fast synchronization
        task->reScheduleDelay = ParticleAttributes.timeSynchronization.fastSyncPackageSeparation;
        ParticleAttributes.timeSynchronization.totalFastSyncPackagesToTransmit--;
    }
}

void sendSyncTimePackageAndUpdateRequestFlagForInPhaseShiftingEvaluationTask(SchedulerTask *const task) {
    // TODO: shrink code by turning loops inside out
    if (false == task->isLastCall) {
        if (ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled) {
            // on call send next time package
            LED_STATUS2_TOGGLE;
            ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTimeUpdateRequest = true;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
        }
    } else {
        // on last call send package with update flag set and re-enable task
        if (ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled) {
            LED_STATUS2_TOGGLE;
//            TEST_POINT1_TOGGLE;
            ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTimeUpdateRequest = true;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
            switchToNewOsccalValue();
            // re-enable task
            taskEnableCountLimit(SCHEDULER_TASK_ID_SYNC_PACKAGE, 5);
            taskEnable(SCHEDULER_TASK_ID_SYNC_PACKAGE);
        }
    }
}

/**
 * Sends the next sync time package. on last call sends the same package with
 * isNextSyncPackageTimeUpdateRequest flag set.
 */
void sendSyncTimePackageAndUpdateRequestFlagTask(SchedulerTask *const task) {
    // TODO: shrink code by turning loops inside out
    if (false == task->isLastCall) {
        if (ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled) {
            __updateSendSyncTimePackageTaskInterval(task);
            // on call send next time package
            LED_STATUS2_TOGGLE;
            ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTimeUpdateRequest = false;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
        }
    } else {
        // on last call send package with update flag set and re-enable task
        if (ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled) {
            LED_STATUS2_TOGGLE;
            // TEST_POINT1_TOGGLE;
            ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTimeUpdateRequest = true;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
            switchToNewOsccalValue();
            // re-enable task
            taskEnableCountLimit(SCHEDULER_TASK_ID_SYNC_PACKAGE, 10);
            taskEnable(SCHEDULER_TASK_ID_SYNC_PACKAGE);
        }
    }
}

/**
 * Triggers the sending the next synchronization package, not "update time request" flag.
 */
void sendNextSyncTimePackageTask(SchedulerTask *const task) {
    if (false == task->isLastCall) {
        if (ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled) {
            __updateSendSyncTimePackageTaskInterval(task);
            LED_STATUS2_TOGGLE;
            ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTimeUpdateRequest = false;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
        }
    } else {
        switchToNewOsccalValue();
        // on last call enable heat wires task
        addCyclicTask(SCHEDULER_TASK_ID_HEAT_WIRES, heatWiresTask, task->startTimestamp + 400, 200);
        taskEnableNodeTypeLimit(SCHEDULER_TASK_ID_HEAT_WIRES, NODE_TYPE_ORIGIN);
        taskEnable(SCHEDULER_TASK_ID_HEAT_WIRES);
    }
}

#if defined(EVALUATION_SIMPLE_SYNC_AND_ACTUATION) || defined(EVALUATION_SYNC_CYCLICALLY)
static bool __incrementAndSetNextHeatWiresAddress(void) {
    if (ParticleAttributes.evaluation.nextHeatWiresAddress.row <
        ParticleAttributes.protocol.networkGeometry.rows) {
        ParticleAttributes.evaluation.nextHeatWiresAddress.row++;
    } else {
        if (ParticleAttributes.evaluation.nextHeatWiresAddress.column <
            ParticleAttributes.protocol.networkGeometry.columns) {
            ParticleAttributes.evaluation.nextHeatWiresAddress.row = 2;
            ParticleAttributes.evaluation.nextHeatWiresAddress.column++;
        } else {
            return false;
        }
    }
    return true;
}

/**
 * triggers the sending of the next actuation (heat wires) command
 */
void heatWiresTask(SchedulerTask *const task) {
    if (__incrementAndSetNextHeatWiresAddress()) {
        // on having new address to traverse
        Actuators actuators;
        actuators.northLeft = true;
        actuators.northRight = true;
        NodeAddress nodeAddress;
        nodeAddress.row = ParticleAttributes.evaluation.nextHeatWiresAddress.row;
        nodeAddress.column = ParticleAttributes.evaluation.nextHeatWiresAddress.column;
        sendHeatWires(&nodeAddress, &actuators, task->startTimestamp + 50, 100);
    }
    else {
        // on no more address to traverse: enable cyclic sync. pkg.
        ParticleAttributes.evaluation.nextHeatWiresAddress.row = 1;
        ParticleAttributes.evaluation.nextHeatWiresAddress.column = 1;

        addCyclicTask(SCHEDULER_TASK_ID_SYNC_PACKAGE, sendNextSyncTimePackageTask, task->startTimestamp + 100,
                      100);
        taskEnableNodeTypeLimit(SCHEDULER_TASK_ID_SYNC_PACKAGE, NODE_TYPE_ORIGIN);
        taskEnableCountLimit(SCHEDULER_TASK_ID_SYNC_PACKAGE, 20);
        taskEnable(SCHEDULER_TASK_ID_SYNC_PACKAGE);
        taskDisable(SCHEDULER_TASK_ID_HEAT_WIRES);
    }
}
#endif

#ifdef EVALUATION_SYNC_WITH_CYCLIC_UPDATE_TIME_REQUEST_FLAG_THEN_ACTUATE_ONCE

void sendSyncTimeAndActuateOnceTask(SchedulerTask *const task) {

    if (ParticleAttributes.evaluation.totalSentSyncPackages >= EVALUATION_SYNC_PACKAGES_BEFORE_ACTUATION) {
        // disable all scheduled tasks
        for (int idx = 0; idx < SCHEDULER_MAX_TASKS; idx++) {
            ParticleAttributes.scheduler.tasks[idx].isEnabled = false;
        }

        // enable one actuation task
        Actuators actuators;
        actuators.northLeft = false;
        actuators.northRight = true;
        NodeAddress topLeft;
        topLeft.row = 1;
        topLeft.column = 1;
        NodeAddress bottomRight;
        bottomRight.row = ParticleAttributes.protocol.networkGeometry.rows;
        bottomRight.column = ParticleAttributes.protocol.networkGeometry.columns;
        sendHeatWiresRange(&topLeft, &bottomRight, &actuators, task->startTimestamp + 400, 400);

        return;
    }

    if (false == task->isLastCall) {
        if (ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled) {
            // on call send next time package
            LED_STATUS2_TOGGLE;
            ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTimeUpdateRequest = true;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;

            ParticleAttributes.evaluation.totalSentSyncPackages++;
        }
    } else {
        // on last call send package with update flag set and re-enable task
        if (ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled) {
            LED_STATUS2_TOGGLE;
            ParticleAttributes.node.state = STATE_TYPE_RESYNC_NEIGHBOUR;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTimeUpdateRequest = true;
            ParticleAttributes.timeSynchronization.isNextSyncPackageTransmissionEnabled = false;
            ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;

            // re-enable task
            taskEnableCountLimit(SCHEDULER_TASK_ID_SYNC_PACKAGE,
                                 ParticleAttributes.timeSynchronization.totalFastSyncPackagesToTransmit);
            taskEnable(SCHEDULER_TASK_ID_SYNC_PACKAGE);

            ParticleAttributes.evaluation.totalSentSyncPackages++;
        }
    }
}

#endif
