import re
import matplotlib.pyplot as plt


class Sample:
    @staticmethod
    def toTuple(sample):
        return (sample.timeStampPicoSeconds, float(sample.value))

    @staticmethod
    def fromTuple(tuple):
        return Sample(tuple[0], tuple[1])

    def __init__(self, timeStampPicoSeconds, value):
        self.timeStampPicoSeconds = timeStampPicoSeconds
        self.value = value


class SampleFilter:
    @staticmethod
    def toString(sampleDescription):
        return ("(%s, %s, %s)" % (int(sampleDescription.nodeId), sampleDescription.domain, sampleDescription.name))

    def __init__(self, nodeId, domain, name, nameAlias=None):
        self.domain = domain
        self.name = name
        self.nodeId = nodeId
        self.nameAlias = nameAlias


class Filter:
    def __init__(self, inFile, discreteMapping):
        self.fileName = inFile
        self.wireTimeStamps = []
        self.discreteToValueMapping = discreteMapping
        self.nodeIdToDomainToNameToSamples = {}

        # line example: ' 0  0:00:00.00251216824  WIRE[HEARTBEAT] <- (low)'
        # line example: ' 0  0:00:00.00007337535  INT[#19] <- (unposted) // INT1 COMPB'
        #                   1       2                    3       4               5        6
        lineRegexp = r'^\s*(\d+)\s*(\d:\d\d:\d\d\.\d+)\s*(\w+)\[(.+)\]\s*<-\s*\((.*)\)\s*(.*)?$'
        # timesamp exmaple: '17:10:20.20004537525
        timestampRegexp = r'^(\d+):(\d\d):(\d\d)\.(\d+)$'

        self.lineMatcher = re.compile(lineRegexp)
        self.timeStampMatcher = re.compile(timestampRegexp)

    def show(self):
        for id in self.nodeIdToDomainToNameToSamples:
            for domain in self.nodeIdToDomainToNameToSamples[id]:
                for name in self.nodeIdToDomainToNameToSamples[id][domain]:
                    for sample in self.nodeIdToDomainToNameToSamples[id][domain][name]:
                        print("[%s][%s][%s] -> %s" % (id, domain, name, sample))

    def getData(self, sampleFilter):
        if sampleFilter.nodeId in self.nodeIdToDomainToNameToSamples:
            if sampleFilter.domain in self.nodeIdToDomainToNameToSamples[sampleFilter.nodeId]:
                if sampleFilter.name in self.nodeIdToDomainToNameToSamples[sampleFilter.nodeId][sampleFilter.domain]:
                    x, y = map(list, zip(*self.nodeIdToDomainToNameToSamples[sampleFilter.nodeId][sampleFilter.domain][
                        sampleFilter.name]))
                    return (x, y)
        return None

    def filter(self, sampleFilter):

        for line in open(self.fileName):
            matched = self.lineMatcher.match(line)

            print (line)
            if matched.group(1) == str(sampleFilter.nodeId) \
                    and matched.group(3).lower() == sampleFilter.domain.lower() and ((matched.group(
                4).lower() == sampleFilter.name.lower() or matched.group(4).lower() == sampleFilter.nameAlias)):

                nodeId = sampleFilter.nodeId
                domain = sampleFilter.domain
                name = sampleFilter.name

                if matched.groups() >= 5:

                    # new dict for node id
                    if nodeId not in self.nodeIdToDomainToNameToSamples:
                        self.nodeIdToDomainToNameToSamples[nodeId] = {}

                    # new list for node id and specific wire
                    if domain not in self.nodeIdToDomainToNameToSamples[nodeId]:
                        self.nodeIdToDomainToNameToSamples[nodeId][domain] = {}

                    if name not in self.nodeIdToDomainToNameToSamples[nodeId][domain]:
                        self.nodeIdToDomainToNameToSamples[nodeId][domain][name] = []

                    timeStamp = matched.group(2)
                    state = matched.group(5)
                    picoSeconds = self.__timeStampToPicoSeconds(self.timeStampMatcher.match(timeStamp))

                    lastSample = self.nodeIdToDomainToNameToSamples[nodeId][domain][name][-1:]
                    if len(lastSample) == 1:
                        if Sample.fromTuple(lastSample[0]).timeStampPicoSeconds == picoSeconds:
                            picoSeconds = picoSeconds + 1

                    sampleAsTuple = Sample.toTuple(Sample(picoSeconds, self.__toDoubleValue(state)))
                    self.nodeIdToDomainToNameToSamples[nodeId][domain][name].append(sampleAsTuple)
                    print("%s == %s" % (SampleFilter.toString(sampleFilter), state))

    def __timeStampToPicoSeconds(self, matchedTimeStampRegexpGroups):

        if len(matchedTimeStampRegexpGroups.groups()) == 4:
            picosecondsPerSecond = 10 ** 12
            hours = 60 * 60 * picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(1))
            minutes = 60 * picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(2))
            seconds = picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(3))
            picoseconds = 10 * int(matchedTimeStampRegexpGroups.group(4))
            return hours + minutes + seconds + picoseconds
        return 0

    def __toDoubleValue(self, value):
        if value in self.discreteToValueMapping.keys():
            return self.discreteToValueMapping[value]
        else:
            return float(value)


class Plotter:
    def __init__(self):
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

    def addPlot(self, xData, yData, title, xLabel, yLabel):

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
        plt.figure()
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
        plt.show()


if __name__ == "__main__":
    def addInterruptPlot(title, nodeId=0, facet="post", interruptName="TX_RX_TIMER_TOP", interruptNameAlias=None,
                         xAxisDescription="", yAxisDescription=""):
        name = interruptToNumber[interruptName] + "-" + facet
        interruptSampleFilter = SampleFilter(domain="INT",
                                             name=name,
                                             nameAlias=interruptNameAlias, nodeId=nodeId)
        filter.filter(interruptSampleFilter)
        try:
            xData, yData = filter.getData(interruptSampleFilter)
            p.addPlot(xData, yData, interruptToName[interruptName] + " - " + title, xAxisDescription,
                      interruptDomainToName[facet])
        except:
            print("cannot add plot [" + title + "] for %s[%s]" % ("INT", name))


    def addPlot(title="UDR[char-out]", nodeId=0, domain="SRAM", name="char-out", yAxisDescription=""):
        sampleFilter = SampleFilter(domain=domain, name=name, nodeId=nodeId)
        filter.filter(sampleFilter)
        try:
            xData, yData = filter.getData(sampleFilter)
            p.addPlot(xData, yData, title, "", yAxisDescription)
        except:
            print("cannot add plot [" + title + "] for %s[%s]" % (domain, name))


    stringToDiscreteValue = {"high": 1.0, "low": 0.0,
                             "hi": 1.0, "lo": 0.0,
                             "posted": 1.0, "unposted": 0.0,
                             "enabled": 1.0, "disabled": 0.0,
                             "invoke": 1.0, "return": 0.0,
                             "'1'": 1.0, "'0'": 0.0, "'x'": 0.5, "'S'": 1.0, "'B'": 0.2, "'A'": 1.0}

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

    filter = Filter("/tmp/particle-state.log", stringToDiscreteValue)

    p = Plotter()

    # transmission wire plot
    transmissionWirefilter = SampleFilter(domain="WIRE", name="tx-south", nodeId=1)
    filter.filter(transmissionWirefilter)
    xData, yData = filter.getData(transmissionWirefilter)
    p.addPlot(xData, yData, "tx-south", "", "tx signal")

    # reception wire plot
    receptionWireFilter = SampleFilter(domain="WIRE", name="rx-north", nodeId=0)
    filter.filter(receptionWireFilter)
    xData, yData = filter.getData(receptionWireFilter)
    p.addPlot(xData, yData, "rx-north 1", "", "rx signal")

    # interrupt: timer/counter1 plots
    interruptName = "NORTH_RECEPTION"
    addInterruptPlot(title="un-/posting", nodeId=0, facet="post", interruptName=interruptName)
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable",
    #                  xAxisDescription="")
    addInterruptPlot(title="call/return", nodeId=0, facet="invoke", interruptName=interruptName)

    interruptName = "TX_RX_TIMER_TOP"
    addInterruptPlot(title="un-/posting", nodeId=0, facet="post", interruptName=interruptName,
                     xAxisDescription="")
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable",
    #                  xAxisDescription="")
    addInterruptPlot(title="call/return", nodeId=0, facet="invoke", interruptName=interruptName)

    interruptName = "TX_RX_TIMER_CENTER"
    addInterruptPlot(title="un-/posting", nodeId=0, facet="post", interruptName=interruptName)
    # addInterruptPlot(title="en-/disabling", nodeId=0, domain="enable", interruptName=interruptName,
    #                  interruptNameAlias="#0-enable",
    #                  xAxisDescription="")
    addInterruptPlot(title="call/return", nodeId=0, facet="invoke", interruptName=interruptName)

    addInterruptPlot(title="call/return", nodeId=0, facet="invoke", interruptName="TX_RX_TIMEOUT_INTERRUPT")

    addPlot(title="SRAM[char-out]", nodeId=0, domain="SRAM", name="char-out", yAxisDescription="")

    addPlot(title="SRAM[int16-out]", nodeId=0, domain="SRAM", name="int16-out", yAxisDescription="")

    filter.show()
    p.plot()
