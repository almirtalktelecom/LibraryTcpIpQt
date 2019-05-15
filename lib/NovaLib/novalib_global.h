#ifndef NOVALIB_GLOBAL_H
#define NOVALIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NOVALIB_LIBRARY)
#  define NOVALIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NOVALIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NOVALIB_GLOBAL_H
