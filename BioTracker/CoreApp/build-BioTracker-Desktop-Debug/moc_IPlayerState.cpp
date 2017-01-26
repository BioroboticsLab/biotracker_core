/****************************************************************************
** Meta object code from reading C++ file 'IPlayerState.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/IStates/IPlayerState.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IPlayerState.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IPlayerState_t {
    QByteArrayData data[11];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IPlayerState_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IPlayerState_t qt_meta_stringdata_IPlayerState = {
    {
QT_MOC_LITERAL(0, 0, 12), // "IPlayerState"
QT_MOC_LITERAL(1, 13, 13), // "emitStateDone"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 13), // "PLAYER_STATES"
QT_MOC_LITERAL(4, 42, 13), // "STATE_INITIAL"
QT_MOC_LITERAL(5, 56, 20), // "STATE_INITIAL_STREAM"
QT_MOC_LITERAL(6, 77, 10), // "STATE_PLAY"
QT_MOC_LITERAL(7, 88, 15), // "STATE_STEP_FORW"
QT_MOC_LITERAL(8, 104, 15), // "STATE_STEP_BACK"
QT_MOC_LITERAL(9, 120, 11), // "STATE_PAUSE"
QT_MOC_LITERAL(10, 132, 10) // "STATE_WAIT"

    },
    "IPlayerState\0emitStateDone\0\0PLAYER_STATES\0"
    "STATE_INITIAL\0STATE_INITIAL_STREAM\0"
    "STATE_PLAY\0STATE_STEP_FORW\0STATE_STEP_BACK\0"
    "STATE_PAUSE\0STATE_WAIT"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IPlayerState[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       1,   20, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // enums: name, flags, count, data
       3, 0x0,    7,   24,

 // enum data: key, value
       4, uint(IPlayerState::STATE_INITIAL),
       5, uint(IPlayerState::STATE_INITIAL_STREAM),
       6, uint(IPlayerState::STATE_PLAY),
       7, uint(IPlayerState::STATE_STEP_FORW),
       8, uint(IPlayerState::STATE_STEP_BACK),
       9, uint(IPlayerState::STATE_PAUSE),
      10, uint(IPlayerState::STATE_WAIT),

       0        // eod
};

void IPlayerState::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IPlayerState *_t = static_cast<IPlayerState *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitStateDone(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IPlayerState::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IPlayerState::emitStateDone)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject IPlayerState::staticMetaObject = {
    { &IModel::staticMetaObject, qt_meta_stringdata_IPlayerState.data,
      qt_meta_data_IPlayerState,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IPlayerState::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IPlayerState::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IPlayerState.stringdata0))
        return static_cast<void*>(const_cast< IPlayerState*>(this));
    return IModel::qt_metacast(_clname);
}

int IPlayerState::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void IPlayerState::emitStateDone()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
