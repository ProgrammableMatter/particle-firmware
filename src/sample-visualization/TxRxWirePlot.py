from collections import OrderedDict
from LogSamplePlot import Plotter as pltr
from LogSamplePlot import Filter as fltr
from Config import Mappings as mappingConfig
from Config import Input as inputConfig

if __name__ == "__main__":
    dataFilter = fltr.Filter(inputConfig.logFile, mappingConfig.wireToFloatValueMapping)
    dataPlotter = pltr.Plotter()

    # node 0 transmission wire plot
    transmissionWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=0)
    dataFilter.filter(transmissionWirefilter)
    xData, yData, annotations = dataFilter.getData(transmissionWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[0] tx-south")

    # node 1 reception wire plot
    receptionWireFilter = fltr.SampleFilter(domain="WIRE", name="rx-north", nodeId=1)
    dataFilter.filter(receptionWireFilter)
    xData, yData, annotations = dataFilter.getData(receptionWireFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[1] rx-north")

    # node 0 reception wire plot
    receptionWireFilter = fltr.SampleFilter(domain="WIRE", name="rx-south", nodeId=0)
    dataFilter.filter(receptionWireFilter)
    xData, yData, annotations = dataFilter.getData(receptionWireFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[0] rx-south")

    # node 1 transmission wire plot
    transmissionWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=1)
    dataFilter.filter(transmissionWirefilter)
    xData, yData, annotations = dataFilter.getData(transmissionWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[1] tx-north")

    # reception events plot
    receptionInterruptValueMapping = {"'V'": 0.0, "'S'": 0.2,
                                      "'A'": 0.6, "'B'": 0.4,
                                      "'0'": 1.2, "'1'": 1.4,
                                      "'R'": 1.0, "'F'": 5,
                                      }
    dataFilter.setValueMapping(receptionInterruptValueMapping)
    pltr.addPlot(dataFilter, dataPlotter, title="[0] rx states", nodeId=0, domain="SRAM", name="char-out")

    receptionInterruptValueMapping = {"'U'": 0.0, "'S'": 0.2,
                                      "'A'": 0.6, "'B'": 0.4,
                                      "'0'": 1.2, "'1'": 1.4,
                                      "'R'": 1.0, "'F'": 5,
                                      }
    dataFilter.setValueMapping(receptionInterruptValueMapping)
    pltr.addPlot(dataFilter, dataPlotter, title="[1] rx states", nodeId=1, domain="SRAM", name="char-out")

    # # node 0 state plot
    # nodeStateToFloatValueMapping = OrderedDict(
    #     [("STATE_TYPE_UNDEFINED", 0),
    #      ("STATE_TYPE_START", 0),
    #      ("STATE_TYPE_ACTIVE", 0),
    #      ("STATE_TYPE_NEIGHBOURS_DISCOVERY", 0),
    #      ("STATE_TYPE_NEIGHBOURS_DISCOVERED", 0),
    #      ("STATE_TYPE_DISCOVERY_PULSING", 0),
    #      ("STATE_TYPE_RESET", 0),
    #      ("STATE_TYPE_WAIT_FOR_BEING_ENUMERATED", 0),
    #      ("STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_SEND_ACK_RESPONSE_TO_PARENT", 0),
    #      ("STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_WAIT_UNTIL_ACK_RESPONSE_TO_PARENT_TRANSMISSION_FINISHED", 0),
    #      ("STATE_TYPE_WAIT_FOR_BEING_ENUMERATED_ACK_RESPONSE_FROM_PARENT", 0),
    #      ("STATE_TYPE_LOCALLY_ENUMERATED", 0),
    #      ("STATE_TYPE_ENUMERATING_NEIGHBOURS", 0),
    #      ("STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR", 0),
    #      ("STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_EAST_ENUMERATION_TRANSMISSIONS_FINISHED", 0),
    #      ("STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_ACK_RESPONSE_FROM_EAST", 0),
    #      ("STATE_TYPE_ENUMERATING_EAST_SEND_ACK_RESPONSE_TO_EAST", 0),
    #      ("STATE_TYPE_ENUMERATING_EAST_WAIT_UNTIL_ACK_RESPONSE_TO_EAST_TRANSMISSION_FINISHED", 0),
    #      ("STATE_TYPE_ENUMERATING_EAST_NEIGHBOUR_DONE", 0),
    #      ("STATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR", 0),
    #      ("STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_SOUTH_ENUMERATION_TRANSMISSIONS_FINISHED", 0),
    #      ("STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_FROM_SOUTH", 0),
    #      ("STATE_TYPE_ENUMERATING_SOUTH_SEND_ACK_RESPONSE_TO_SOUTH", 0),
    #      ("STATE_TYPE_ENUMERATING_SOUTH_WAIT_UNTIL_ACK_RESPONSE_TO_SOUTH_TRANSMISSION_FINISHED", 0),
    #      ("SATE_TYPE_ENUMERATING_SOUTH_NEIGHBOUR_DONE", 0),
    #      ("STATE_TYPE_ENUMERATING_NEIGHBOURS_DONE", 0),
    #      ("STATE_TYPE_IDLE", 0), ("STATE_TYPE_INTERPRET_COMMAND", 0),
    #      ("STATE_TYPE_TX_START", 0),
    #      ("STATE_TYPE_TX_DONE", 0), ("STATE_TYPE_FORWARD_PKG", 0),
    #      ("STATE_TYPE_SCHEDULE_COMMAND", 0),
    #      ("STATE_TYPE_EXECUTE_COMMAND", 0),
    #      ("STATE_TYPE_COMMAND_SCHEDULED_ACK", 0),
    #      ("STATE_TYPE_ERRONEOUS", 0),
    #      ("STATE_TYPE_STALE", 0)])
    #
    # idx = 0.0
    # for key in nodeStateToFloatValueMapping.keys():
    #     nodeStateToFloatValueMapping[key] = idx
    #     idx += 1.0
    #
    # dataFilter.setValueMapping(nodeStateToFloatValueMapping)
    # nodeStateFilter = fltr.SampleFilter(domain="SRAM", name="globalState.node.state", nodeId=0)
    # dataFilter.filter(nodeStateFilter)
    # xData, yData, annotations = dataFilter.getData(nodeStateFilter)
    # dataPlotter.addPlot(xData, yData, annotations, "[0] node states")
    #
    # #
    # dataFilter.setValueMapping(nodeStateToFloatValueMapping)
    # nodeStateFilter = fltr.SampleFilter(domain="SRAM", name="globalState.node.state", nodeId=1)
    # dataFilter.filter(nodeStateFilter)
    # xData, yData, annotations = dataFilter.getData(nodeStateFilter)
    # dataPlotter.addPlot(xData, yData, annotations, "[1] node states")
    #
    # interpreterCharOutToHumanReadableAnnotation = {"'I'": "call", "'i'": "ret",
    #                                                "'c'": "clr buff",
    #                                                "'E'": "enum pck recv",
    #                                                "'e'": "enum pck recv: err",
    #                                                "'K'": "enum wait ack parent: ack",
    #                                                "'k'": "xxx",
    #                                                "'W'": "enum wait ack parent: enum",
    #                                                "'w'": "enum wait ack parent: err",
    #                                                "'T'": "enum east wait ack: ack+dta",
    #                                                "'t'": "enum east wait ack: err",
    #                                                }
    #
    # txCharOutToFloatValues = {"'I'": 0.2, "'i'": 0.1,
    #                           "'c'": 0.3,
    #                           "'E'": 0.4, "'e'": 0.5,
    #                           "'K'": 0.6, "'k'": 0.7,
    #                           "'W'": 0.9, "'w'": 1.0,
    #                           "'T'": 1.2, "'t'": 1.4,
    #                           }
    # dataFilter.setValueMapping(txCharOutToFloatValues)
    # nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=1)
    # dataFilter.removeSamples(nodeTxCharOutFilter)
    # dataFilter.filter(nodeTxCharOutFilter)
    # xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    # annotations = pltr.reMapAnnotation(annotations, interpreterCharOutToHumanReadableAnnotation)
    # dataPlotter.addPlot(xData, yData, annotations, "[1] interpreter states")
    #
    # nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=0)
    # dataFilter.removeSamples(nodeTxCharOutFilter)
    # dataFilter.filter(nodeTxCharOutFilter)
    # # xxx
    # xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    # dataPlotter.addPlot(xData, yData, annotations, "[0] interpreter states")
    #
    # # rx north sync plot
    # txCharOutToHumanReadableAnnotation = {"'U'": "rx timeouted", "'S'": "rx active"}
    # txCharOutToFloatValues = {"'U'": 0.0, "'S'": 1.0,}
    # dataFilter.setValueMapping(txCharOutToFloatValues)
    # nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=1)
    # dataFilter.removeSamples(nodeTxCharOutFilter)
    # dataFilter.filter(nodeTxCharOutFilter)
    # xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    # annotations = pltr.reMapAnnotation(annotations, txCharOutToHumanReadableAnnotation)
    # dataPlotter.addPlot(xData, yData, annotations, "node [1] is rx north sync.")
    #
    # # xmission states plots
    # xmissionStatesToFloatValues = {
    #     "STATE_TYPE_XMISSION_TYPE_ENABLED_TX": 1.0,
    #     "STATE_TYPE_XMISSION_TYPE_ENABLED_RX": 2.0,
    #     "STATE_TYPE_XMISSION_TYPE_ENABLED_TX_RX": 3.0,
    #     "STATE_TYPE_XMISSION_TYPE_DISABLED_TX_RX": 0.0,
    # }
    # dataFilter.setValueMapping(txCharOutToFloatValues)
    # nodeXmissionStatesFilter = fltr.SampleFilter(domain="SRAM", name="globalState.ports.xmissonState", nodeId=0)
    # dataFilter.setValueMapping(xmissionStatesToFloatValues)
    # dataFilter.removeSamples(nodeXmissionStatesFilter)
    # dataFilter.filter(nodeXmissionStatesFilter)
    # xData, yData, annotations = dataFilter.getData(nodeXmissionStatesFilter)
    # dataPlotter.addPlot(xData, yData, annotations, "[0] xmission states")
    #
    # dataFilter.setValueMapping(txCharOutToFloatValues)
    # nodeXmissionStatesFilter = fltr.SampleFilter(domain="SRAM", name="globalState.ports.xmissonState", nodeId=1)
    # dataFilter.removeSamples(nodeXmissionStatesFilter)
    # dataFilter.setValueMapping(xmissionStatesToFloatValues)
    # dataFilter.filter(nodeXmissionStatesFilter)
    # xData, yData, annotations = dataFilter.getData(nodeXmissionStatesFilter)
    # dataPlotter.addPlot(xData, yData, annotations, "[1] xmission states")
    #
    #
    # # plot tx/rx counter interrupts
    # dataFilter.setValueMapping(mappingConfig.interruptToFloatValueMapping)
    # pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] tx/rx counter top call/return", nodeId=0,
    #                       interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="invoke",
    #                       interruptName="TX_RX_TIMER_TOP")
    #
    # pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] tx/rx counter center call/return", nodeId=0,
    #                       interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="invoke",
    #                       interruptName="TX_RX_TIMER_CENTER")
    #
    # pltr.addPlot(dataFilter, dataPlotter, title="SRAM[int16-out]", nodeId=1, domain="SRAM", name="int16-out")
    #
    # # tx signal generation plot
    # txCharOutToHumanReadableAnnotation = {"'G'": "rectification", "'g'": "rectification", "'H'": "0", "'h'": "1",}
    # txCharOutToFloatValues = {"'G'": 0.5, "'g'": 0.0, "'H'": 1.0, "'h'": 2.0}
    # dataFilter.setValueMapping(txCharOutToFloatValues)
    # nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=0)
    # dataFilter.removeSamples(nodeTxCharOutFilter)
    # dataFilter.filter(nodeTxCharOutFilter)
    # xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    # annotations = pltr.reMapAnnotation(annotations, txCharOutToHumanReadableAnnotation)
    # dataPlotter.addPlot(xData, yData, annotations, "node [0] signal generation")

    dataPlotter.setWindowTitle("Tx-/Rx-Wire Plot")
    dataFilter.printValues()
    dataPlotter.plot()
