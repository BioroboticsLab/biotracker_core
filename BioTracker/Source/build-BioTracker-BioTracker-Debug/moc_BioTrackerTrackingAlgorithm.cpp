/****************************************************************************
** Meta object code from reading C++ file 'BioTrackerTrackingAlgorithm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/Model/BioTrackerTrackingAlgorithm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BioTrackerTrackingAlgorithm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BioTrackerTrackingAlgorithm_t {
    QByteArrayData data[7];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BioTrackerTrackingAlgorithm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BioTrackerTrackingAlgorithm_t qt_meta_stringdata_BioTrackerTrackingAlgorithm = {
    {
QT_MOC_LITERAL(0, 0, 27), // "BioTrackerTrackingAlgorithm"
QT_MOC_LITERAL(1, 28, 10), // "emitCvMatA"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 24), // "std::shared_ptr<cv::Mat>"
QT_MOC_LITERAL(4, 65, 5), // "image"
QT_MOC_LITERAL(5, 71, 4), // "name"
QT_MOC_LITERAL(6, 76, 10) // "doTracking"

    },
    "BioTrackerTrackingAlgorithm\0emitCvMatA\0"
    "\0std::shared_ptr<cv::Mat>\0image\0name\0"
    "doTracking"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BioTrackerTrackingAlgorithm[] = {

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
       6,    1,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void BioTrackerTrackingAlgorithm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BioTrackerTrackingAlgorithm *_t = static_cast<BioTrackerTrackingAlgorithm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitCvMatA((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->doTracking((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BioTrackerTrackingAlgorithm::*_t)(std::shared_ptr<cv::Mat> , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTrackerTrackingAlgorithm::emitCvMatA)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject BioTrackerTrackingAlgorithm::staticMetaObject = {
    { &ITrackingAlgorithm::staticMetaObject, qt_meta_stringdata_BioTrackerTrackingAlgorithm.data,
      qt_meta_data_BioTrackerTrackingAlgorithm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BioTrackerTrackingAlgorithm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BioTrackerTrackingAlgorithm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BioTrackerTrackingAlgorithm.stringdata0))
        return static_cast<void*>(const_cast< BioTrackerTrackingAlgorithm*>(this));
    return ITrackingAlgorithm::qt_metacast(_clname);
}

int BioTrackerTrackingAlgorithm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ITrackingAlgorithm::qt_metacall(_c, _id, _a);
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
void BioTrackerTrackingAlgorithm::emitCvMatA(std::shared_ptr<cv::Mat> _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
