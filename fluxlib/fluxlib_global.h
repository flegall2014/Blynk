#ifndef FLUXLIB_GLOBAL_H
#define FLUXLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FLUXLIB_LIBRARY)
#  define FLUXLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FLUXLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FLUXLIB_GLOBAL_H
