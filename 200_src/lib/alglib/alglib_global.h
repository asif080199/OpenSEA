#ifndef ALGLIB_GLOBAL_H
#define ALGLIB_GLOBAL_H

#include <QtGlobal>

#ifdef Q_OS_WIN
    //Windows location of Qt file

#elif defined Q_OS_LINUX
    //Linux location of Qt file
    #include <QtCore/qglobal.h>
#endif


#if defined(ALGLIB_LIBRARY)
#  define ALGLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALGLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ALGLIB_GLOBAL_H
