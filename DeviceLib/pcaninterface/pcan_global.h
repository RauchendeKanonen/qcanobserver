#ifndef PCAN_GLOBAL_H
#define PCAN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PCAN_LIBRARY)
#  define PCANSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PCANSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PCAN_GLOBAL_H
