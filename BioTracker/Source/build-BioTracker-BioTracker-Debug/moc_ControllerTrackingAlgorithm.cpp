/****************************************************************************
** Meta object code from reading C++ file 'ControllerTrackingAlgorithm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/Controller/ControllerTrackingAlgorithm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControllerTrackingAlgorithm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ControllerTrackingAlgorithm_t {
    QByteArrayData data[7];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControllerTrackingAlgorithm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControllerTrackingAlgorithm_t qt_meta_stringdata_ControllerTrackingAlgorithm = {
    {
QT_MOC_LITERAL(0, 0, 27), // "ControllerTrackingAlgorithm"
QT_MOC_LITERAL(1, 28, 10), // "emitCvMatA"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 24), // "std::shared_ptr<cv::Mat>"
QT_MOC_LITERAL(4, 65, 3), // "mat"
QT_MOC_LITERAL(5, 69, 4), // "name"
QT_MOC_LITERAL(6, 74, 13) // "receiveCvMatA"

    },
    "ControllerTrackingAlgorithm\0emitCvMatA\0"
    "\0std::shared_ptr<cv::Mat>\0mat\0name\0"
    "receiveCvMatA"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControllerTrackingAlgorithm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,

       0        // eod
};

void ControllerTrackingAlgorithm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ControllerTrackingAlgorithm *_t = static_cast<ControllerTrackingAlgorithm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitCvMatA((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->receiveCvMatA((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ControllerTrackingAlgorithm::*_t)(std::shared_ptr<cv::Mat> , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerTrackingAlgorithm::emitCvMatA)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ControllerTrackingAlgorithm::staticMetaObject = {
    { &IController::staticMetaObject, qt_meta_stringdata_ControllerTrackingAlgorithm.data,
      qt_meta_data_ControllerTrackingAlgorithm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ControllerTrackingAlgorithm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControllerTrackingAlgorithm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ControllerTrackingAlgorithm.stringdata0))
        return static_cast<void*>(const_cast< ControllerTrackingAlgorithm*>(this));
    return IController::qt_metacast(_clname);
}

int ControllerTrackingAlgorithm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void ControllerTrackingAlgorithm::emitCvMatA(std::shared_ptr<cv::Mat> _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
