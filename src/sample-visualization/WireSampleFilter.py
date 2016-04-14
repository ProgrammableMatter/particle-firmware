import re
import matplotlib.pyplot as plt


class Sample:
    @staticmethod
    def toTuple(sample):
        return (sample.timeStampPicoSeconds, float(sample.value))

    @staticmethod
    def fromTuple(tuple):
        return Sample(tuple[0], tuple[1], )

    def __init__(self, timeStampPicoSeconds, value):
        self.timeStampPicoSeconds = timeStampPicoSeconds
        self.value = value


class WireDescription:
    @staticmethod
    def fromTuple(tuple):
        return WireDescription(tuple[0], tuple[1])

    @staticmethod
    def toTuple(wireDescription):
        return (wireDescription.name, int(wireDescription.nodeId))

    def __init__(self, name, nodeId):
        self.name = name
        self.nodeId = nodeId


class WireSampleFilter:
    def __init__(self, inFile):
        self.file = open(inFile)
        self.wireTimeStamps = []
        self.discreteToValueMapping = {"high": 1.0, "low": 0.0,
                                       "hi": 1.0, "lo": 0.0}
        self.nodeToWireToSamples = {}
        self.wireOfInterest = []

    def show(self):
        for id in self.nodeToWireToSamples:
            for wire in self.nodeToWireToSamples[id]:
                for sample in self.nodeToWireToSamples[id][wire]:
                    print("[%s][%s] -> %s" % (id, wire, sample))

    def getData(self, wireName, nodeId):
        nodeId = str(nodeId)
        if nodeId in self.nodeToWireToSamples:
            if wireName in self.nodeToWireToSamples[nodeId]:
                x,y = map(list, zip(*self.nodeToWireToSamples[nodeId][wireName]))
                return (x,y)
        return None

    def addFilter(self, wireDescription):
        self.wireOfInterest.append(WireDescription.toTuple(wireDescription))

    def filter(self):
        # line example: '    0  0:00:00.00251216824  WIRE[HEARTBEAT] <- (low)'
        lineRegexp = r'^\s*(\d+)\s*(\d:\d\d:\d\d.\d+)\s*(\w+)\[(.+)\]\s*<-\s*\((.*)\)\s*$'
        # timesamp exmaple: '17:10:20.20004537525
        timestampRegexp = r'^(\d+):(\d\d):(\d\d).(\d+)$'

        lineMatcher = re.compile(lineRegexp)
        timeStampMatcher = re.compile(timestampRegexp)

        for line in self.file:
            matched = lineMatcher.match(line)
            if matched.group(3).lower() == "wire":
                nodeId = matched.group(1)
                wireName = matched.group(4)
                if WireDescription.toTuple(WireDescription(wireName, nodeId)) in self.wireOfInterest:
                    if matched.groups() >= 6:
                        # print(
                        # "%s in %s" % (WireDescription.toTuple(WireDescription(wireName, nodeId)), self.wireOfInterest))

                        # new dict for node id
                        if nodeId not in self.nodeToWireToSamples:
                            self.nodeToWireToSamples[nodeId] = {}

                        # new list for node id and specific wire
                        if wireName not in self.nodeToWireToSamples[nodeId]:
                            self.nodeToWireToSamples[nodeId][wireName] = []

                        timeStamp = matched.group(2)
                        picoSeconds = self.__timeStampToNanoSeconds(timeStampMatcher.match(timeStamp))
                        wireState = matched.group(5)
                        sampleAsTuple = Sample.toTuple(Sample(picoSeconds, self.__toDoubleValue(wireState)))
                        self.nodeToWireToSamples[nodeId][wireName].append(sampleAsTuple)

    def __timeStampToNanoSeconds(self, matchedTimeStampRegexpGroups):

        if len(matchedTimeStampRegexpGroups.groups()) == 4:
            picosecondsPerSecond = 10 ** 12
            hours = 60 * 60 * picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(1))
            minutes = 60 * picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(2))
            seconds = picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(3))
            picoseconds = 10 * int(matchedTimeStampRegexpGroups.group(4))
            return hours + minutes + seconds + picoseconds
        return 0

    def __toDoubleValue(self, value):
        if value in self.discreteToValueMapping:
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

        sortedSamples = [(x,y) for (x, y) in sorted(zip(xData, yData))]

        discretizedSamples =  []
        lastSample = sortedSamples[0]
        discretizedSamples.append(lastSample)
        for sample in sortedSamples[1:]:
            discretizedSamples.append((sample[0]-1, lastSample[1]))
            discretizedSamples.append(sample)
            lastSample = sample

        xData, yData = map(list, zip(*discretizedSamples))

        self.plots.append((xData, yData, title, xLabel, yLabel))

        self.maxY = max(yData)
        if xData[-1] > self.maxX:
            self.maxX = xData[-1]

    def plot(self):
        # data = [(x, x) for x in range(1, 10)]
        data = [x for x in range(1, 10)]
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
            self.__newSubplot(xData, yData, title, xLabel, yLabel)
            row = row + 1
            plt.axis([0, self.maxX, -0.2, self.maxY + 0.2])
        plt.show()


if __name__ == "__main__":
    filter = WireSampleFilter("/tmp/particle-state.log")
    filter.addFilter(WireDescription(name="rx-north", nodeId=0))
    filter.addFilter(WireDescription(name="tx-south", nodeId=1))
    filter.filter()
    filter.show()

    p = Plotter()

    xData, yData = filter.getData("tx-south", 1)
    p.addPlot(xData, yData, "tx-south 1", "t[pico seconds]", "signal")

    xData, yData = filter.getData("rx-north", 0)
    p.addPlot(xData, yData, "rx-north 0", "t[pico seconds]", "signal")

    p.plot()
