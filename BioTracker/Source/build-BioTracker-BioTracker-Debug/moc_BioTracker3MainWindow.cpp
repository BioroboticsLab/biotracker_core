/****************************************************************************
** Meta object code from reading C++ file 'BioTracker3MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/View/BioTracker3MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BioTracker3MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BioTracker3MainWindow_t {
    QByteArrayData data[11];
    char stringdata0[278];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BioTracker3MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BioTracker3MainWindow_t qt_meta_stringdata_BioTracker3MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 21), // "BioTracker3MainWindow"
QT_MOC_LITERAL(1, 22, 29), // "on_actionOpen_Video_triggered"
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 31), // "on_actionLoad_Tracker_triggered"
QT_MOC_LITERAL(4, 85, 31), // "on_actionOpen_Picture_triggered"
QT_MOC_LITERAL(5, 117, 37), // "on_actionLoad_tracking_data_t..."
QT_MOC_LITERAL(6, 155, 37), // "on_actionSave_tracking_data_t..."
QT_MOC_LITERAL(7, 193, 23), // "on_actionQuit_triggered"
QT_MOC_LITERAL(8, 217, 30), // "on_actionOpen_Camera_triggered"
QT_MOC_LITERAL(9, 248, 27), // "receiveSelectedCameraDevice"
QT_MOC_LITERAL(10, 276, 1) // "i"

    },
    "BioTracker3MainWindow\0"
    "on_actionOpen_Video_triggered\0\0"
    "on_actionLoad_Tracker_triggered\0"
    "on_actionOpen_Picture_triggered\0"
    "on_actionLoad_tracking_data_triggered\0"
    "on_actionSave_tracking_data_triggered\0"
    "on_actionQuit_triggered\0"
    "on_actionOpen_Camera_triggered\0"
    "receiveSelectedCameraDevice\0i"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BioTracker3MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void BioTracker3MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BioTracker3MainWindow *_t = static_cast<BioTracker3MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionOpen_Video_triggered(); break;
        case 1: _t->on_actionLoad_Tracker_triggered(); break;
        case 2: _t->on_actionOpen_Picture_triggered(); break;
        case 3: _t->on_actionLoad_tracking_data_triggered(); break;
        case 4: _t->on_actionSave_tracking_data_triggered(); break;
        case 5: _t->on_actionQuit_triggered(); break;
        case 6: _t->on_actionOpen_Camera_triggered(); break;
        case 7: _t->receiveSelectedCameraDevice((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject BioTracker3MainWindow::staticMetaObject = {
    { &IViewMainWindow::staticMetaObject, qt_meta_stringdata_BioTracker3MainWindow.data,
      qt_meta_data_BioTracker3MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BioTracker3MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BioTracker3MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BioTracker3MainWindow.stringdata0))
        return static_cast<void*>(const_cast< BioTracker3MainWindow*>(this));
    return IViewMainWindow::qt_metacast(_clname);
}

int BioTracker3MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IViewMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
