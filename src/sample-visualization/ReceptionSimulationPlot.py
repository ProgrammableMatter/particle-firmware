from LogSamplePlot import Plotter as pltr

from LogSamplePlot import Filter as fltr

from Config import Mappings as config

if __name__ == "__main__":
    filter = fltr.Filter("/tmp/particle-state.log", config.wireToFloatValueMapping)
    plotter = pltr.Plotter()

    # transmission wire plot
    transmissionWirefilter = fltr.SampleFilter(domain="WIRE", name="tx-south", nodeId=1)
    filter.filter(transmissionWirefilter)
    xData, yData, annotations = filter.getData(transmissionWirefilter)
    plotter.addPlot(xData, yData, annotations, "tx-south [1]")

    # reception wire plot
    receptionWireFilter = fltr.SampleFilter(domain="WIRE", name="rx-north", nodeId=0)
    filter.filter(receptionWireFilter)
    xData, yData, annotations = filter.getData(receptionWireFilter)
    plotter.addPlot(xData, yData, annotations, "rx-north [0]")

    # interrupt: timer/counter1 plots
    interruptName = "NORTH_RECEPTION"
    filter.setValueMapping(config.interruptToFloatValueMapping)

    pltr.addInterruptPlot(filter, plotter, title="un-/posting [0]", nodeId=0,
                          interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                          interruptName=interruptName)
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable")
    pltr.addInterruptPlot(filter, plotter, title="call/return [0]", nodeId=0,
                          interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                          interruptName=interruptName)

    interruptName = "TX_RX_TIMER_TOP"
    pltr.addInterruptPlot(filter, plotter, title="un-/posting [0]", nodeId=0,
                          interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                          interruptName=interruptName)
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable")
    pltr.addInterruptPlot(filter, plotter, title="call/return", nodeId=0,
                          interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                          interruptName=interruptName)

    interruptName = "TX_RX_TIMER_CENTER"
    pltr.addInterruptPlot(filter, plotter, title="un-/posting [0]", nodeId=0,
                          interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                          interruptName=interruptName)
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable")
    pltr.addInterruptPlot(filter, plotter, title="call/return [0]", nodeId=0,
                          interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                          interruptName=interruptName)

    pltr.addInterruptPlot(filter, plotter, title="call/return [0]", nodeId=0,
                          interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                          interruptName="TX_RX_TIMEOUT_INTERRUPT")

    pltr.addPlot(filter, plotter, title="SRAM[int16-out]", nodeId=0, domain="SRAM", name="int16-out")
    # addPlot(title="tx - SRAM[int16-out]", nodeId=1, domain="SRAM", name="int16-out")

    receptionInterruptValueMapping = {"'U'": 0.0, "'S'": 0.2,
                                      "'A'": 0.6, "'B'": 0.4, "'x'": 0.0, "'0'": 1.2, "'1'": 1.4}
    filter.setValueMapping(receptionInterruptValueMapping)
    pltr.addPlot(filter, plotter, title="SRAM[char-out] - States", nodeId=0, domain="SRAM", name="char-out")

    plotter.setWindowTitle("Reception Simulation")
    filter.printValues()
    plotter.plot()
