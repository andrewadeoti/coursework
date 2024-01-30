TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++20 -Wall -Wfatal-errors

OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/
TARGET = nand-parser-tests

LIBS += -lboost_unit_test_framework

INCLUDEPATH += headers/

HEADERS += \
    headers/earth.h \
    headers/geometry.h \
    headers/nand/nand-data_entry.h \
    headers/nand/nand-parser.h \
    headers/types.h \
    headers/waypoint.h

SOURCES += \
    src/earth.cpp \
    src/geometry.cpp \
    src/nand/nand-parser.cpp \
    src/waypoint.cpp

SOURCES += \
    tests/nand/BoostUTF-main.cpp \
    tests/nand/nand-parser-dataEntryToWaypoint-tests.cpp \
    tests/nand/nand-parser-hasExpectedNumberOfFields-tests.cpp \
    tests/nand/nand-parser-hasFormOfNANDdataEntry-tests.cpp \
    tests/nand/nand-parser-isRecognisedCode-tests.cpp \
    tests/nand/nand-parser-parseDataEntry-tests.cpp \
    tests/nand/nand-parser-processDataLog-tests.cpp \
    tests/nand/nand-parser-verifyChecksum-tests.cpp
