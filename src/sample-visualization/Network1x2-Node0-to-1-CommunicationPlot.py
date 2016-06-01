from LogSamplePlot import Plotter as pltr

from LogSamplePlot import Filter as fltr

from Config import Mappings as config

if __name__ == "__main__":
    dataFilter = fltr.Filter("/tmp/particle-state.log", config.wireToFloatValueMapping)
    dataPlotter = pltr.Plotter()

    # transmission wire plot
    transmissionWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=0)
    dataFilter.filter(transmissionWirefilter)
    xData, yData = dataFilter.getData(transmissionWirefilter)
    dataPlotter.addPlot(xData, yData, "tx-south [0]")

    # reception wire plot
    receptionWireFilter = fltr.SampleFilter(domain="WIRE", name="rx-south", nodeId=0)
    dataFilter.filter(receptionWireFilter)
    xData, yData = dataFilter.getData(receptionWireFilter)
    dataPlotter.addPlot(xData, yData, "rx-north [0]")

    dataFilter.printValues()
    dataPlotter.plot()
