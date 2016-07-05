from collections import OrderedDict
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

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=3)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[3] tx-north")
    #
    #
    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=0)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[0] tx-south")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=1)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[1] tx-north")
    #
    #
    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=1)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[2] tx-south")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=2)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[2] tx-north")

    #
    #
    #
    #
    #
    #

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-east", nodeId=3)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[3] tx-east")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=6)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[6] tx-north")
    #
    #
    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=3)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[3] tx-south")


    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=4)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[4] tx-north")
    #
    #
    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=4)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[4] tx-south")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=5)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[5] tx-north")

    #
    #
    #
    #
    #
    #

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=6)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[3] tx-south")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=7)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[4] tx-north")
    #
    #
    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=7)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[4] tx-south")

    txWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-north", nodeId=8)
    dataFilter.filter(txWirefilter)
    xData, yData, annotations = dataFilter.getData(txWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[5] tx-north")


    dataPlotter.setWindowTitle("Network 3x3 Simulation")
    dataFilter.printValues()
    dataPlotter.plot()
