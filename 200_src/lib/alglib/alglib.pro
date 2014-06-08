#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T18:40:05
#
#-------------------------------------------------

QT       -= gui

TARGET = alglib
TEMPLATE = lib

DEFINES += ALGLIB_LIBRARY

SOURCES += alglib.cpp \
    fasttransforms.cpp \
    diffequations.cpp \
    dataanalysis.cpp \
    ap.cpp \
    alglibmisc.cpp \
    alglibinternal.cpp \
    statistics.cpp \
    specialfunctions.cpp \
    solvers.cpp \
    optimization.cpp \
    linalg.cpp \
    interpolation.cpp \
    integration.cpp

HEADERS += alglib.h\
        alglib_global.h \
    fasttransforms.h \
    diffequations.h \
    dataanalysis.h \
    ap.h \
    alglibmisc.h \
    alglibinternal.h \
    stdafx.h \
    statistics.h \
    specialfunctions.h \
    solvers.h \
    optimization.h \
    linalg.h \
    interpolation.h \
    integration.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


win32 {
    #Windows Includes
    INCLUDEPATH += \
        C:\Qt\Qt5.0.1.\mingw47_32\lib

    CONFIG += staticlib

    CONFIG += staticlib
    CONFIG(debug|release) {
        DEFINES += QT_NO_DEBUG_OUTPUT
    }
}
