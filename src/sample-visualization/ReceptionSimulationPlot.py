from LogSamplePlot import Plotter as pltr
from LogSamplePlot import Filter as fltr
from Config import Mappings as mappingConfig
from Config import Input as inputConfig

if __name__ == "__main__":
    dataFilter = fltr.Filter(inputConfig.logFile, mappingConfig.wireToFloatValueMapping)
    dataPlotter = pltr.Plotter()

    # transmission wire plot
    transmissionWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=1)
    dataFilter.filter(transmissionWirefilter)
    xData, yData, annotations = dataFilter.getData(transmissionWirefilter)
    dataPlotter.addPlot(xData, yData, annotations, "[1] tx-south")

    # reception wire plot
    receptionWireFilter = fltr.SampleFilter(domain="WIRE", name="rx-north", nodeId=0)
    dataFilter.filter(receptionWireFilter)
    xData, yData, annotations = dataFilter.getData(receptionWireFilter)
    dataPlotter.addPlot(xData, yData, annotations, "[0] rx-north")

    # interrupt: timer/counter1 plots
    interruptName = "NORTH_RECEPTION"
    dataFilter.setValueMapping(mappingConfig.interruptToFloatValueMapping)

    pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] un-/posting", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="post",
                          interruptName=interruptName)

    pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] call/return", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="invoke",
                          interruptName=interruptName)

    interruptName = "TX_RX_TIMER_TOP"
    pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] un-/posting", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="post",
                          interruptName=interruptName)

    pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] call/return", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="invoke",
                          interruptName=interruptName)

    interruptName = "TX_RX_TIMER_CENTER"
    pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] un-/posting", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="post",
                          interruptName=interruptName)

    pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] call/return", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="invoke",
                          interruptName=interruptName)

    pltr.addInterruptPlot(dataFilter, dataPlotter, title="[0] call/return", nodeId=0,
                          interruptToNumberMapping=mappingConfig.interruptToNumberMapping, facet="invoke",
                          interruptName="TX_RX_TIMEOUT_INTERRUPT")

    pltr.addPlot(dataFilter, dataPlotter, title="[0] SRAM[int16-out]", nodeId=0, domain="SRAM", name="int16-out")

    receptionInterruptValueMapping = {"'U'": 0.0, "'S'": 0.2,
                                      "'A'": 0.6, "'B'": 0.4, "'x'": 0.0, "'0'": 1.2, "'1'": 1.4}
    dataFilter.setValueMapping(receptionInterruptValueMapping)
    pltr.addPlot(dataFilter, dataPlotter, title="[0] SRAM[char-out] - rx states", nodeId=0, domain="SRAM",
                 name="char-out")

    dataPlotter.setWindowTitle("Reception Simulation")
    dataFilter.printValues()
    dataPlotter.plot()
