TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3

SOURCES += \
        ../CommonClasses/TMConfiguration.cpp \
        MasterFrame.cpp \
        ParameterInfo.cpp \
        ProcessParameters.cpp \
        TMDataType.cpp \
        TMDatabase.cpp \
        TMProcessingType.cpp \
        TMProcessor.cpp \
        ThreadSpawner.cpp \
        main.cpp

HEADERS += \
    ../CommonClasses/TMConfiguration.h \
    MasterFrame.h \
    ParameterInfo.h \
    ProcessParameters.h \
    TMDataType.h \
    TMDatabase.h \
    TMProcessingType.h \
    TMProcessor.hpp \
    ThreadSpawner.h \
    ../includes/GlobalMacros.h \
    ../includes/HkTmShmBuf.h \
    ../includes/MqBufDefs.h \
    ../includes/StringUtils.h \
    ../includes/SystemFiles.h \
    ../includes/TMDBReader.h \
    tmProcessor.h


LIBS += /home/pramod/projects/tm-api-dataTypeChg/dist/Debug/GNU-Linux/libtm-api-datatypechg.a
#LIBS +=  /umacssrc/umacsdevadmin/lib/libtm-api.a
LIBS += -lpthread

DISTFILES += \
    README.md
