/****************************************************************************
** Meta object code from reading C++ file 'ControllerPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/Controller/ControllerPlayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControllerPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ControllerPlayer_t {
    QByteArrayData data[25];
    char stringdata0[390];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControllerPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControllerPlayer_t qt_meta_stringdata_ControllerPlayer = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ControllerPlayer"
QT_MOC_LITERAL(1, 17, 19), // "emitLoadVideoStream"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 3), // "str"
QT_MOC_LITERAL(4, 42, 16), // "emitLoadPictures"
QT_MOC_LITERAL(5, 59, 36), // "std::vector<boost::filesystem..."
QT_MOC_LITERAL(6, 96, 5), // "files"
QT_MOC_LITERAL(7, 102, 20), // "emitLoadCameraDevice"
QT_MOC_LITERAL(8, 123, 1), // "i"
QT_MOC_LITERAL(9, 125, 20), // "emitNextFrameCommand"
QT_MOC_LITERAL(10, 146, 20), // "emitPrevFrameCommand"
QT_MOC_LITERAL(11, 167, 15), // "emitPlayCommand"
QT_MOC_LITERAL(12, 183, 15), // "emitStopCommand"
QT_MOC_LITERAL(13, 199, 16), // "emitPauseCommand"
QT_MOC_LITERAL(14, 216, 22), // "emitRunPlayerOperation"
QT_MOC_LITERAL(15, 239, 26), // "receivePlayerOperationDone"
QT_MOC_LITERAL(16, 266, 25), // "receiveCurrentFrameNumber"
QT_MOC_LITERAL(17, 292, 6), // "size_t"
QT_MOC_LITERAL(18, 299, 3), // "num"
QT_MOC_LITERAL(19, 303, 10), // "receiveFPS"
QT_MOC_LITERAL(20, 314, 3), // "fps"
QT_MOC_LITERAL(21, 318, 22), // "receiveTotalNumbFrames"
QT_MOC_LITERAL(22, 341, 27), // "receiveVideoControllsStates"
QT_MOC_LITERAL(23, 369, 13), // "QVector<bool>"
QT_MOC_LITERAL(24, 383, 6) // "states"

    },
    "ControllerPlayer\0emitLoadVideoStream\0"
    "\0str\0emitLoadPictures\0"
    "std::vector<boost::filesystem::path>\0"
    "files\0emitLoadCameraDevice\0i\0"
    "emitNextFrameCommand\0emitPrevFrameCommand\0"
    "emitPlayCommand\0emitStopCommand\0"
    "emitPauseCommand\0emitRunPlayerOperation\0"
    "receivePlayerOperationDone\0"
    "receiveCurrentFrameNumber\0size_t\0num\0"
    "receiveFPS\0fps\0receiveTotalNumbFrames\0"
    "receiveVideoControllsStates\0QVector<bool>\0"
    "states"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControllerPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       7,    1,   90,    2, 0x06 /* Public */,
       9,    0,   93,    2, 0x06 /* Public */,
      10,    0,   94,    2, 0x06 /* Public */,
      11,    0,   95,    2, 0x06 /* Public */,
      12,    0,   96,    2, 0x06 /* Public */,
      13,    0,   97,    2, 0x06 /* Public */,
      14,    0,   98,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,   99,    2, 0x0a /* Public */,
      16,    1,  100,    2, 0x0a /* Public */,
      19,    1,  103,    2, 0x0a /* Public */,
      21,    1,  106,    2, 0x0a /* Public */,
      22,    1,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, QMetaType::Double,   20,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 23,   24,

       0        // eod
};

void ControllerPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ControllerPlayer *_t = static_cast<ControllerPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitLoadVideoStream((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->emitLoadPictures((*reinterpret_cast< std::vector<boost::filesystem::path>(*)>(_a[1]))); break;
        case 2: _t->emitLoadCameraDevice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->emitNextFrameCommand(); break;
        case 4: _t->emitPrevFrameCommand(); break;
        case 5: _t->emitPlayCommand(); break;
        case 6: _t->emitStopCommand(); break;
        case 7: _t->emitPauseCommand(); break;
        case 8: _t->emitRunPlayerOperation(); break;
        case 9: _t->receivePlayerOperationDone(); break;
        case 10: _t->receiveCurrentFrameNumber((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 11: _t->receiveFPS((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->receiveTotalNumbFrames((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 13: _t->receiveVideoControllsStates((*reinterpret_cast< QVector<bool>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
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
            typedef void (ControllerPlayer::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitLoadVideoStream)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ControllerPlayer::*_t)(std::vector<boost::filesystem::path> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitLoadPictures)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ControllerPlayer::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitLoadCameraDevice)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ControllerPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitNextFrameCommand)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ControllerPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitPrevFrameCommand)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (ControllerPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitPlayCommand)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (ControllerPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitStopCommand)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (ControllerPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitPauseCommand)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (ControllerPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ControllerPlayer::emitRunPlayerOperation)) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject ControllerPlayer::staticMetaObject = {
    { &IController::staticMetaObject, qt_meta_stringdata_ControllerPlayer.data,
      qt_meta_data_ControllerPlayer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ControllerPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControllerPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ControllerPlayer.stringdata0))
        return static_cast<void*>(const_cast< ControllerPlayer*>(this));
    return IController::qt_metacast(_clname);
}

int ControllerPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IController::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void ControllerPlayer::emitLoadVideoStream(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControllerPlayer::emitLoadPictures(std::vector<boost::filesystem::path> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ControllerPlayer::emitLoadCameraDevice(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ControllerPlayer::emitNextFrameCommand()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void ControllerPlayer::emitPrevFrameCommand()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void ControllerPlayer::emitPlayCommand()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void ControllerPlayer::emitStopCommand()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void ControllerPlayer::emitPauseCommand()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}

// SIGNAL 8
void ControllerPlayer::emitRunPlayerOperation()
{
    QMetaObject::activate(this, &staticMetaObject, 8, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
