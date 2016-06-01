import matplotlib.pyplot as plt

import Filter as flt

from Config import Mappings as config

class Plotter:
    def __init__(self):

        self.topMargin = 0.96
        self.bottomMargin = 0.03
        self.leftMargin = 0.03
        self.rightMargin = 0.99
        self.hSpace = 0
        self.wSpace = 0
        self.plots = []
        self.maxX = 0
        self.maxY = 0

    def __newSubplot(self, xValues, yValues, title="Title", xLabel="", yLabel=""):
        """
        Create a new figure and plot data using blue, square markers.
        """
        plt.plot(xValues, yValues)  # line plot
        plt.title(title)
        plt.xlabel(xLabel)
        plt.ylabel(yLabel)
        plt.grid()

    def addPlot(self, xData, yData, title, xLabel="", yLabel=""):

        if xData == None or yData == None:
            return


        sortedSamples = [(x, y) for (x, y) in sorted(zip(xData, yData))]

        discretizedSamples = []
        lastSample = sortedSamples[0]
        discretizedSamples.append(lastSample)
        for sample in sortedSamples[1:]:
            discretizedSamples.append((sample[0] - 1, lastSample[1]))
            discretizedSamples.append(sample)
            lastSample = sample

        xData, yData = map(list, zip(*discretizedSamples))

        maxY = max(yData)
        if xData[-1] > self.maxX:
            self.maxX = xData[-1]

        self.plots.append((xData, yData, title, xLabel, yLabel, maxY))

    def plot(self):
        figure = plt.figure()
        plt.subplots_adjust(hspace=0.6)

        rows = len(self.plots)
        columns = 1
        row = 1

        for plot in self.plots:
            plt.subplot(rows, columns, row)
            xData = plot[0]
            yData = plot[1]
            title = plot[2]
            xLabel = plot[3]
            yLabel = plot[4]
            yMax = plot[5]
            self.__newSubplot(xData, yData, title, xLabel, yLabel)
            row = row + 1
            plt.axis([0, self.maxX, -0.2, yMax + 0.2])
        figure.tight_layout()
        plt.subplots_adjust(left=self.leftMargin, right=self.rightMargin, top=self.topMargin, bottom=self.bottomMargin,
                            hspace=self.hSpace, wspace=self.wSpace)
        plt.show()

def addInterruptPlot(filter, plotter, title, nodeId=0, interruptToNumberMapping={}, facet="post",
                     interruptName="TX_RX_TIMER_TOP",
                     interruptNameAlias=None,
                     yAxisDescription=""):


    """
    Convenience function for adding an interrupt chart to plot.

    :param plotter: the plotter to add the chart to
    :param title: chart title
    :param nodeId: node number
    :param interruptToNumberMapping: example: {
        "TX_RX_TIMER_TOP": "#7",
        "TX_RX_TIMER_CENTER": "#8",
        "TX_RX_TIMEOUT_INTERRUPT": "#20",
        "NORTH_RECEPTION": "#19", }
    :param facet: "post", "invoke", "enable"
    :param interruptName: the mape as specified in parameter interruptToNumberMapping
    :param interruptNameAlias:
    :param yAxisDescription: optinal description
    :return: None
    """

    name = interruptToNumberMapping[interruptName] + "-" + facet
    interruptSampleFilter = flt.SampleFilter(domain="INT",
                                         name=name,
                                         nameAlias=interruptNameAlias, nodeId=nodeId)
    filter.filter(interruptSampleFilter)
    try:
        xData, yData = filter.getData(interruptSampleFilter)
        plotter.addPlot(xData, yData, interruptToNumberMapping[interruptName] + " - " + title) #, yAxisDescription),
        # interruptDomainToName[facet])
    except:
        print("cannot add plot [" + title + "] for %s[%s]" % ("INT", name))


def addPlot(filter, plotter, title="UDR[char-out]", nodeId=0, domain="SRAM", name="char-out", yAxisDescription=""):
    """
    Convenience function for adding a chart to plot

    :param plot:
    :param title:
    :param nodeId:
    :param domain:
    :param name:
    :param yAxisDescription:
    :return:
    """

    sampleFilter = flt.SampleFilter(domain=domain, name=name, nodeId=nodeId)
    filter.filter(sampleFilter)
    try:
        xData, yData = filter.getData(sampleFilter)
        plotter.addPlot(xData, yData, title, "", yAxisDescription)
    except:
        print("cannot add plot [" + title + "] for %s[%s]" % (domain, name))



if __name__ == "__main__":
    """
    Example usage.
    """

    filter = flt.Filter("/tmp/particle-state.log", config.wireToFloatValueMapping)
    plotter = Plotter()

    # transmission wire plot
    transmissionWirefilter = flt.SampleFilter(domain="WIRE", name="tx-south", nodeId=1)
    filter.filter(transmissionWirefilter)
    xData, yData = filter.getData(transmissionWirefilter)
    plotter.addPlot(xData, yData, "tx-south")

    # reception wire plot
    receptionWireFilter = flt.SampleFilter(domain="WIRE", name="rx-north", nodeId=0)
    filter.filter(receptionWireFilter)
    xData, yData = filter.getData(receptionWireFilter)
    plotter.addPlot(xData, yData, "rx-north")

    # interrupt: timer/counter1 plots
    interruptName = "NORTH_RECEPTION"
    filter.setValueMapping(config.interruptToFloatValueMapping)
    addInterruptPlot(filter, plotter, title="un-/posting", nodeId=0, interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                     interruptName=interruptName,
                     yAxisDescription="")
    addInterruptPlot(filter, plotter, title="call/return", nodeId=0, interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                     interruptName=interruptName,
                     yAxisDescription="")

    interruptName = "TX_RX_TIMER_TOP"
    addInterruptPlot(filter, plotter, title="un-/posting", nodeId=0, interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                     interruptName=interruptName,
                     yAxisDescription="")
    addInterruptPlot(filter, plotter, title="call/return", nodeId=0, interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                     interruptName=interruptName,
                     yAxisDescription="")

    interruptName = "TX_RX_TIMER_CENTER"
    addInterruptPlot(filter, plotter, title="un-/posting", nodeId=0, interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                     interruptName=interruptName,
                     yAxisDescription="")
    addInterruptPlot(filter, plotter, title="call/return", nodeId=0, interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                     interruptName=interruptName,
                     yAxisDescription="")

    addInterruptPlot(filter, plotter, title="call/return", nodeId=0, interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                     interruptName="TX_RX_TIMEOUT_INTERRUPT",
                     yAxisDescription="")

    addPlot(filter, plotter, title="SRAM[int16-out]", nodeId=0, domain="SRAM", name="int16-out", yAxisDescription="")

    receptionInterruptToDiscreteValue = {"'U'": 0.0, "'S'": 0.2,
                                         "'A'": 0.6, "'B'": 0.4, "'x'": 0.0, "'0'": 1.2, "'1'": 1.4}
    filter.setValueMapping(receptionInterruptToDiscreteValue)
    addPlot(filter, plotter, title="SRAM[char-out] - States", nodeId=0, domain="SRAM", name="char-out", yAxisDescription="")

    receptionCharDataToDiscreteValue = {"'U'": 0.0, "'S'": 0.0,
                                        "'A'": 0.0, "'B'": 0.0, "'x'": 0.0, "'0'": 0.5, "'1'": 1.0}
    filter.setValueMapping(receptionCharDataToDiscreteValue)
    filter.removeSamples(flt.SampleFilter(nodeId=0, domain="SRAM", name="char-out"))
    addPlot(filter, plotter, title="SRAM[char-out] - Bytes", nodeId=0, domain="SRAM", name="char-out", yAxisDescription="")

    filter.printValues()
    plotter.plot()
