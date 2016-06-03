from collections import OrderedDict
from LogSamplePlot import Plotter as pltr
from LogSamplePlot import Filter as fltr
from Config import Mappings as mappingConfig
from Config import Input as inputConfig

if __name__ == "__main__":
    dataFilter = fltr.Filter(inputConfig.logFile, mappingConfig.wireToFloatValueMapping)
    dataPlotter = pltr.Plotter()

    # transmission wire plot
    transmissionWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=0)
    dataFilter.filter(transmissionWirefilter)
    xData, yData, annotations = dataFilter.getData(transmissionWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "tx-south [0]")

    # reception wire plot
    receptionWireFilter = fltr.SampleFilter(domain="WIRE", name="rx-north", nodeId=1)
    dataFilter.filter(receptionWireFilter)
    xData, yData, annotations = dataFilter.getData(receptionWireFilter)
    dataPlotter.addPlot(xData, yData, annotations, "rx-north [1]")

    # reception events plot
    receptionInterruptValueMapping = {"'U'": 0.0, "'S'": 0.2,
                                      "'A'": 0.6, "'B'": 0.4,
                                      "'0'": 1.2, "'1'": 1.4,
                                      "'R'": 1.0,
                                      }
    dataFilter.setValueMapping(receptionInterruptValueMapping)
    pltr.addPlot(dataFilter, dataPlotter, title="rx[1] - states", nodeId=1, domain="SRAM", name="char-out")

    # node 0 state plot
    nodeStateToFloatValueMapping = OrderedDict(
        [("STATE_TYPE_UNDEFINED", 0),
         ("STATE_TYPE_START", 0),
         ("STATE_TYPE_ACTIVE", 0),
         ("STATE_TYPE_NEIGHBOURS_DISCOVERY", 0),
         ("STATE_TYPE_NEIGHBOURS_DISCOVERED", 0),
         ("STATE_TYPE_DISCOVERY_PULSING", 0),
         ("STATE_TYPE_RESET", 0),
         ("STATE_TYPE_WAIT_FOR_BEING_ENUMERATED", 0),
         ("STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_SEND_ACK_RESPONSE_TO_PARENT", 0),
         ("STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_WAIT_UNTIL_ACK_RESPONSE_TO_PARENT_TRANSMISSION_FINISHED", 0),
         ("STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_ACK_RESPONSE_FROM_PARENT", 0),
         ("STATE_TYPE_LOCALLY_ENUMERATED", 0),
         ("STATE_TYPE_ENUMERATING_NEIGHBOURS", 0),
         ("STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR", 0),
         ("STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_EAST_ENUMERATION_TRANSMISSIONS_FINISHED", 0),
         ("STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_ACK_RESPONSE_FROM_EAST", 0),
         ("STATE_TYPE_ENUMERATING_EAST_SEND_ACK_RESPONSE_TO_EAST", 0),
         ("STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_ACK_RESPONSE_TO_EAST_TRANSMISSION_FINISHED", 0),
         ("STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE", 0),
         ("STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR", 0),
         ("STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_SOUTH_ENUMERATION_TRANSMISSIONS_FINISHED", 0),
         ("STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_FROM_SOUTH", 0),
         ("STATE_TYPE_ENUMERATING_SOUTH_SEND_ACK_RESPONSE_TO_SOUTH", 0),
         ("STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_TO_SOUTH_TRANSMISSION_FINISHED", 0),
         ("SATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE", 0),
         ("STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE", 0),
         ("STATE_TYPE_IDLE", 0), ("STATE_TYPE_INTERPRET_COMMAND", 0),
         ("STATE_TYPE_TX_START", 0),
         ("STATE_TYPE_TX_DONE", 0), ("STATE_TYPE_FORWARD_PKG", 0),
         ("STATE_TYPE_SCHEDULE_COMMAND", 0),
         ("STATE_TYPE_EXECUTE_COMMAND", 0),
         ("STATE_TYPE_COMMAND_SCHEDULED_ACK", 0),
         ("STATE_TYPE_ERRONEOUS", 0),
         ("STATE_TYPE_STALE", 0)])

    idx = 0.0
    for key in nodeStateToFloatValueMapping.keys():
        nodeStateToFloatValueMapping[key] = idx
        idx += 1.0

    dataFilter.setValueMapping(nodeStateToFloatValueMapping)
    nodeStateFilter = fltr.SampleFilter(domain="SRAM", name="globalState.node.state", nodeId=0)
    dataFilter.filter(nodeStateFilter)
    xData, yData, annotations = dataFilter.getData(nodeStateFilter)
    dataPlotter.addPlot(xData, yData, annotations, "node [0] states")

    #
    dataFilter.setValueMapping(nodeStateToFloatValueMapping)
    nodeStateFilter = fltr.SampleFilter(domain="SRAM", name="globalState.node.state", nodeId=1)
    dataFilter.filter(nodeStateFilter)
    xData, yData, annotations = dataFilter.getData(nodeStateFilter)
    dataPlotter.addPlot(xData, yData, annotations, "node [1] states")

    interpreterCharOutToHumanReadableAnnotation = {"'I'": "call", "'i'": "ret",
                                                   "'c'": "clr buff",
                                                   "'E'": "enum pck recv",
                                                   "'e'": "enum pck recv: err",
                                                   "'K'": "enum wait ack parent: ack",
                                                   "'k'": "xxx",
                                                   "'W'": "enum wait ack parent: enum",
                                                   "'w'": "enum wait ack parent: err",
                                                   "'T'": "enum east wait ack: ack+dta",
                                                   "'t'": "enum east wait ack: err",
                                                   }

    rxCharOutToFloatValues = {"'I'": 0.2, "'i'": 0.1,
                              "'c'": 0.3,
                              "'E'": 0.4, "'e'": 0.5,
                              "'K'": 0.6, "'k'": 0.7,
                              "'W'": 0.9, "'w'": 1.0,
                              "'T'": 1.2, "'t'": 1.4,
                              }
    dataFilter.setValueMapping(rxCharOutToFloatValues)
    nodeCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=1)
    dataFilter.removeSamples(nodeCharOutFilter)
    dataFilter.filter(nodeCharOutFilter)
    xData, yData, annotations = dataFilter.getData(nodeCharOutFilter)
    annotations = pltr.reMapAnnotation(annotations, interpreterCharOutToHumanReadableAnnotation)
    dataPlotter.addPlot(xData, yData, annotations, "node [1] interpreter states")

    # rx north sync plot
    rxCharOutToHumanReadableAnnotation = {"'U'": "rx timeouted", "'S'": "rx active"}
    rxCharOutToFloatValues = {"'U'": 0.0, "'S'": 1.0, }
    dataFilter.setValueMapping(rxCharOutToFloatValues)
    nodeCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=1)
    dataFilter.removeSamples(nodeCharOutFilter)
    dataFilter.filter(nodeCharOutFilter)
    xData, yData, annotations = dataFilter.getData(nodeCharOutFilter)
    annotations = pltr.reMapAnnotation(annotations, rxCharOutToHumanReadableAnnotation)
    dataPlotter.addPlot(xData, yData, annotations, "node [1] is rx north sync.")

    # SRAM[globalState.ports.xmissonState] < - (STATE_TYPE_XMISSION_TYPE_DISABLED_TX_RX)
    xmissionStatesToFloatValues = {
        "STATE_TYPE_XMISSION_TYPE_ENABLED_TX": 1.0,
        "STATE_TYPE_XMISSION_TYPE_ENABLED_RX": 2.0,
        "STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX": 3.0,
        "STATE_TYPE_XMISSION_TYPE_DISABLED_TX_RX": 0.0,
    }
    dataFilter.setValueMapping(rxCharOutToFloatValues)
    nodeXmissionStatesFilter = fltr.SampleFilter(domain="SRAM", name="globalState.ports.xmissonState", nodeId=1)
    # dataFilter.removeSamples(nodeXmissionStatesFilter  utFilter)
    dataFilter.setValueMapping(xmissionStatesToFloatValues)
    dataFilter.filter(nodeXmissionStatesFilter)
    xData, yData, annotations = dataFilter.getData(nodeXmissionStatesFilter)
    # annotations = pltr.reMapAnnotation(annotations, rxCharOutToHumanReadableAnnotation)
    dataPlotter.addPlot(xData, yData, annotations, "node [1] xmissoin")

    # plot tx/rx counter interrupts
    dataFilter.setValueMapping(mappingConfig.interruptToFloatValueMapping)
    pltr.addInterruptPlot(dataFilter, dataPlotter, title="call/return [0]", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="invoke",
                          interruptName="TX_RX_TIMER_TOP")

    pltr.addInterruptPlot(dataFilter, dataPlotter, title="call/return [0]", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="invoke",
                          interruptName="TX_RX_TIMER_CENTER")

    dataPlotter.setWindowTitle("Network 2x1 Simulation")
    dataFilter.printValues()
    dataPlotter.plot()
