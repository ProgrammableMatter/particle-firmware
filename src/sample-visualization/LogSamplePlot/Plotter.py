import gtk
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
        self.pointAnnotations = []
        self.windowTitle = ""

        # self.lastXCoordinate = 0;
        self.lastClickedXCoordinate = None;

    def __newSubplot(self, xValues, yValues, annotations, title="Title", xLabel="", yLabel=""):
        """
        Create a new figure and plot data using blue, square markers.
        """
        points = plt.plot(xValues, yValues, linestyle="-", color="b", marker=".", picker=5)  # line plot

        for x, y, a in zip(points[0]._x, points[0]._y, annotations):
            annotation = points[0].axes.annotate("%s" % a, xy=(x, y), xycoords='data',
                                                 xytext=(x, y), textcoords='data',
                                                 horizontalalignment="center",
                                                 arrowprops=dict(arrowstyle="simple", connectionstyle="arc3,rad=-0.2"),
                                                 bbox=dict(boxstyle="round", facecolor="w", edgecolor="0.5", alpha=0.9)
                                                 )
            annotation.set_visible(False)
            self.pointAnnotations.append([(float(x), float(y)), annotation])

        plt.title(title, y=0.5)
        plt.xlabel(xLabel)
        plt.ylabel(yLabel)
        plt.grid()

    def setWindowTitle(self, title):
        self.windowTitle = title

    def addPlot(self, xData, yData, annotations, title, xLabel="", yLabel=""):

        if xData == None or yData == None:
            return

        sortedSamples = sorted(zip(xData, yData, annotations))

        # to achieve a rectangular path on chart, before the next event we must add the last y value
        discretizedSamples = []
        lastSample = sortedSamples[0]
        discretizedSamples.append(lastSample)
        for sample in sortedSamples[1:]:
            discretizedSamples.append((sample[0] - 1, lastSample[1], "(" + lastSample[2] + ")"))
            discretizedSamples.append(sample)
            lastSample = sample

        xData, yData, annotations = map(list, zip(*discretizedSamples))

        maxY = max(yData)
        if xData[-1] > self.maxX:
            self.maxX = xData[-1]

        self.plots.append((xData, yData, title, xLabel, yLabel, maxY, annotations))

    def plot(self):
        fig = plt.figure()
        plt.subplots_adjust(hspace=0.6)

        rows = len(self.plots)
        columns = 1
        row = 1

        def formatCoord(x, y):
            # self.lastXCoordinate = x
            if self.lastClickedXCoordinate != None:
                difference = (self.lastClickedXCoordinate - x) * 1E-9
                return '(t={:1.9f}[ms], y={:}) || t1={:1.9f}[ms] diff.={:1.9f}[ms]'.format(x * 1E-9, y,
                                                                                           self.lastClickedXCoordinate * 1E-9,
                                                                                           difference)
            return '(t={:1.9f}[ms], y={:})'.format(x * 1E-9, y)

        for plot in self.plots:
            ax = plt.subplot(rows, columns, row)
            ax.format_coord = formatCoord
            xData = plot[0]
            yData = plot[1]
            title = plot[2]
            xLabel = plot[3]
            yLabel = plot[4]
            yMax = plot[5]
            annotations = plot[6]
            self.__newSubplot(xData, yData, annotations, title, xLabel, yLabel)
            row = row + 1
            plt.axis([0, self.maxX, -0.2, yMax + 0.2])
        fig.tight_layout()
        plt.subplots_adjust(left=self.leftMargin, right=self.rightMargin, top=self.topMargin, bottom=self.bottomMargin,
                            hspace=self.hSpace, wspace=self.wSpace)
        fig.canvas.set_window_title(self.windowTitle)

        def __onPick(event):
            """
            Changes the annotation's visibility according to the mous event

            :param event:
            :return:
            """
            thisline = event.artist
            xdata = thisline.get_xdata()
            ydata = thisline.get_ydata()
            ind = event.ind
            eventPoints = zip(xdata[ind], ydata[ind])

            # self.lastClickedXCoordinate = self.lastXCoordinate

            for eventPoint in eventPoints:
                isVisibilityChanged = False
                for chartPoint, annotation in self.pointAnnotations:
                    if (float(eventPoint[0]), float(eventPoint[1])) == (float(chartPoint[0]), float(chartPoint[1])):
                        if annotation.get_visible() == False:
                            annotation.set_visible(True)
                            self.lastClickedXCoordinate = float(chartPoint[0])
                            clipboard = gtk.clipboard_get()
                            clipboard.set_text(str(self.lastClickedXCoordinate * 1E-9))
                            clipboard.store()
                            isVisibilityChanged = True
                    else:
                        if annotation.get_visible() == True:
                            annotation.set_visible(False)
                            isVisibilityChanged = True

            if isVisibilityChanged:
                plt.draw()

        fig.canvas.mpl_connect('pick_event', __onPick)
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
        xData, yData, annotations = filter.getData(interruptSampleFilter)
        plotter.addPlot(xData, yData, annotations, title + " " +
                        interruptToNumberMapping[interruptName])  # , yAxisDescription),
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
        xData, yData, annotations = filter.getData(sampleFilter)
        plotter.addPlot(xData, yData, annotations, title, "", yAxisDescription)
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
    addInterruptPlot(filter, plotter, title="un-/posting", nodeId=0,
                     interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                     interruptName=interruptName,
                     yAxisDescription="")
    addInterruptPlot(filter, plotter, title="call/return", nodeId=0,
                     interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                     interruptName=interruptName,
                     yAxisDescription="")

    interruptName = "TX_RX_TIMER_TOP"
    addInterruptPlot(filter, plotter, title="un-/posting", nodeId=0,
                     interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                     interruptName=interruptName,
                     yAxisDescription="")
    addInterruptPlot(filter, plotter, title="call/return", nodeId=0,
                     interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                     interruptName=interruptName,
                     yAxisDescription="")

    interruptName = "TX_RX_TIMER_CENTER"
    addInterruptPlot(filter, plotter, title="un-/posting", nodeId=0,
                     interruptToNumberMapping=config.interruptToNumberMapping, facet="post",
                     interruptName=interruptName,
                     yAxisDescription="")
    addInterruptPlot(filter, plotter, title="call/return", nodeId=0,
                     interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                     interruptName=interruptName,
                     yAxisDescription="")

    addInterruptPlot(filter, plotter, title="call/return", nodeId=0,
                     interruptToNumberMapping=config.interruptToNumberMapping, facet="invoke",
                     interruptName="TX_RX_TIMEOUT_INTERRUPT",
                     yAxisDescription="")

    addPlot(filter, plotter, title="SRAM[int16-out]", nodeId=0, domain="SRAM", name="int16-out", yAxisDescription="")

    receptionInterruptToDiscreteValue = {"'U'": 0.0, "'S'": 0.2,
                                         "'A'": 0.6, "'B'": 0.4, "'x'": 0.0, "'0'": 1.2, "'1'": 1.4}
    filter.setValueMapping(receptionInterruptToDiscreteValue)
    addPlot(filter, plotter, title="SRAM[char-out] - States", nodeId=0, domain="SRAM", name="char-out",
            yAxisDescription="")

    receptionCharDataToDiscreteValue = {"'U'": 0.0, "'S'": 0.0,
                                        "'A'": 0.0, "'B'": 0.0, "'x'": 0.0, "'0'": 0.5, "'1'": 1.0}
    filter.setValueMapping(receptionCharDataToDiscreteValue)
    filter.removeSamples(flt.SampleFilter(nodeId=0, domain="SRAM", name="char-out"))
    addPlot(filter, plotter, title="SRAM[char-out] - Bytes", nodeId=0, domain="SRAM", name="char-out",
            yAxisDescription="")

    filter.printValues()
    plotter.plot()


def reMapAnnotation(oldAnnotation, oldToNewMapping):
    """
    Maps an annotation list to a new according to the given mapping.
    
    :param oldAnnotation: the original annotation 
    :param oldToNewMapping: the mapping
    :return: the new annotation list
    """
    newAnnotationList = []
    for a in oldAnnotation:
        if oldToNewMapping[a] == None:
            newAnnotationList.append(a)
        else:
            newAnnotationList.append(oldToNewMapping[a])
    return newAnnotationList
