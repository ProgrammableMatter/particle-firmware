import json
from collections import OrderedDict
from google.protobuf.internal.decoder import EnumDecoder

from EnumsToJson import EnumsToJson
from ParseCTypes import CTypesParser, StructDeclVisitor, EnumDeclVisitor


def printLeaf(fieldPath, fieldMame, fullyQualifiedName, type, bitSize, dimension):
    if bitSize is not None and bitSize is not "":
        bitSize = ":" + bitSize
    else:
        bitSize = ""

    if dimension is not None and dimension is not "":
        dimension = "[" + dimension + "]"
    else:
        dimension = ""

    print("%s%s <%s%s>" % (fullyQualifiedName, dimension, type, bitSize))


def traverseStructMembersDF(structs, nativeTypesToSize, enumTypesToValue, rootStructName, prefix, callback=printLeaf):
    for structSubDecl in structs[rootStructName]:
        structSubDeclTypeName = StructDeclVisitor.getStructTypeName(structSubDecl)
        structSubDeclName = structSubDecl.name

        dimension = None
        if hasattr(structSubDecl.type, "dim"):
            dimension = int(structSubDecl.type.dim.value)

        if structSubDeclTypeName in nativeTypesToSize.keys() or \
                        structSubDeclTypeName in enumTypesToValue.keys():  # on enum/native type

            bitSize = None
            if structSubDecl.bitsize != None:
                bitSize = structSubDecl.bitsize.value

            callback(prefix, structSubDeclName, prefix + "." + structSubDeclName, structSubDeclTypeName, bitSize,
                     dimension)
        else:  # on struct type
            if dimension != None:
                for dim in range(dimension):
                    traverseStructMembersDF(structs, nativeTypesToSize, enumTypesToValue,
                                            rootStructName=structSubDeclTypeName, callback=callback,
                                            prefix=prefix + "." + structSubDeclName + "[" + str(dim) + "]")
            else:
                traverseStructMembersDF(structs, nativeTypesToSize, enumTypesToValue,
                                        rootStructName=structSubDeclTypeName, callback=callback,
                                        prefix=prefix + "." + structSubDeclName)


def appendEnumToSize(enumToSize, typeToSizeMapping):
    for enum in enumToSize.keys():
        typeToSizeMapping[enum] = 2


def appendEnumNames(enumDict, nativeTypeToSimulatorTypeMaping):
    for enum in enumDict.keys():
        nativeTypeToSimulatorTypeMaping[enum] = enum


class FieldDescription:
    def __init__(self, description, fieldPath, fieldName, typeName, isBitField=False, startBit=None, bitsLength=None):
        self.startBit = startBit
        self.length = bitsLength
        self.endBit = None
        self.fieldPath = fieldPath
        self.fieldName = fieldName
        self.typeName = typeName
        self.__isBitField = isBitField

        if self.startBit != None:
            self.startBit = int(self.startBit)

        if self.length != None:
            self.length = int(self.length)
        else:
            self.length = 0

        if self.startBit != None and self.length != None:
            self.endBit = self.startBit + self.length - 1

        self.description = description

    def isBitField(self):
        return self.__isBitField

    def getFieldName(self):
        return self.fieldName

    def getFieldPath(self):
        return self.fieldPath

    def getTypeName(self):
        return self.typeName

    def getStartBit(self):
        return self.startBit

    def getEndBit(self):
        return self.endBit

    def getLength(self):
        return self.length

    def getDescription(self):
        return self.description

    def __str__(self):
        return "st[%s]\tlen=%s\tend[%s]\tdesc=%s p=%s f=%s t=<%s>" % (
            self.startBit, self.length, self.endBit, self.description, self.fieldPath, self.fieldName, self.typeName)


class RegisterEntry:
    def __init__(self, name, type, bitSize=None, arraySize=None):
        self.name = name
        self.type = type
        self.fieldDescriptions = {}

        if bitSize != None:
            self.isBitField = True
            self.bitSize = bitSize

        if arraySize != None:
            self.isArray = True
            self.arraySize = arraySize

    def addFieldDescription(self, fieldDescription):
        self.fieldDescriptions[fieldDescription.getStartBit()] = fieldDescription


class LinearStructComposer:
    def __init__(self, typeToSizeMapping):
        self.linearFields = []
        self.typeToSize = typeToSizeMapping
        self.aggregatedLinearFields = OrderedDict()

    def consumeStructField(self, fieldPath, fieldName, fullyQualifiedName, type, bitSize, dimension):

        isBitField = True
        if bitSize == None:
            isBitField = False
            bitSize = self.typeToSize[type] * 8
        if dimension == None:
            startBit = 0
            if bitSize != None:
                if len(self.linearFields) > 0:
                    lastField = self.linearFields[-1]
                    startBit = (1 + lastField.getEndBit()) % (self.typeToSize[lastField.getTypeName()] * 8)
            fieldDescription = FieldDescription(fullyQualifiedName, fieldPath, fieldName, type, isBitField=isBitField,
                                                startBit=startBit,
                                                bitsLength=bitSize)
            self.linearFields.append(fieldDescription)
        else:
            for i in range(dimension):
                fieldDescription = FieldDescription("%s[%i]" % (fullyQualifiedName, i), fieldPath, fieldName, type,
                                                    isBitField=isBitField, startBit=0, bitsLength=8)
                self.linearFields.append(fieldDescription)

    def __createDescriptionDictIfNotAvailable(self, key):
        if key not in self.aggregatedLinearFields:
            self.aggregatedLinearFields[key] = OrderedDict()

    def aggregateDescriptions(self):
        currentBitFieldId = 0
        for description in reversed(self.linearFields):
            if description.isBitField():
                bitFieldKey = description.getFieldPath() + "<" + str(currentBitFieldId) + ">"
                self.__createDescriptionDictIfNotAvailable(bitFieldKey)
                self.aggregatedLinearFields[bitFieldKey][description.getStartBit()] = description
                if description.getStartBit() == 0:
                    currentBitFieldId += 1
            else:
                bitFieldKey = description.getDescription() + "<" + str(currentBitFieldId) + ">"
                self.__createDescriptionDictIfNotAvailable(bitFieldKey)
                self.aggregatedLinearFields[bitFieldKey][description.getStartBit()] = description
                currentBitFieldId += 1


class LinearStructFieldsToJson:
    def __init__(self):
        self.jsonSource = None

    def __compactBitFieldDescriptions(self, fieldDescriptions):
        desc = "("
        isFirst = True
        for fieldDescription in fieldDescriptions:
            d = fieldDescriptions[fieldDescription]
            if not isFirst:
                desc += " | "
            isFirst = False

            if d.getStartBit() == d.getEndBit():
                fromTo = str(d.getStartBit())
            else:
                fromTo = str(d.getStartBit()) + ":" + str(d.getEndBit())

            desc += d.getFieldName() + " [" + fromTo + "]"
        return desc + ")"

    def toJson(self, structFields, nativeTypeToSize={}, nativeTypeToSimulatorType={}, startAddressLabel=""):

        addressOffet = 0
        entries = []
        for field in structFields:
            fieldDescription = structFields[field][structFields[field].keys()[0]]
            property = field
            fieldTypeName = fieldDescription.getTypeName()

            if fieldDescription.isBitField():
                type = nativeTypeToSimulatorType["bitfield"]
                property += "." + self.__compactBitFieldDescriptions(structFields[field])
            else:
                type = nativeTypeToSimulatorType[fieldTypeName]

            if startAddressLabel != "":
                address = startAddressLabel + "+" + str(addressOffet)
            else:
                address = addressOffet
            addressOffet += nativeTypeToSize[fieldTypeName]

            entries.append({"property": property, "type": type, "address": address})
        self.jsonSource = {"structs": entries}

    def show(self):
        print(json.dumps(self.jsonSource, sort_keys=False, indent=2, separators=(',', ': ')))

    def getDump(self):
        return json.dumps(self.jsonSource, sort_keys=False, indent=2, separators=(',', ': '))


def parseEnumTypesAndStructs():
    ctp = CTypesParser()
    sdv = StructDeclVisitor()
    structs = ctp.getEntities(sdv)
    print("parsed structs")
    sdv.show()
    edv = EnumDeclVisitor()
    enumTypesToValue = ctp.getEntities(edv)

    print("\nparsed enums")
    edv.show()
    return structs, enumTypesToValue


def aggregateFields(structs, enumTypesToValue):
    nativeTypeToSize = {"int16_t": 2,
                        "uint16_t": 2,
                        "int8_t": 1,
                        "uint8_t": 1,
                        }
    enumToSize = {}
    for enum in enumTypesToValue:
        enumToSize[enum] = 2
    typeToSizeMapping = dict(nativeTypeToSize.viewitems() | enumToSize.viewitems())
    composer = LinearStructComposer(typeToSizeMapping)
    traverseStructMembersDF(structs, nativeTypeToSize, enumTypesToValue, "ParticleState", "ParticleState",
                            callback=composer.consumeStructField)
    print("\nlinear ordered struct member list")
    for d in composer.linearFields:
        print ("%s" % d)
    composer.aggregateDescriptions()
    structFieldToDescription = OrderedDict()
    print("\naggregated linear ordered struct member list")
    for fieldPath in reversed(composer.aggregatedLinearFields.keys()):
        print ("%s" % fieldPath)
        fieldDescriptions = composer.aggregatedLinearFields[fieldPath]
        structFieldToDescription[fieldPath] = OrderedDict()
        for startBitPosition in reversed(fieldDescriptions.keys()):
            field = fieldDescriptions[startBitPosition]
            print(" -- %s" % (field))
            structFieldToDescription[fieldPath][field.getStartBit()] = field
    return enumToSize, structFieldToDescription


def generateJson(structFieldToDescription):
    typeToSize = {
        "bitfield": 1,
        "uint8_t": 1,
        "int8_t": 1,
        "uint16_t": 2,
        "int16_t": 2,
    }
    appendEnumToSize(enumToSize, typeToSize)

    nativeTypeToSimulatorType = {
        "bitfield": "bit",
        # 1byte
        "uint8_t": "unsigned",
        "int8_t": "signed",
        "hex": "hex",
        # 2 byte
        "uint16_t": "unsigned int",
        "int16_t": "signed int",
        # 1 byte as bit field
        "bitfield": "bit",
        # 1 byte as char
        "char": "char",
        # 2 byte as hex
        "dhex": "dhex",
        "hex16": "hex16",
    }
    appendEnumNames(enumToSize, nativeTypeToSimulatorType)

    lsfj = LinearStructFieldsToJson()
    lsfj.toJson(structFieldToDescription, nativeTypeToSize=typeToSize,
                nativeTypeToSimulatorType=nativeTypeToSimulatorType, startAddressLabel="globalStateBase")
    # lsfj.show()
    return lsfj.getDump()


if __name__ == "__main__":
    structs, enumTypesToValue = parseEnumTypesAndStructs()
    enumToSize, structFieldToDescription = aggregateFields(structs, enumTypesToValue)
    j = generateJson(structFieldToDescription)
    print(j)

    # Todo:
    # add "patch" to config to override autogenerated
    #       for 2byte types: treat as uint8_t for low byte, uint16 for high byte
    #       for type overriding of auto generated: uint8_t vs. char, hex, bin
