/**
 * @author Raoul Rubien  13.07.16.
 */

#pragma once

extern FUNC_ATTRS void setNewNetworkGeometry(void);
/**
 * Transmits a new network geometry to the network. Particles outside the new boundary
 * switch to sleep mode.
 *
 * @pre:
 * + the network must be in broadcast mode
 * + the ParticleAttributes.protocol.networkGeometry.rows/cols are set accordingly
 */
FUNC_ATTRS void setNewNetworkGeometry(void) {
    // TODO refactoring necessary
    clearTransmissionPortBuffer(ParticleAttributes.communication.ports.tx.simultaneous);
    setInitiatorStateStart(&ParticleAttributes.protocol.ports.east);
    ParticleAttributes.protocol.isSimultaneousTransmissionEnabled = true;
    ParticleAttributes.node.state = STATE_TYPE_SEND_SET_NETWORK_GEOMETRY;
}

extern FUNC_ATTRS void scheduleActiationCommand(uint16_t startPeriodTimeStamp, uint16_t endPeriodTimeStamp);
/**
 * Schedules an actuation command starting and ending at the specified time stamps.
 */
FUNC_ATTRS void scheduleActiationCommand(uint16_t startPeriodTimeStamp, uint16_t endPeriodTimeStamp) {
    ParticleAttributes.actuationCommand.actuationStart.periodTimeStamp = startPeriodTimeStamp;
    ParticleAttributes.actuationCommand.actuationEnd.periodTimeStamp = endPeriodTimeStamp;
    ParticleAttributes.actuationCommand.isScheduled = true;
}
