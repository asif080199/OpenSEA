#ifndef ALGLIB_GLOBAL_H
#define ALGLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ALGLIB_LIBRARY)
#  define ALGLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALGLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ALGLIB_GLOBAL_H
