from collections import OrderedDict

from Config.Mappings import charOutToHumanReadableAnnotation
from LogSamplePlot import Plotter as pltr
from LogSamplePlot import Filter as fltr
from Config import Mappings as mappingConfig
from Config import Input as inputConfig

if __name__ == "__main__":
    dataFilter = fltr.Filter(inputConfig.logFile, mappingConfig.wireToFloatValueMapping)
    dataPlotter = pltr.Plotter()

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-east", nodeId=0)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[0] tx-east")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=2)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[2] tx-north")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=0)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[0] tx-south")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=1)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[1] tx-north")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=2)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[2] tx-south")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=3)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[3] tx-north")

    # reception events plot
    receptionInterruptValueMapping = {}
    counter = 0
    for key in charOutToHumanReadableAnnotation:
        receptionInterruptValueMapping[key] = counter
        counter += 1

    receptionInterruptValueMapping["'0'"] = 30
    receptionInterruptValueMapping["'1'"] = 35
    receptionInterruptValueMapping["'|'"] = 40
    receptionInterruptValueMapping["'+'"] = 45
    receptionInterruptValueMapping["'x'"] = 50
    receptionInterruptValueMapping["'x'"] = 55

    dataFilter.setValueMapping(receptionInterruptValueMapping)

    nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=0)
    dataFilter.removeSamples(nodeTxCharOutFilter)
    dataFilter.filter(nodeTxCharOutFilter)
    xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    annotations = pltr.reMapAnnotation(annotations, charOutToHumanReadableAnnotation)
    dataPlotter.addPlot(xData, yData, annotations, "[0] char-out")

    nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=1)
    dataFilter.removeSamples(nodeTxCharOutFilter)
    dataFilter.filter(nodeTxCharOutFilter)
    xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    annotations = pltr.reMapAnnotation(annotations, charOutToHumanReadableAnnotation)
    dataPlotter.addPlot(xData, yData, annotations, "[1] char-out")

    nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=2)
    dataFilter.removeSamples(nodeTxCharOutFilter)
    dataFilter.filter(nodeTxCharOutFilter)
    xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    annotations = pltr.reMapAnnotation(annotations, charOutToHumanReadableAnnotation)
    dataPlotter.addPlot(xData, yData, annotations, "[2] char-out")

    nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=3)
    dataFilter.removeSamples(nodeTxCharOutFilter)
    dataFilter.filter(nodeTxCharOutFilter)
    xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    annotations = pltr.reMapAnnotation(annotations, charOutToHumanReadableAnnotation)
    dataPlotter.addPlot(xData, yData, annotations, "[3] char-out")

    nodeId = 1
    register = "Particle.communication.ports.rx.north.snapshotsBuffer.decoderStates.decodingState"
    decoderFilter = fltr.SampleFilter(domain="SRAM", name=register, nodeId=nodeId)
    dataFilter.removeSamples(decoderFilter)
    dataFilter.filter(decoderFilter)
    xData, yData, annotations = dataFilter.getData(decoderFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (decoderFilter.nodeId, register))

    nodeId = 2
    register = "Particle.communication.ports.rx.north.snapshotsBuffer.decoderStates.decodingState"
    decoderFilter = fltr.SampleFilter(domain="SRAM", name=register, nodeId=nodeId)
    dataFilter.removeSamples(decoderFilter)
    dataFilter.filter(decoderFilter)
    xData, yData, annotations = dataFilter.getData(decoderFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (decoderFilter.nodeId, register))

    nodeId = 3
    register = "Particle.communication.ports.rx.north.snapshotsBuffer.decoderStates.decodingState"
    decoderFilter = fltr.SampleFilter(domain="SRAM", name=register, nodeId=nodeId)
    dataFilter.removeSamples(decoderFilter)
    dataFilter.filter(decoderFilter)
    xData, yData, annotations = dataFilter.getData(decoderFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (decoderFilter.nodeId, register))

    #
    #
    #
    #
    #
    #

    sramFilter = fltr.SampleFilter(domain="SRAM", name="Particle.node.state", nodeId=0)
    dataFilter.filter(sramFilter)
    xData, yData, annotations = dataFilter.getData(sramFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] Particle.node.state" % sramFilter.nodeId)

    sramFilter = fltr.SampleFilter(domain="SRAM", name="Particle.node.state", nodeId=1)
    dataFilter.filter(sramFilter)
    xData, yData, annotations = dataFilter.getData(sramFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] Particle.node.state" % sramFilter.nodeId)

    sramFilter = fltr.SampleFilter(domain="SRAM", name="Particle.node.state", nodeId=2)
    dataFilter.filter(sramFilter)
    xData, yData, annotations = dataFilter.getData(sramFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] Particle.node.state" % sramFilter.nodeId)

    sramFilter = fltr.SampleFilter(domain="SRAM", name="Particle.node.state", nodeId=3)
    dataFilter.filter(sramFilter)
    xData, yData, annotations = dataFilter.getData(sramFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] Particle.node.state" % sramFilter.nodeId)

    dataPlotter.setWindowTitle("Network 2x2 Simulation")
    dataFilter.printValues()
    dataPlotter.plot()
