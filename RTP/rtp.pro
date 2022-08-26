TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

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
#LIBS += /umacssrc/umacsusr18/libs/libtm-api.a
LIBS += -lpthread

DISTFILES += \
    README.md
