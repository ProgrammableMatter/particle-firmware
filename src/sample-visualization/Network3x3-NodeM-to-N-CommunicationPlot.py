from collections import OrderedDict
from LogSamplePlot import Plotter as pltr
from LogSamplePlot import Filter as fltr
from Config import Mappings as mappingConfig
from Config import Input as inputConfig

if __name__ == "__main__":
    dataFilter = fltr.Filter(inputConfig.logFile, mappingConfig.wireToFloatValueMapping)
    dataPlotter = pltr.Plotter()

    numRows = 3
    numColumns = 3
    nodesTotal = numRows * numColumns

    # column 1

    filter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=0)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=1)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=1)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=2)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    # column 2

    filter = fltr.SampleFilter(domain="WIRE", name="tx-east", nodeId=0)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=3)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=3)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=4)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=4)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=5)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    # column 3

    filter = fltr.SampleFilter(domain="WIRE", name="tx-east", nodeId=5)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=6)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=6)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=7)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=7)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=8)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=8)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    filter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=9)
    dataFilter.filter(filter)
    xData, yData, annotations = dataFilter.getData(filter)
    dataPlotter.addPlot(xData, yData, annotations, "[%s] %s" % (filter.nodeId, filter.name))

    dataPlotter.setWindowTitle("Network %sx%s Simulation" % (numRows, numColumns))
    dataFilter.printValues()
    dataPlotter.plot()
