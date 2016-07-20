from collections import OrderedDict

from Config.Mappings import charOutToHumanReadableAnnotation
from LogSamplePlot import Plotter as pltr
from LogSamplePlot import Filter as fltr
from Config import Mappings as mappingConfig
from Config import Input as inputConfig

if __name__ == "__main__":
    dataFilter = fltr.Filter(inputConfig.logFile, mappingConfig.wireToFloatValueMapping)
    dataPlotter = pltr.Plotter()

    numRows = 1
    numColumns = 2
    nodesTotal = numRows * numColumns

    # column 1 to 2
    filter = fltr.SampleFilter(domain="WIRE", name="tx-east", nodeId=0)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    # node 1 reception wire plot
    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=1)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    # char out plots

    for id in range(0, nodesTotal):
        filter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=id)
        dataFilter.removeSamples(filter)
        dataFilter.filter(filter)
        xData, yData, annotations = dataFilter.getData(filter)
        annotations = pltr.reMapAnnotation(annotations, charOutToHumanReadableAnnotation)
        dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    # int16 out plots

    for id in range(0, nodesTotal):
        filter = fltr.SampleFilter(domain="SRAM", name="int16-out", nodeId=id)
        dataFilter.removeSamples(filter)
        dataFilter.filter(filter)
        xData, yData, annotations = dataFilter.getData(filter)
        dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    # node state plots

    for id in range(0, nodesTotal):
        filter = fltr.SampleFilter(domain="SRAM", name="Particle.node.state", nodeId=id)
        dataFilter.filter(filter)
        xData, yData, annotations = dataFilter.getData(filter)
        dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    dataPlotter.setWindowTitle("Network %sx%s Simulation" % (numRows, numColumns))
    dataFilter.printValues()
    dataPlotter.plot()
