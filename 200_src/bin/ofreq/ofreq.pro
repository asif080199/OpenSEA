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
    file_reader/dictseaenv.cpp \
    sea_models/seamodel_longcrest.cpp \
    sea_models/seamodel_uniform.cpp \
    hydro_data/hydrodata.cpp \
    global_objects/mathinterp.cpp \
    hydro_reader/hydroreader.cpp \
    hydro_data/hydromanager.cpp \
    hydro_reader/dicthydrodirection.cpp \
    hydro_reader/dicthydroenv.cpp \
    hydro_reader/dicthydrofrequency.cpp \
    hydro_reader/dictcrossdamp.cpp \
    hydro_reader/dictcrossmass.cpp \
    hydro_reader/dictcrossstiff.cpp \
    hydro_reader/dictforceexcite.cpp \
    hydro_reader/dicthydrodamp.cpp \
    hydro_reader/dicthydromass.cpp \
    hydro_reader/dicthydrostiff.cpp \
    hydro_reader/dicthydroreact.cpp \
    hydro_reader/dictcrossreact.cpp \
    file_reader/dictdata.cpp \
    reports/data.cpp \
    reports/report.cpp \
    reports/reportmanager.cpp \
    reports/repdirections.cpp \
    reports/repfrequencies.cpp \
    file_reader/dictoutputs.cpp \
    reports/replocalsolution.cpp \
    reports/replocalacceleration.cpp \
    reports/repbodacceleration.cpp \
    reports/repbodmotion.cpp \
    reports/repbodsolution.cpp \
    reports/repbodvelocity.cpp \
    reports/repwavespectra.cpp \
    reports/repstaticbodyaccel.cpp \
    reports/repdesignaccel.cpp \
    reports/repdynfreeboard.cpp \
    reports/repbodforces.cpp \
    reports/repbodpower.cpp \
    reports/repcapturewidth.cpp \
    reports/repreleff.cpp \
    reports/repabseff.cpp

HEADERS += \
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
    file_reader/dictseaenv.h \
    sea_models/seamodel_longcrest.h \
    sea_models/seamodel_uniform.h \
    hydro_data/hydrodata.h \
    global_objects/mathinterp.h \
    hydro_reader/hydroreader.h \
    hydro_data/hydromanager.h \
    hydro_reader/dicthydrodirection.h \
    hydro_reader/dicthydroenv.h \
    hydro_reader/dicthydrofrequency.h \
    hydro_reader/dictcrossdamp.h \
    hydro_reader/dictcrossmass.h \
    hydro_reader/dictcrossstiff.h \
    hydro_reader/dictforceexcite.h \
    hydro_reader/dicthydrodamp.h \
    hydro_reader/dicthydromass.h \
    hydro_reader/dicthydrostiff.h \
    hydro_reader/dicthydroreact.h \
    hydro_reader/dictcrossreact.h \
    file_reader/dictdata.h \
    reports/data.h \
    reports/report.h \
    reports/reportmanager.h \
    reports/repdirections.h \
    reports/repfrequencies.h \
    file_reader/dictoutputs.h \
    reports/replocalsolution.h \
    reports/replocalacceleration.h \
    reports/repbodacceleration.h \
    reports/repbodmotion.h \
    reports/repbodsolution.h \
    reports/repbodvelocity.h \
    reports/repwavespectra.h \
    reports/repstaticbodyaccel.h \
    reports/repdesignaccel.h \
    reports/repdynfreeboard.h \
    reports/repbodforces.h \
    reports/repbodpower.h \
    reports/repcapturewidth.h \
    reports/repreleff.h \
    reports/repabseff.h

# Include header files path for libraries
INCLUDEPATH += \
    $$_PRO_FILE_PWD_/../../lib \
    $$_PRO_FILE_PWD_/../../var \
    $$_PRO_FILE_PWD_/../../lib/alglib \

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
        DESTDIR = $$_PRO_FILE_PWD_/../../../300_build/310_build_windows/bin/ofreq
    }

    CONFIG(release) {
        LIBS += -L$$_PRO_FILE_PWD_/../../../300_build/310_build_windows/lib/alglib/release/ -lalglib
        DESTDIR = $$_PRO_FILE_PWD_/../../../300_build/310_build_windows/bin/ofreq
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
