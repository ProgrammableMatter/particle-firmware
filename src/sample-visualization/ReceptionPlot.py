import Filter

# TODO: move addInterruptPlot(), addPlot to Filter.py
if __name__ == "__main__":
    def addInterruptPlot(title, nodeId=0, facet="post", interruptName="TX_RX_TIMER_TOP", interruptNameAlias=None,
                         yAxisDescription=""):
        name = interruptToNumber[interruptName] + "-" + facet
        interruptSampleFilter = Filter.SampleFilter(domain="INT",
                                                    name=name,
                                                    nameAlias=interruptNameAlias, nodeId=nodeId)
        filter.filter(interruptSampleFilter)
        try:
            xData, yData = filter.getData(interruptSampleFilter)
            p.addPlot(xData, yData, interruptToName[interruptName] + " - " + title)  # , yAxisDescription,
            # interruptDomainToName[facet])
        except:
            print("cannot add plot [" + title + "] for %s[%s]" % ("INT", name))


    def addPlot(title="UDR[char-out]", nodeId=0, domain="SRAM", name="char-out", yAxisDescription=""):
        sampleFilter = Filter.SampleFilter(domain=domain, name=name, nodeId=nodeId)
        filter.filter(sampleFilter)
        try:
            xData, yData = filter.getData(sampleFilter)
            p.addPlot(xData, yData, title, "", yAxisDescription)
        except:
            print("cannot add plot [" + title + "] for %s[%s]" % (domain, name))


    wireValueMapping = {"high": 1.0, "low": 0.0,
                        "hi": 1.0, "lo": 0.0,}

    interruptValueMapping = {"posted": 1.0, "unposted": 0.0,
                             "enabled": 1.0, "disabled": 0.0,
                             "invoke": 1.0, "return": 0.0,}

    interruptToNumber = {"TX_RX_TIMER_TOP": "#7",
                         "TX_RX_TIMER_CENTER": "#8",
                         "TX_RX_TIMEOUT_INTERRUPT": "#20",
                         "NORTH_RECEPTION": "#19"}

    interruptToName = {"TX_RX_TIMER_TOP": "counter1 comp. A (top)",
                       "TX_RX_TIMER_CENTER": "counter1 comp. B (center)",
                       "TX_RX_TIMEOUT_INTERRUPT": "counter0 comp. A",
                       "NORTH_RECEPTION": "north reception"}

    interruptDomainToName = {"post": "un-/posting",
                             "enable": "en-/disabling",
                             "invoke": "call/return"}

    filter = Filter.Filter("/tmp/particle-state.log", wireValueMapping)

    p = Filter.Plotter()

    # transmission wire plot
    transmissionWirefilter = Filter.SampleFilter(domain="WIRE", name="tx-south", nodeId=1)
    filter.filter(transmissionWirefilter)
    xData, yData = filter.getData(transmissionWirefilter)
    p.addPlot(xData, yData, "tx-south")

    # reception wire plot
    receptionWireFilter = Filter.SampleFilter(domain="WIRE", name="rx-north", nodeId=0)
    filter.filter(receptionWireFilter)
    xData, yData = filter.getData(receptionWireFilter)
    p.addPlot(xData, yData, "rx-north")

    # interrupt: timer/counter1 plots
    interruptName = "NORTH_RECEPTION"
    filter.setValueMapping(interruptValueMapping)
    addInterruptPlot(title="un-/posting", nodeId=0, facet="post", interruptName=interruptName)
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable")
    addInterruptPlot(title="call/return", nodeId=0, facet="invoke", interruptName=interruptName)

    interruptName = "TX_RX_TIMER_TOP"
    addInterruptPlot(title="un-/posting", nodeId=0, facet="post", interruptName=interruptName)
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable")
    addInterruptPlot(title="call/return", nodeId=0, facet="invoke", interruptName=interruptName)

    interruptName = "TX_RX_TIMER_CENTER"
    addInterruptPlot(title="un-/posting", nodeId=0, facet="post", interruptName=interruptName)
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable")
    addInterruptPlot(title="call/return", nodeId=0, facet="invoke", interruptName=interruptName)

    addInterruptPlot(title="call/return", nodeId=0, facet="invoke", interruptName="TX_RX_TIMEOUT_INTERRUPT")

    # TODO: isReceiving/isOverflowed plot: SRAM[globalState.ports.rx.XXX.( - [7:5] | isOverflowed [4]  | isReceiving [3:0])]
    addPlot(title="SRAM[int16-out]", nodeId=0, domain="SRAM", name="int16-out")
    #addPlot(title="tx - SRAM[int16-out]", nodeId=1, domain="SRAM", name="int16-out")

    receptionInterruptValueMapping = {"'U'": 0.0, "'S'": 0.2,
                                      "'A'": 0.6, "'B'": 0.4, "'x'": 0.0,"'0'": 1.2, "'1'": 1.4}
    filter.setValueMapping(receptionInterruptValueMapping)
    addPlot(title="SRAM[char-out] - States", nodeId=0, domain="SRAM", name="char-out")

    # interruptByteValues = {'0': 0.0, '1': 1.0}
    # filter.setValueMapping(interruptByteValues)
    # addPlot(title="SRAM[char-out] - Bytes", nodeId=0, domain="SRAM", name="char-out", yAxisDescription="")

    filter.show()
    p.plot()
