/****************************************************************************
** Meta object code from reading C++ file 'VideoControllWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTracker/View/VideoControllWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoControllWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_VideoControllWidget_t {
    QByteArrayData data[13];
    char stringdata0[264];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoControllWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoControllWidget_t qt_meta_stringdata_VideoControllWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "VideoControllWidget"
QT_MOC_LITERAL(1, 20, 11), // "getNotified"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 24), // "on_sld_video_sliderMoved"
QT_MOC_LITERAL(4, 58, 8), // "position"
QT_MOC_LITERAL(5, 67, 18), // "on_DurationChanged"
QT_MOC_LITERAL(6, 86, 18), // "on_PositionChanged"
QT_MOC_LITERAL(7, 105, 27), // "on_button_nextFrame_clicked"
QT_MOC_LITERAL(8, 133, 27), // "on_button_playPause_clicked"
QT_MOC_LITERAL(9, 161, 22), // "on_button_stop_clicked"
QT_MOC_LITERAL(10, 184, 31), // "on_button_previousFrame_clicked"
QT_MOC_LITERAL(11, 216, 42), // "on_comboBoxSelectedView_curre..."
QT_MOC_LITERAL(12, 259, 4) // "arg1"

    },
    "VideoControllWidget\0getNotified\0\0"
    "on_sld_video_sliderMoved\0position\0"
    "on_DurationChanged\0on_PositionChanged\0"
    "on_button_nextFrame_clicked\0"
    "on_button_playPause_clicked\0"
    "on_button_stop_clicked\0"
    "on_button_previousFrame_clicked\0"
    "on_comboBoxSelectedView_currentTextChanged\0"
    "arg1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoControllWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    1,   60,    2, 0x08 /* Private */,
       5,    1,   63,    2, 0x08 /* Private */,
       6,    1,   66,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    1,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

void VideoControllWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoControllWidget *_t = static_cast<VideoControllWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->getNotified(); break;
        case 1: _t->on_sld_video_sliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_DurationChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_PositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_button_nextFrame_clicked(); break;
        case 5: _t->on_button_playPause_clicked(); break;
        case 6: _t->on_button_stop_clicked(); break;
        case 7: _t->on_button_previousFrame_clicked(); break;
        case 8: _t->on_comboBoxSelectedView_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject VideoControllWidget::staticMetaObject = {
    { &IViewWidget::staticMetaObject, qt_meta_stringdata_VideoControllWidget.data,
      qt_meta_data_VideoControllWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *VideoControllWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoControllWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_VideoControllWidget.stringdata0))
        return static_cast<void*>(const_cast< VideoControllWidget*>(this));
    return IViewWidget::qt_metacast(_clname);
}

int VideoControllWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IViewWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
