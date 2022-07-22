CONFIG -= qt

TEMPLATE = lib

CONFIG += c++11

CONFIG += staticlib

SOURCES += \
    ../CommonClasses/Configuration.cpp \
    rtpapi.cpp

HEADERS += \
    ../CommonClasses/Configuration.h \
    rtpapi.h \
    rtpapi.hpp

