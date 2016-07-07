//
// Created by Raoul Rubien on 25.05.16.
//

#pragma once

#include "CommunicationProtocolTypes.h"
#include "CommunicationProtocolPackageCtors.h"


extern FUNC_ATTRS void executeSetLocalTime(volatile TimePackage *package);
/**
 * Updates the local time.
 */
FUNC_ATTRS void executeSetLocalTime(volatile TimePackage *package) {
    // TODO: compensation value missing
    TIMER_TX_RX_COUNTER_VALUE = package->time;
    ParticleAttributes.protocol.isBoradcastEnabled = package->enableBroadcast;
}


extern FUNC_ATTRS void executeSetLocalAddress(volatile EnumerationPackage *package);
/**
 * Updates the local address.
 */
FUNC_ATTRS void executeSetLocalAddress(volatile EnumerationPackage *package) {
    ParticleAttributes.node.address.row = package->addressRow0;
    ParticleAttributes.node.address.column = package->addressColumn0;
    ParticleAttributes.protocol.hasNetworkGeometryDiscoveryBreadCrumb = package->hasNetworkGeometryDiscoveryBreadCrumb;
}


extern FUNC_ATTRS void executeRelayAnnounceNetworkGeometryPackage(
        volatile AnnounceNetworkGeometryPackage *package);
/**
 * If current node is the origin consume the package otherwise put the particle into relaying package state.
 */
FUNC_ATTRS void executeRelayAnnounceNetworkGeometryPackage(volatile AnnounceNetworkGeometryPackage *package) {

    if (ParticleAttributes.node.type == NODE_TYPE_ORIGIN) {
        ParticleAttributes.protocol.networkGeometry.rows = package->rows;
        ParticleAttributes.protocol.networkGeometry.columns = package->columns;
    } else {
        clearTransmissionPortBuffer(&ParticleAttributes.communication.ports.tx.north);
        constructSendAnnounceNetworkGeometryPackage(package->rows, package->columns);
        setInitiatorStateStart(&ParticleAttributes.protocol.ports.north);
        ParticleAttributes.node.state = STATE_TYPE_ANNOUNCE_NETWORK_GEOMETRY_RELAY;
    }
    ParticleAttributes.protocol.isBoradcastEnabled = package->enableBroadcast;
}