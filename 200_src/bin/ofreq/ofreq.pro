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

# Force C++11 build.  This is required by equantionofmotion.h and derived classes
# These derived classes may implement lambda functions.
QMAKE_CXXFLAGS += -std=c++11

SOURCES += ofreq.cpp \
    derived_outputs/outputsbody.cpp \
    derived_outputs/outputderived.cpp \
    derived_outputs/globalvelocity.cpp \
    derived_outputs/globalsolution.cpp \
    derived_outputs/globalmotion.cpp \
    derived_outputs/globalacceleration.cpp \
    file_writer/filewriter.cpp \
    motion_solver/motionsolver.cpp \
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
    global_objects/forcereact.cpp \
    global_objects/forcecross.cpp \
    global_objects/forceactive.cpp \
    global_objects/force.cpp \
    global_objects/equation.cpp \
    global_objects/derivative.cpp \
    global_objects/body.cpp \
    motion_model/eqntranslation.cpp \
    motion_model/eqnrotation.cpp \
    motion_model/model6dof.cpp \
    system_objects/system.cpp \
    system_objects/log.cpp \
    system_objects/ofreqcore.cpp \
    global_objects/ioword.cpp \
    wave_spectra/wavespec.cpp \
    sea_models/seamodel.cpp \
    wave_spectra/specpm.cpp \
    wave_spectra/wavespecbase.cpp \
    wave_spectra/specbretschneider.cpp \
    wave_spectra/specjonswap.cpp \
    sea_models/seamodel_singledirection.cpp \
    sea_models/seamodel_dualdirection.cpp \
    file_reader/dictseaenv.cpp

HEADERS += \
    derived_outputs/outputsbody.h \
    derived_outputs/outputderived.h \
    derived_outputs/globalvelocity.h \
    derived_outputs/globalsolution.h \
    derived_outputs/globalmotion.h \
    derived_outputs/globalacceleration.h \
    file_writer/filewriter.h \
    motion_solver/motionsolver.h \
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
    global_objects/forcereact.h \
    global_objects/forcecross.h \
    global_objects/forceactive.h \
    global_objects/force.h \
    global_objects/equation.h \
    global_objects/derivative.h \
    global_objects/body.h \
    motion_model/eqntranslation.h \
    motion_model/eqnrotation.h \
    motion_model/model6dof.h \
    system_objects/system.h \
    system_objects/log.h \
    system_objects/ofreqcore.h \
    global_objects/ioword.h \
    wave_spectra/wavespec.h \
    sea_models/seamodel.h \
    wave_spectra/specpm.h \
    wave_spectra/wavespecbase.h \
    wave_spectra/specbretschneider.h \
    wave_spectra/specjonswap.h \
    sea_models/seamodel_singledirection.h \
    sea_models/seamodel_dualdirection.h \
    file_reader/dictseaenv.h

# Include header files path for libraries
INCLUDEPATH += \
    $$_PRO_FILE_PWD_/../../lib \
    $$_PRO_FILE_PWD_/../../var \
    $$_PRO_FILE_PWD_/../../lib/alglib \
    #$$_PRO_FILE_PWD_/../../lib/boost

# force rebuild if the headers change
DEPENDPATH += $${INCLUDEPATH} \
    $$_PRO_FILE_PWD_/../../lib/alglib


# Platform Specific files go in these scope brackets
# ========================================================
win32 {
    # Any files specific to windows go in these brackets.
    LIBS += \
        $$_PRO_FILE_PWD_/../../lib/armadillo.h \        #Add armadillo and associated support
        $$_PRO_FILE_PWD_/../../lib/blas_win32_MT.lib \
        $$_PRO_FILE_PWD_/../../lib/lapack_win32_MT.lib
    #LIBS += boost_system boost_filesystem   # Add boost and boost filesystem
    CONFIG(debug) {
        LIBS += -L$$_PRO_FILE_PWD_/../../../300_build/310_build_windows/lib/alglib/debug/ -lalglib
        DESTDIR = $$_PRO_FILE_PWD_/../../../300_build/310_build_windows/bin/ofreq_debug
    }

    CONFIG(release) {
        LIBS += -L$$_PRO_FILE_PWD_/../../../300_build/310_build_windows/lib/alglib/release/ -lalglib
        DESTDIR = $$_PRO_FILE_PWD_/../../../300_build/310_build_windows/bin/ofreq_release
    }

}

unix {
    # Any files specific to linux go in these brackets.
    LIBS += -larmadillo -llapack -lblas #Add armadillo and associated support
    #LIBS += -lboost_system -lboost_filesystem   # Add boost and boost filesystem
    LIBS += -L$$PWD/../../../300_build/320_build_linux/lib/alglib/ -lalglib #AGLIB library for interpolation.
}

OTHER_FILES += \
    ../../var/openseaheader.txt
