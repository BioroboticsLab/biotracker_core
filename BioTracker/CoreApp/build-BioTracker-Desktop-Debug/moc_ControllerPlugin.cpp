/****************************************************************************
** Meta object code from reading C++ file 'ControllerPlugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/Controller/ControllerPlugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControllerPlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ControllerPlugin_t {
    QByteArrayData data[7];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControllerPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControllerPlugin_t qt_meta_stringdata_ControllerPlugin = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ControllerPlugin"
QT_MOC_LITERAL(1, 17, 19), // "receiveCurrentFrame"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 24), // "std::shared_ptr<cv::Mat>"
QT_MOC_LITERAL(4, 63, 3), // "mat"
QT_MOC_LITERAL(5, 67, 6), // "number"
QT_MOC_LITERAL(6, 74, 19) // "receiveTrackingDone"

    },
    "ControllerPlugin\0receiveCurrentFrame\0"
    "\0std::shared_ptr<cv::Mat>\0mat\0number\0"
    "receiveTrackingDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControllerPlugin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x08 /* Private */,
       6,    0,   29,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::UInt,    4,    5,
    QMetaType::Void,

       0        // eod
};

void ControllerPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ControllerPlugin *_t = static_cast<ControllerPlugin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->receiveCurrentFrame((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 1: _t->receiveTrackingDone(); break;
        default: ;
        }
    }
}

const QMetaObject ControllerPlugin::staticMetaObject = {
    { &IController::staticMetaObject, qt_meta_stringdata_ControllerPlugin.data,
      qt_meta_data_ControllerPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ControllerPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControllerPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ControllerPlugin.stringdata0))
        return static_cast<void*>(const_cast< ControllerPlugin*>(this));
    return IController::qt_metacast(_clname);
}

int ControllerPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
