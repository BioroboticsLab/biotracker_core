/****************************************************************************
** Meta object code from reading C++ file 'ENUMS.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTrackerInterfaces/Interfaces/ENUMS.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ENUMS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ENUMS_t {
    QByteArrayData data[10];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ENUMS_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ENUMS_t qt_meta_stringdata_ENUMS = {
    {
QT_MOC_LITERAL(0, 0, 5), // "ENUMS"
QT_MOC_LITERAL(1, 6, 14), // "CONTROLLERTYPE"
QT_MOC_LITERAL(2, 21, 6), // "NO_CTR"
QT_MOC_LITERAL(3, 28, 10), // "MAINWINDOW"
QT_MOC_LITERAL(4, 39, 13), // "TEXTUREOBJECT"
QT_MOC_LITERAL(5, 53, 6), // "PLAYER"
QT_MOC_LITERAL(6, 60, 8), // "TRACKING"
QT_MOC_LITERAL(7, 69, 9), // "COMPONENT"
QT_MOC_LITERAL(8, 79, 12), // "GRAPHICSVIEW"
QT_MOC_LITERAL(9, 92, 6) // "PLUGIN"

    },
    "ENUMS\0CONTROLLERTYPE\0NO_CTR\0MAINWINDOW\0"
    "TEXTUREOBJECT\0PLAYER\0TRACKING\0COMPONENT\0"
    "GRAPHICSVIEW\0PLUGIN"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ENUMS[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,    8,   18,

 // enum data: key, value
       2, uint(ENUMS::NO_CTR),
       3, uint(ENUMS::MAINWINDOW),
       4, uint(ENUMS::TEXTUREOBJECT),
       5, uint(ENUMS::PLAYER),
       6, uint(ENUMS::TRACKING),
       7, uint(ENUMS::COMPONENT),
       8, uint(ENUMS::GRAPHICSVIEW),
       9, uint(ENUMS::PLUGIN),

       0        // eod
};

void ENUMS::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ENUMS::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ENUMS.data,
      qt_meta_data_ENUMS,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ENUMS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ENUMS::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ENUMS.stringdata0))
        return static_cast<void*>(const_cast< ENUMS*>(this));
    return QObject::qt_metacast(_clname);
}

int ENUMS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
