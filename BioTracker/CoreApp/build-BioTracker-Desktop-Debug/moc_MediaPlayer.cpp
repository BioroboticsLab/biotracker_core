/****************************************************************************
** Meta object code from reading C++ file 'MediaPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/Model/MediaPlayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MediaPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MediaPlayer_t {
    QByteArrayData data[41];
    char stringdata0[650];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MediaPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MediaPlayer_t qt_meta_stringdata_MediaPlayer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MediaPlayer"
QT_MOC_LITERAL(1, 12, 13), // "emitMediaType"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 19), // "GuiParam::MediaType"
QT_MOC_LITERAL(4, 47, 4), // "type"
QT_MOC_LITERAL(5, 52, 19), // "emitTotalNumbFrames"
QT_MOC_LITERAL(6, 72, 6), // "size_t"
QT_MOC_LITERAL(7, 79, 3), // "num"
QT_MOC_LITERAL(8, 83, 22), // "emitCurrentFrameNumber"
QT_MOC_LITERAL(9, 106, 7), // "emitFPS"
QT_MOC_LITERAL(10, 114, 3), // "fps"
QT_MOC_LITERAL(11, 118, 19), // "emitCurrentFileName"
QT_MOC_LITERAL(12, 138, 4), // "name"
QT_MOC_LITERAL(13, 143, 19), // "emitCurrentFrameStr"
QT_MOC_LITERAL(14, 163, 24), // "std::shared_ptr<cv::Mat>"
QT_MOC_LITERAL(15, 188, 3), // "mat"
QT_MOC_LITERAL(16, 192, 16), // "emitCurrentFrame"
QT_MOC_LITERAL(17, 209, 6), // "number"
QT_MOC_LITERAL(18, 216, 24), // "emitVideoControllsStates"
QT_MOC_LITERAL(19, 241, 13), // "QVector<bool>"
QT_MOC_LITERAL(20, 255, 6), // "states"
QT_MOC_LITERAL(21, 262, 25), // "emitTrackingIsActiveState"
QT_MOC_LITERAL(22, 288, 5), // "state"
QT_MOC_LITERAL(23, 294, 23), // "emitPlayerOperationDone"
QT_MOC_LITERAL(24, 318, 18), // "runPlayerOperation"
QT_MOC_LITERAL(25, 337, 23), // "receiveLoadVideoCommand"
QT_MOC_LITERAL(26, 361, 7), // "fileDir"
QT_MOC_LITERAL(27, 369, 19), // "receiveLoadPictures"
QT_MOC_LITERAL(28, 389, 36), // "std::vector<boost::filesystem..."
QT_MOC_LITERAL(29, 426, 5), // "files"
QT_MOC_LITERAL(30, 432, 23), // "receiveLoadCameraDevice"
QT_MOC_LITERAL(31, 456, 1), // "i"
QT_MOC_LITERAL(32, 458, 23), // "receiveActivateTracking"
QT_MOC_LITERAL(33, 482, 25), // "receiveDeaktivateTracking"
QT_MOC_LITERAL(34, 508, 23), // "receivePrevFrameCommand"
QT_MOC_LITERAL(35, 532, 22), // "receiveNextFramCommand"
QT_MOC_LITERAL(36, 555, 19), // "receivePauseCommand"
QT_MOC_LITERAL(37, 575, 18), // "receiveStopCommand"
QT_MOC_LITERAL(38, 594, 18), // "receivePlayCommand"
QT_MOC_LITERAL(39, 613, 16), // "receiveStateDone"
QT_MOC_LITERAL(40, 630, 19) // "receiveTrackingDone"

    },
    "MediaPlayer\0emitMediaType\0\0"
    "GuiParam::MediaType\0type\0emitTotalNumbFrames\0"
    "size_t\0num\0emitCurrentFrameNumber\0"
    "emitFPS\0fps\0emitCurrentFileName\0name\0"
    "emitCurrentFrameStr\0std::shared_ptr<cv::Mat>\0"
    "mat\0emitCurrentFrame\0number\0"
    "emitVideoControllsStates\0QVector<bool>\0"
    "states\0emitTrackingIsActiveState\0state\0"
    "emitPlayerOperationDone\0runPlayerOperation\0"
    "receiveLoadVideoCommand\0fileDir\0"
    "receiveLoadPictures\0"
    "std::vector<boost::filesystem::path>\0"
    "files\0receiveLoadCameraDevice\0i\0"
    "receiveActivateTracking\0"
    "receiveDeaktivateTracking\0"
    "receivePrevFrameCommand\0receiveNextFramCommand\0"
    "receivePauseCommand\0receiveStopCommand\0"
    "receivePlayCommand\0receiveStateDone\0"
    "receiveTrackingDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MediaPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  129,    2, 0x06 /* Public */,
       5,    1,  132,    2, 0x06 /* Public */,
       8,    1,  135,    2, 0x06 /* Public */,
       9,    1,  138,    2, 0x06 /* Public */,
      11,    1,  141,    2, 0x06 /* Public */,
      13,    2,  144,    2, 0x06 /* Public */,
      16,    2,  149,    2, 0x06 /* Public */,
      18,    1,  154,    2, 0x06 /* Public */,
      21,    1,  157,    2, 0x06 /* Public */,
      23,    0,  160,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      24,    0,  161,    2, 0x0a /* Public */,
      25,    1,  162,    2, 0x0a /* Public */,
      27,    1,  165,    2, 0x0a /* Public */,
      30,    1,  168,    2, 0x0a /* Public */,
      32,    0,  171,    2, 0x0a /* Public */,
      33,    0,  172,    2, 0x0a /* Public */,
      34,    0,  173,    2, 0x0a /* Public */,
      35,    0,  174,    2, 0x0a /* Public */,
      36,    0,  175,    2, 0x0a /* Public */,
      37,    0,  176,    2, 0x0a /* Public */,
      38,    0,  177,    2, 0x0a /* Public */,
      39,    0,  178,    2, 0x0a /* Public */,
      40,    0,  179,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Double,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, 0x80000000 | 14, QMetaType::QString,   15,   12,
    QMetaType::Void, 0x80000000 | 14, QMetaType::UInt,   15,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::Bool,   22,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void, QMetaType::Int,   31,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MediaPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MediaPlayer *_t = static_cast<MediaPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitMediaType((*reinterpret_cast< GuiParam::MediaType(*)>(_a[1]))); break;
        case 1: _t->emitTotalNumbFrames((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 2: _t->emitCurrentFrameNumber((*reinterpret_cast< size_t(*)>(_a[1]))); break;
        case 3: _t->emitFPS((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->emitCurrentFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->emitCurrentFrameStr((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->emitCurrentFrame((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 7: _t->emitVideoControllsStates((*reinterpret_cast< QVector<bool>(*)>(_a[1]))); break;
        case 8: _t->emitTrackingIsActiveState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->emitPlayerOperationDone(); break;
        case 10: _t->runPlayerOperation(); break;
        case 11: _t->receiveLoadVideoCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->receiveLoadPictures((*reinterpret_cast< std::vector<boost::filesystem::path>(*)>(_a[1]))); break;
        case 13: _t->receiveLoadCameraDevice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->receiveActivateTracking(); break;
        case 15: _t->receiveDeaktivateTracking(); break;
        case 16: _t->receivePrevFrameCommand(); break;
        case 17: _t->receiveNextFramCommand(); break;
        case 18: _t->receivePauseCommand(); break;
        case 19: _t->receiveStopCommand(); break;
        case 20: _t->receivePlayCommand(); break;
        case 21: _t->receiveStateDone(); break;
        case 22: _t->receiveTrackingDone(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
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
            typedef void (MediaPlayer::*_t)(GuiParam::MediaType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitMediaType)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)(size_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitTotalNumbFrames)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)(size_t );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitCurrentFrameNumber)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitFPS)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitCurrentFileName)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)(std::shared_ptr<cv::Mat> , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitCurrentFrameStr)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)(std::shared_ptr<cv::Mat> , uint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitCurrentFrame)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)(QVector<bool> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitVideoControllsStates)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitTrackingIsActiveState)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (MediaPlayer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MediaPlayer::emitPlayerOperationDone)) {
                *result = 9;
                return;
            }
        }
    }
}

const QMetaObject MediaPlayer::staticMetaObject = {
    { &IModel::staticMetaObject, qt_meta_stringdata_MediaPlayer.data,
      qt_meta_data_MediaPlayer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MediaPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MediaPlayer.stringdata0))
        return static_cast<void*>(const_cast< MediaPlayer*>(this));
    return IModel::qt_metacast(_clname);
}

int MediaPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void MediaPlayer::emitMediaType(GuiParam::MediaType _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MediaPlayer::emitTotalNumbFrames(size_t _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MediaPlayer::emitCurrentFrameNumber(size_t _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MediaPlayer::emitFPS(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MediaPlayer::emitCurrentFileName(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MediaPlayer::emitCurrentFrameStr(std::shared_ptr<cv::Mat> _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MediaPlayer::emitCurrentFrame(std::shared_ptr<cv::Mat> _t1, uint _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MediaPlayer::emitVideoControllsStates(QVector<bool> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MediaPlayer::emitTrackingIsActiveState(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MediaPlayer::emitPlayerOperationDone()
{
    QMetaObject::activate(this, &staticMetaObject, 9, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
