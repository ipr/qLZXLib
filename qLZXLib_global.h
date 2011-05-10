#ifndef QLZXLIB_GLOBAL_H
#define QLZXLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QLZXLIB_LIBRARY)
#  define QLZXLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QLZXLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QLZXLIB_GLOBAL_H
