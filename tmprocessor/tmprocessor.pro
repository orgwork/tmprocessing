CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../RTP/MasterFrame.cpp \
    ../RTP/ParameterInfo.cpp \
    ../RTP/ProcessParameters.cpp \
    ../RTP/TMDataType.cpp \
    ../RTP/TMDatabase.cpp \
    ../RTP/TMProcessingType.cpp \
    ../RTP/TMProcessor.cpp

HEADERS += \
    ../RTP/MasterFrame.h \
    ../RTP/ParameterInfo.h \
    ../RTP/ProcessParameters.h \
    ../RTP/TMDataType.h \
    ../RTP/TMDatabase.h \
    ../RTP/TMProcessingType.h \
    ../RTP/TMProcessor.hpp \
    ../includes/TMDBReader.h \
    ../includes/TMShmStruct.h



LIBS += /umacssrc/umacsusr18/libs/libtm-api.a

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
