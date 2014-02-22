#-------------------------------------------------
#
# Project created by QtCreator 2014-02-16T08:45:48
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = MotionModelObject
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    motion_model/motionmodel.cpp \
    global_objects/system.cpp \
    global_objects/ofreqcore.cpp \
    global_objects/forcereact.cpp \
    global_objects/forcecross.cpp \
    global_objects/forceactive.cpp \
    global_objects/force.cpp \
    global_objects/equation.cpp \
    global_objects/derivative.cpp \
    motion_model/motionmodel.cpp \
    global_objects/body.cpp

HEADERS += \
    motion_model/motionmodel.h \
    global_objects/system.h \
    global_objects/ofreqcore.h \
    global_objects/forcereact.h \
    global_objects/forcecross.h \
    global_objects/forceactive.h \
    global_objects/force.h \
    global_objects/equation.h \
    global_objects/derivative.h \
    global_objects/body.h \
    motion_model/motionmodel.h

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
