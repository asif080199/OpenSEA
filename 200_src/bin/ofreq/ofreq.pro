#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T19:56:40
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ofreq
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += ofreq.cpp \
    derived_outputs/outputsbody.cpp \
    derived_outputs/outputderived.cpp \
    derived_outputs/globalvelocity.cpp \
    derived_outputs/globalsolution.cpp \
    derived_outputs/globalmotion.cpp \
    derived_outputs/globalacceleration.cpp \
    file_writer/filewriter.cpp \
    global_objects/system.cpp \
    motion_solver/motionsolver.cpp \
    wave_calcs/wavespreadmodel.cpp \
    wave_calcs/wavespectrummodel.cpp \
    wave_calcs/seaenviroment.cpp \
    motion_solver/matbody.cpp \
    motion_model/equationofmotion.cpp \
    motion_model/motionmodel.cpp \
    global_objects/solution.cpp \
    file_reader/filereader.cpp \
    file_reader/parser.cpp \
    file_reader/objectgroup.cpp \
    file_reader/dictionary.cpp \
    file_reader/dictcontrol.cpp \
    file_reader/dictforces.cpp \
    file_reader/dictbodies.cpp \
    global_objects/solutionset.cpp \
    motion_solver/matforcecross.cpp \
    motion_solver/matforceactive.cpp \
    motion_solver/matforcereact.cpp \
    global_objects/ofreqcore.cpp \
    global_objects/forcereact.cpp \
    global_objects/forcecross.cpp \
    global_objects/forceactive.cpp \
    global_objects/force.cpp \
    global_objects/equation.cpp \
    global_objects/derivative.cpp \
    global_objects/body.cpp

HEADERS += \
    derived_outputs/outputsbody.h \
    derived_outputs/outputderived.h \
    derived_outputs/globalvelocity.h \
    derived_outputs/globalsolution.h \
    derived_outputs/globalmotion.h \
    derived_outputs/globalacceleration.h \
    file_writer/filewriter.h \
    global_objects/system.h \
    motion_solver/motionsolver.h \
    wave_calcs/wavespreadmodel.h \
    wave_calcs/wavespectrummodel.h \
    wave_calcs/seaenviroment.h \
    motion_solver/matbody.h \
    motion_model/equationofmotion.h \
    motion_model/motionmodel.h \
    global_objects/solution.h \
    file_reader/filereader.h \
    file_reader/parser.h \
    file_reader/objectgroup.h \
    file_reader/dictionary.h \
    file_reader/dictcontrol.h \
    file_reader/dictforces.h \
    file_reader/dictbodies.h \
    global_objects/solutionset.h \
    motion_solver/matforcecross.h \
    motion_solver/matforceactive.h \
    motion_solver/matforcereact.h \
    global_objects/ofreqcore.h \
    global_objects/forcereact.h \
    global_objects/forcecross.h \
    global_objects/forceactive.h \
    global_objects/force.h \
    global_objects/equation.h \
    global_objects/derivative.h \
    global_objects/body.h

# Include header files path for libraries
INCLUDEPATH += \
    $$PWD/../../../lib \
    $$PWD/../../../var

# force rebuild if the headers change
DEPENDPATH += $${INCLUDEPATH}

OTHER_FILES += \
    ../../var/openseaheader.txt

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
