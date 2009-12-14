#ifndef CONFIG_H
#define CONFIG_H

#ifndef CANDEVICE_H
#include <QtGlobal>
#include <qwt_global.h>
#endif

//#define WINDOWS
#define LINUX


#if QT_VERSION < 0x040500 // needs Qt 4.5.0 or better
#error "Please use a more recent version of Qt"
#endif


#endif // CONFIG_H
