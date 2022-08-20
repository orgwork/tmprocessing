CONFIG -= qt

TEMPLATE = lib

CONFIG += c++11

CONFIG += staticlib

QMAKE_CC = gcc
QMAKE_CXX = gcc

SOURCES += \
    ../CommonClasses/TMConfiguration.cpp \
    rtpapi.cpp

HEADERS += \
    ../CommonClasses/TMConfiguration.h \
    rtpapi.h \
    rtpapi.hpp

