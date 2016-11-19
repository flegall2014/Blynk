#ifndef BLRLIB_GLOBAL_H
#define BLRLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BLRLIB_LIBRARY)
#  define BLRLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BLRLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BLRLIB_GLOBAL_H
