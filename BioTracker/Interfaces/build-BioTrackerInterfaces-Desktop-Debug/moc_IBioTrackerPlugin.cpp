/****************************************************************************
** Meta object code from reading C++ file 'IBioTrackerPlugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTrackerInterfaces/Interfaces/IBioTrackerPlugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IBioTrackerPlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IBioTrackerPlugin_t {
    QByteArrayData data[10];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IBioTrackerPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IBioTrackerPlugin_t qt_meta_stringdata_IBioTrackerPlugin = {
    {
QT_MOC_LITERAL(0, 0, 17), // "IBioTrackerPlugin"
QT_MOC_LITERAL(1, 18, 9), // "emitCvMat"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 24), // "std::shared_ptr<cv::Mat>"
QT_MOC_LITERAL(4, 54, 3), // "mat"
QT_MOC_LITERAL(5, 58, 4), // "name"
QT_MOC_LITERAL(6, 63, 16), // "emitTrackingDone"
QT_MOC_LITERAL(7, 80, 12), // "receiveCvMat"
QT_MOC_LITERAL(8, 93, 11), // "frameNumber"
QT_MOC_LITERAL(9, 105, 26) // "receiveCvMatFromController"

    },
    "IBioTrackerPlugin\0emitCvMat\0\0"
    "std::shared_ptr<cv::Mat>\0mat\0name\0"
    "emitTrackingDone\0receiveCvMat\0frameNumber\0"
    "receiveCvMatFromController"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IBioTrackerPlugin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       6,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   40,    2, 0x0a /* Public */,
       9,    2,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::UInt,    4,    8,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,

       0        // eod
};

void IBioTrackerPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IBioTrackerPlugin *_t = static_cast<IBioTrackerPlugin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitCvMat((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->emitTrackingDone(); break;
        case 2: _t->receiveCvMat((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 3: _t->receiveCvMatFromController((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IBioTrackerPlugin::*_t)(std::shared_ptr<cv::Mat> , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IBioTrackerPlugin::emitCvMat)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (IBioTrackerPlugin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IBioTrackerPlugin::emitTrackingDone)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject IBioTrackerPlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IBioTrackerPlugin.data,
      qt_meta_data_IBioTrackerPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IBioTrackerPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IBioTrackerPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IBioTrackerPlugin.stringdata0))
        return static_cast<void*>(const_cast< IBioTrackerPlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int IBioTrackerPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
