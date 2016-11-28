/****************************************************************************
** Meta object code from reading C++ file 'BioTracker3Player.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/Model/BioTracker3Player.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BioTracker3Player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BioTracker3Player_t {
    QByteArrayData data[34];
    char stringdata0[527];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BioTracker3Player_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BioTracker3Player_t qt_meta_stringdata_BioTracker3Player = {
    {
QT_MOC_LITERAL(0, 0, 17), // "BioTracker3Player"
QT_MOC_LITERAL(1, 18, 13), // "emitMediaType"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 19), // "GuiParam::MediaType"
QT_MOC_LITERAL(4, 53, 4), // "type"
QT_MOC_LITERAL(5, 58, 19), // "emitTotalNumbFrames"
QT_MOC_LITERAL(6, 78, 6), // "size_t"
QT_MOC_LITERAL(7, 85, 3), // "num"
QT_MOC_LITERAL(8, 89, 22), // "emitCurrentFrameNumber"
QT_MOC_LITERAL(9, 112, 7), // "emitFPS"
QT_MOC_LITERAL(10, 120, 3), // "fps"
QT_MOC_LITERAL(11, 124, 19), // "emitCurrentFileName"
QT_MOC_LITERAL(12, 144, 4), // "name"
QT_MOC_LITERAL(13, 149, 16), // "emitCurrentFrame"
QT_MOC_LITERAL(14, 166, 24), // "std::shared_ptr<cv::Mat>"
QT_MOC_LITERAL(15, 191, 3), // "mat"
QT_MOC_LITERAL(16, 195, 24), // "emitVideoControllsStates"
QT_MOC_LITERAL(17, 220, 13), // "QVector<bool>"
QT_MOC_LITERAL(18, 234, 6), // "states"
QT_MOC_LITERAL(19, 241, 23), // "emitPlayerOperationDone"
QT_MOC_LITERAL(20, 265, 18), // "runPlayerOperation"
QT_MOC_LITERAL(21, 284, 23), // "receiveLoadVideoCommand"
QT_MOC_LITERAL(22, 308, 7), // "fileDir"
QT_MOC_LITERAL(23, 316, 19), // "receiveLoadPictures"
QT_MOC_LITERAL(24, 336, 36), // "std::vector<boost::filesystem..."
QT_MOC_LITERAL(25, 373, 5), // "files"
QT_MOC_LITERAL(26, 379, 23), // "receiveLoadCameraDevice"
QT_MOC_LITERAL(27, 403, 1), // "i"
QT_MOC_LITERAL(28, 405, 23), // "receivePrevFrameCommand"
QT_MOC_LITERAL(29, 429, 22), // "receiveNextFramCommand"
QT_MOC_LITERAL(30, 452, 19), // "receivePauseCommand"
QT_MOC_LITERAL(31, 472, 18), // "receiveStopCommand"
QT_MOC_LITERAL(32, 491, 18), // "receivePlayCommand"
QT_MOC_LITERAL(33, 510, 16) // "receiveStateDone"

    },
    "BioTracker3Player\0emitMediaType\0\0"
    "GuiParam::MediaType\0type\0emitTotalNumbFrames\0"
    "size_t\0num\0emitCurrentFrameNumber\0"
    "emitFPS\0fps\0emitCurrentFileName\0name\0"
    "emitCurrentFrame\0std::shared_ptr<cv::Mat>\0"
    "mat\0emitVideoControllsStates\0QVector<bool>\0"
    "states\0emitPlayerOperationDone\0"
    "runPlayerOperation\0receiveLoadVideoCommand\0"
    "fileDir\0receiveLoadPictures\0"
    "std::vector<boost::filesystem::path>\0"
    "files\0receiveLoadCameraDevice\0i\0"
    "receivePrevFrameCommand\0receiveNextFramCommand\0"
    "receivePauseCommand\0receiveStopCommand\0"
    "receivePlayCommand\0receiveStateDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BioTracker3Player[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,
       5,    1,  107,    2, 0x06 /* Public */,
       8,    1,  110,    2, 0x06 /* Public */,
       9,    1,  113,    2, 0x06 /* Public */,
      11,    1,  116,    2, 0x06 /* Public */,
      13,    2,  119,    2, 0x06 /* Public */,
      16,    1,  124,    2, 0x06 /* Public */,
      19,    0,  127,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      20,    0,  128,    2, 0x0a /* Public */,
      21,    1,  129,    2, 0x0a /* Public */,
      23,    1,  132,    2, 0x0a /* Public */,
      26,    1,  135,    2, 0x0a /* Public */,
      28,    0,  138,    2, 0x0a /* Public */,
      29,    0,  139,    2, 0x0a /* Public */,
      30,    0,  140,    2, 0x0a /* Public */,
      31,    0,  141,    2, 0x0a /* Public */,
      32,    0,  142,    2, 0x0a /* Public */,
      33,    0,  143,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, 0x80000000 | 14, QMetaType::QString,   15,   12,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BioTracker3Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BioTracker3Player *_t = static_cast<BioTracker3Player *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitMediaType((*reinterpret_cast< GuiParam::MediaType(*)>(_a[1]))); break;
        case 1: _t->emitTotalNumbFrames((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 2: _t->emitCurrentFrameNumber((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 3: _t->emitFPS((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->emitCurrentFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->emitCurrentFrame((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->emitVideoControllsStates((*reinterpret_cast< QVector<bool>(*)>(_a[1]))); break;
        case 7: _t->emitPlayerOperationDone(); break;
        case 8: _t->runPlayerOperation(); break;
        case 9: _t->receiveLoadVideoCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->receiveLoadPictures((*reinterpret_cast< std::vector<boost::filesystem::path>(*)>(_a[1]))); break;
        case 11: _t->receiveLoadCameraDevice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->receivePrevFrameCommand(); break;
        case 13: _t->receiveNextFramCommand(); break;
        case 14: _t->receivePauseCommand(); break;
        case 15: _t->receiveStopCommand(); break;
        case 16: _t->receivePlayCommand(); break;
        case 17: _t->receiveStateDone(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<bool> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BioTracker3Player::*_t)(GuiParam::MediaType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTracker3Player::emitMediaType)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (BioTracker3Player::*_t)(size_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTracker3Player::emitTotalNumbFrames)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (BioTracker3Player::*_t)(size_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTracker3Player::emitCurrentFrameNumber)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (BioTracker3Player::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTracker3Player::emitFPS)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (BioTracker3Player::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTracker3Player::emitCurrentFileName)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (BioTracker3Player::*_t)(std::shared_ptr<cv::Mat> , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTracker3Player::emitCurrentFrame)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (BioTracker3Player::*_t)(QVector<bool> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTracker3Player::emitVideoControllsStates)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (BioTracker3Player::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTracker3Player::emitPlayerOperationDone)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject BioTracker3Player::staticMetaObject = {
    { &IModel::staticMetaObject, qt_meta_stringdata_BioTracker3Player.data,
      qt_meta_data_BioTracker3Player,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BioTracker3Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BioTracker3Player::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BioTracker3Player.stringdata0))
        return static_cast<void*>(const_cast< BioTracker3Player*>(this));
    return IModel::qt_metacast(_clname);
}

int BioTracker3Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void BioTracker3Player::emitMediaType(GuiParam::MediaType _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BioTracker3Player::emitTotalNumbFrames(size_t _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BioTracker3Player::emitCurrentFrameNumber(size_t _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BioTracker3Player::emitFPS(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BioTracker3Player::emitCurrentFileName(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BioTracker3Player::emitCurrentFrame(std::shared_ptr<cv::Mat> _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void BioTracker3Player::emitVideoControllsStates(QVector<bool> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void BioTracker3Player::emitPlayerOperationDone()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
