import re

class Sample:
    """
    A Sample class represents a single sample.
    """

    @staticmethod
    def toTuple(sample):
        return (sample.timeStampPicoSeconds, float(sample.value), sample.annotation)

    @staticmethod
    def fromTuple(tuple):
        return Sample(tuple[0], tuple[1], tuple[2])

    def __init__(self, timeStampPicoSeconds, value, annotation):
        self.timeStampPicoSeconds = timeStampPicoSeconds
        self.value = value
        self.annotation = annotation


class SampleFilter:
    """
    Sample filter class is used to match and filter samples.
    """

    @staticmethod
    def toString(sampleDescription):
        return ("(%s, %s, %s)" % (int(sampleDescription.nodeId), sampleDescription.domain, sampleDescription.name))

    def __init__(self, nodeId, domain, name, nameAlias=None):
        self.domain = domain
        self.name = name
        self.nodeId = nodeId
        self.nameAlias = nameAlias


class Filter:
    """
    Filter clas for filtering samples from log file. Each call to filter() accumulates the filtered data using the
    specified filter and the lastly defined discrete to float value mapping.
    """

    def __init__(self, inFile, discreteToFloatValueMapping):
        """

        :param inFile: the log file
        :param discreteToFloatValueMapping: the initial discrete to float value mapping
        """
        self.fileName = inFile
        self.wireTimeStamps = []
        self.discreteToValueMapping = discreteToFloatValueMapping
        self.nodeIdToDomainToNameToSamples = {}
        # line example: ' 0  0:00:00.00251216824  WIRE[HEARTBEAT] <- (low)'
        # line example: ' 0  0:00:00.00007337535  INT[#19] <- (unposted) // INT1 COMPB'
        #                   1       2                    3       4               5        6
        lineRegexp = r'^\s*(\d+)\s*(\d:\d\d:\d\d\.\d+)\s*(\w+)\[(.+)\]\s*<-\s*\((.*)\)\s*(.*)?$'
        # timesamp exmaple: '17:10:20.20004537525
        timestampRegexp = r'^(\d+):(\d\d):(\d\d)\.(\d+)$'

        self.lineMatcher = re.compile(lineRegexp)
        self.timeStampMatcher = re.compile(timestampRegexp)

    def setValueMapping(self, discreteToFloatValueMapping):
        """
        Sets a new discrete 'char' to float value mapping.

        :param discreteToFloatValueMapping:  the new mapping to use
        :return: None
        """
        self.discreteToValueMapping = discreteToFloatValueMapping

    def printValues(self):
        """
        Prints the accumulated filtered values to console.

        :return: None
        """
        for id in self.nodeIdToDomainToNameToSamples:
            for domain in self.nodeIdToDomainToNameToSamples[id]:
                for name in self.nodeIdToDomainToNameToSamples[id][domain]:
                    for sample in self.nodeIdToDomainToNameToSamples[id][domain][name]:
                        print("[%s][%s][%s] -> %s" % (id, domain, name, sample))

    def getData(self, sampleFilter):
        """
        Returns the filtered data.

        :param sampleFilter:
        :return: the filtered data as tuples of x/y coordinates
        """
        if sampleFilter.nodeId in self.nodeIdToDomainToNameToSamples.keys():
            if sampleFilter.domain in self.nodeIdToDomainToNameToSamples[sampleFilter.nodeId]:
                if sampleFilter.name in self.nodeIdToDomainToNameToSamples[sampleFilter.nodeId][sampleFilter.domain]:
                    try:
                        x, y, annotation = map(list, zip(*self.nodeIdToDomainToNameToSamples[sampleFilter.nodeId][sampleFilter.domain][
                            sampleFilter.name]))
                        return x, y, annotation
                    except:
                        return None, None, None
        return None, None, None

    def removeSamples(self, sampleFilter):
        """
        Removes values for the specified filter. In case of plotting with different discrete to float value mappings
        for same samples the old samples must be re-calculated, thus they have to be cleared before plotting.

        :param sampleFilter: the sample filter
        :return: None
        """
        self.nodeIdToDomainToNameToSamples[sampleFilter.nodeId][sampleFilter.domain][sampleFilter.name] = []

    def filter(self, sampleFilter):
        """
        Filters samples from data matching the given filter.

        :param sampleFilter: the filter
        :return: None
        """
        for line in open(self.fileName):
            matched = self.lineMatcher.match(line)

            # print (line)
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

                    doubleValue = self.__toDoubleValue(state)
                    if doubleValue != None:
                        sampleAsTuple = Sample.toTuple(Sample(picoSeconds, doubleValue, state))
                        self.nodeIdToDomainToNameToSamples[nodeId][domain][name].append(sampleAsTuple)
                        # print("%s == %s" % (SampleFilter.toString(sampleFilter), state))

    def __timeStampToPicoSeconds(self, matchedTimeStampRegexpGroups):
        """
        Converts the given timestamp to 10E-12 seconds.

        :param matchedTimeStampRegexpGroups:
        :return:
        """

        if len(matchedTimeStampRegexpGroups.groups()) == 4:
            picosecondsPerSecond = 10 ** 12
            hours = 60 * 60 * picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(1))
            minutes = 60 * picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(2))
            seconds = picosecondsPerSecond * int(matchedTimeStampRegexpGroups.group(3))
            picoseconds = 10 * int(matchedTimeStampRegexpGroups.group(4))
            return hours + minutes + seconds + picoseconds
        return 0

    def __toDoubleValue(self, value):
        """
        Maps the given value to double. If no mapping found, the value is converted to double directly.

        :param value: the char value
        :return: the mapped/converted double value
        """
        if value in self.discreteToValueMapping.keys():
            return self.discreteToValueMapping[value]
        else:
            try:
                return float(value)
            except:
                return None

