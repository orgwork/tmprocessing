CONFIG -= qt

TEMPLATE = lib

CONFIG += c++11

CONFIG += staticlib

QMAKE_CC = gcc
QMAKE_CXX = gcc

SOURCES += \
    ../CommonClasses/TMConfiguration.cpp \
    RtpTmApi.cpp \
    rtpapi.cpp

HEADERS += \
    ../CommonClasses/TMConfiguration.h \
    RtpTmApi.hpp \
    rtpapi.h

