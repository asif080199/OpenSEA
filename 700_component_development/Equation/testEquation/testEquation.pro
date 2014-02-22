#-------------------------------------------------
#
# Project created by QtCreator 2014-02-13T18:48:26
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = testEquation
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    equationofmotion.cpp \
    eqntranslation.cpp \
    body.cpp \
    motionmodel.cpp

# Platform Specific files go in these scope brackets
# ========================================================
win32 {
    # Any files specific to windows go in these brackets.
    LIBS += \
        $$PWD/../../../lib/armadillo.h \        #Add armadillo and associated support
        $$PWD/../../../lib/blas_win32_MT.lib \
        $$PWD/../../../lib/lapack_win32_MT.lib
    LIBS += boost_system boost_filesystem   # Add boost and boost filesystem
}

unix {
    # Any files specific to linux go in these brackets.
    LIBS += -larmadillo -llapack -lblas #Add armadillo and associated support
    LIBS += -lboost_system -lboost_filesystem   # Add boost and boost filesystem
}

HEADERS += \
    equationofmotion.h \
    eqntranslation.h \
    body.h \
    motionmodel.h
