TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Configuration.cpp \
        MasterFrame.cpp \
        ParameterInfo.cpp \
        Processor.cpp \
        SharedMemoryConnector.cpp \
        TMFormatInfo.cpp \
        ThreadSpawner.cpp \
        main.cpp

HEADERS += \
    Configuration.h \
    MasterFrame.h \
    ParameterInfo.h \
    Processor.h \
    Processor.hpp \
    SharedMemoryConnector.h \
    TMFormatInfo.h \
    ThreadSpawner.h \
    includes/GlobalMacros.h \
    includes/HkTmShmBuf.h \
    includes/MqBufDefs.h \
    includes/StringUtils.h \
    includes/SystemFiles.h \
    includes/TMShmStruct.h

LIBS += -lpthread
