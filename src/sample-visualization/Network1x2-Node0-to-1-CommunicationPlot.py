from collections import OrderedDict

from Config.Mappings import charOutToHumanReadableAnnotation
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

    nodeFilter = fltr.SampleFilter(domain="SRAM", name="int16-out", nodeId=0)
    dataFilter.removeSamples(nodeFilter)
    dataFilter.filter(nodeFilter)
    xData, yData, annotations = dataFilter.getData(nodeFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[0] int16-out")


    nodeTxCharOutFilter = fltr.SampleFilter(domain="SRAM", name="char-out", nodeId=1)
    dataFilter.removeSamples(nodeTxCharOutFilter)
    dataFilter.filter(nodeTxCharOutFilter)
    xData, yData, annotations = dataFilter.getData(nodeTxCharOutFilter)
    annotations = pltr.reMapAnnotation(annotations, charOutToHumanReadableAnnotation)
    dataPlotter.addPlot(xData, yData, annotations, "[1] char-out")

    nodeFilter = fltr.SampleFilter(domain="SRAM", name="int16-out", nodeId=1)
    dataFilter.removeSamples(nodeFilter)
    dataFilter.filter(nodeFilter)
    xData, yData, annotations = dataFilter.getData(nodeFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[1] int16-out")

    dataPlotter.setWindowTitle("Network 1x2 Simulation")
    dataFilter.printValues()
    dataPlotter.plot()
