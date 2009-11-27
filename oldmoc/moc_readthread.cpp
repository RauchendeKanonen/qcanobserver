/****************************************************************************
** Meta object code from reading C++ file 'readthread.h'
**
** Created: Wed Nov 11 09:39:58 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "readthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'readthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReadThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      20,   12,   11,   11, 0x0a,
      36,   11,   11,   11, 0x0a,
      51,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ReadThread[] = {
    "ReadThread\0\0PathArg\0setDev(QString)\0"
    "startReading()\0stopReading()\0"
};

const QMetaObject ReadThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ReadThread,
      qt_meta_data_ReadThread, 0 }
};

const QMetaObject *ReadThread::metaObject() const
{
    return &staticMetaObject;
}

void *ReadThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReadThread))
        return static_cast<void*>(const_cast< ReadThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ReadThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setDev((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: startReading(); break;
        case 2: stopReading(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
