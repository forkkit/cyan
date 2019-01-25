#ifndef CYAN_COMMON_GLOBAL_H
#define CYAN_COMMON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CYAN_COMMON_LIBRARY)
#  define CYAN_COMMON_EXPORT Q_DECL_EXPORT
#else
#  define CYAN_COMMON_EXPORT Q_DECL_IMPORT
#endif

#if defined(CYAN_TILEITEM_LIBRARY)
#  define CYAN_TILEITEM_EXPORT Q_DECL_EXPORT
#else
#  define CYAN_TILEITEM_EXPORT Q_DECL_IMPORT
#endif

#if defined(CYAN_LAYERITEM_LIBRARY)
#  define CYAN_LAYERITEM_EXPORT Q_DECL_EXPORT
#else
#  define CYAN_LAYERITEM_EXPORT Q_DECL_IMPORT
#endif

#if defined(CYAN_RENDER_LIBRARY)
#  define CYAN_RENDER_EXPORT Q_DECL_EXPORT
#else
#  define CYAN_RENDER_EXPORT Q_DECL_IMPORT
#endif

#if defined(CYAN_VIEW_LIBRARY)
#  define CYAN_VIEW_EXPORT Q_DECL_EXPORT
#else
#  define CYAN_VIEW_EXPORT Q_DECL_IMPORT
#endif

#endif // CYAN_COMMON_GLOBAL_H
