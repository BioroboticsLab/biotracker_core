/****************************************************************************
** Meta object code from reading C++ file 'BioTrackerPlugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BioTrackerPlugin/BioTrackerPlugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BioTrackerPlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BioTrackerPlugin_t {
    QByteArrayData data[11];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BioTrackerPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BioTrackerPlugin_t qt_meta_stringdata_BioTrackerPlugin = {
    {
QT_MOC_LITERAL(0, 0, 16), // "BioTrackerPlugin"
QT_MOC_LITERAL(1, 17, 9), // "emitCvMat"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 24), // "std::shared_ptr<cv::Mat>"
QT_MOC_LITERAL(4, 53, 3), // "mat"
QT_MOC_LITERAL(5, 57, 4), // "name"
QT_MOC_LITERAL(6, 62, 16), // "emitTrackingDone"
QT_MOC_LITERAL(7, 79, 12), // "receiveCvMat"
QT_MOC_LITERAL(8, 92, 11), // "frameNumber"
QT_MOC_LITERAL(9, 104, 26), // "receiveCvMatFromController"
QT_MOC_LITERAL(10, 131, 19) // "receiveTrackingDone"

    },
    "BioTrackerPlugin\0emitCvMat\0\0"
    "std::shared_ptr<cv::Mat>\0mat\0name\0"
    "emitTrackingDone\0receiveCvMat\0frameNumber\0"
    "receiveCvMatFromController\0"
    "receiveTrackingDone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BioTrackerPlugin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       6,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   45,    2, 0x0a /* Public */,
       9,    2,   50,    2, 0x08 /* Private */,
      10,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::UInt,    4,    8,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void,

       0        // eod
};

void BioTrackerPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BioTrackerPlugin *_t = static_cast<BioTrackerPlugin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitCvMat((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->emitTrackingDone(); break;
        case 2: _t->receiveCvMat((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 3: _t->receiveCvMatFromController((*reinterpret_cast< std::shared_ptr<cv::Mat>(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->receiveTrackingDone(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BioTrackerPlugin::*_t)(std::shared_ptr<cv::Mat> , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTrackerPlugin::emitCvMat)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (BioTrackerPlugin::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BioTrackerPlugin::emitTrackingDone)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject BioTrackerPlugin::staticMetaObject = {
    { &IBioTrackerPlugin::staticMetaObject, qt_meta_stringdata_BioTrackerPlugin.data,
      qt_meta_data_BioTrackerPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BioTrackerPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BioTrackerPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BioTrackerPlugin.stringdata0))
        return static_cast<void*>(const_cast< BioTrackerPlugin*>(this));
    if (!strcmp(_clname, "de.fu-berlin.mi.biorobotics.IBioTrackerPlugin"))
        return static_cast< IBioTrackerPlugin*>(const_cast< BioTrackerPlugin*>(this));
    return IBioTrackerPlugin::qt_metacast(_clname);
}

int BioTrackerPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IBioTrackerPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void BioTrackerPlugin::emitCvMat(std::shared_ptr<cv::Mat> _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BioTrackerPlugin::emitTrackingDone()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

QT_PLUGIN_METADATA_SECTION const uint qt_section_alignment_dummy = 42;

#ifdef QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    'q',  'b',  'j',  's',  0x01, 0x00, 0x00, 0x00,
    0x08, 0x01, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0xf4, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    ',',  0x00, 'd',  'e',  '.',  'f',  'u',  '-', 
    'b',  'e',  'r',  'l',  'i',  'n',  '.',  'm', 
    'i',  '.',  'b',  'i',  'o',  'r',  'o',  'b', 
    'o',  't',  'i',  'c',  's',  '.',  'B',  'i', 
    'o',  'T',  'r',  'a',  'c',  'k',  'e',  'r', 
    'P',  'l',  'u',  'g',  'i',  'n',  0x00, 0x00,
    0x1b, 0x0b, 0x00, 0x00, 0x09, 0x00, 'c',  'l', 
    'a',  's',  's',  'N',  'a',  'm',  'e',  0x00,
    0x10, 0x00, 'B',  'i',  'o',  'T',  'r',  'a', 
    'c',  'k',  'e',  'r',  'P',  'l',  'u',  'g', 
    'i',  'n',  0x00, 0x00, ':',  0xc0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
    0x05, 0x00, 'd',  'e',  'b',  'u',  'g',  0x00,
    0x15, 0x13, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    '\\', 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    'X',  0x00, 0x00, 0x00, 0x14, 0x03, 0x00, 0x00,
    0x04, 0x00, 'K',  'e',  'y',  's',  0x00, 0x00,
    '@',  0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    '<',  0x00, 0x00, 0x00, ',',  0x00, 'd',  'e', 
    '.',  'f',  'u',  '-',  'b',  'e',  'r',  'l', 
    'i',  'n',  '.',  'm',  'i',  '.',  'b',  'i', 
    'o',  'r',  'o',  'b',  'o',  't',  'i',  'c', 
    's',  '.',  'B',  'i',  'o',  'T',  'r',  'a', 
    'c',  'k',  'e',  'r',  'P',  'l',  'u',  'g', 
    'i',  'n',  0x00, 0x00, 0x8b, 0x01, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x88, 0x00, 0x00, 0x00, 'H',  0x00, 0x00, 0x00,
    '|',  0x00, 0x00, 0x00, 'l',  0x00, 0x00, 0x00
};

#else // QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    'q',  'b',  'j',  's',  0x01, 0x00, 0x00, 0x00,
    0x08, 0x01, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0xf4, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    ',',  0x00, 'd',  'e',  '.',  'f',  'u',  '-', 
    'b',  'e',  'r',  'l',  'i',  'n',  '.',  'm', 
    'i',  '.',  'b',  'i',  'o',  'r',  'o',  'b', 
    'o',  't',  'i',  'c',  's',  '.',  'B',  'i', 
    'o',  'T',  'r',  'a',  'c',  'k',  'e',  'r', 
    'P',  'l',  'u',  'g',  'i',  'n',  0x00, 0x00,
    0x15, 0x0b, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    '\\', 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    'X',  0x00, 0x00, 0x00, 0x14, 0x03, 0x00, 0x00,
    0x04, 0x00, 'K',  'e',  'y',  's',  0x00, 0x00,
    '@',  0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    '<',  0x00, 0x00, 0x00, ',',  0x00, 'd',  'e', 
    '.',  'f',  'u',  '-',  'b',  'e',  'r',  'l', 
    'i',  'n',  '.',  'm',  'i',  '.',  'b',  'i', 
    'o',  'r',  'o',  'b',  'o',  't',  'i',  'c', 
    's',  '.',  'B',  'i',  'o',  'T',  'r',  'a', 
    'c',  'k',  'e',  'r',  'P',  'l',  'u',  'g', 
    'i',  'n',  0x00, 0x00, 0x8b, 0x01, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x9b, 0x18, 0x00, 0x00,
    0x09, 0x00, 'c',  'l',  'a',  's',  's',  'N', 
    'a',  'm',  'e',  0x00, 0x10, 0x00, 'B',  'i', 
    'o',  'T',  'r',  'a',  'c',  'k',  'e',  'r', 
    'P',  'l',  'u',  'g',  'i',  'n',  0x00, 0x00,
    '1',  0x00, 0x00, 0x00, 0x05, 0x00, 'd',  'e', 
    'b',  'u',  'g',  0x00, ':',  0xc0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    'H',  0x00, 0x00, 0x00, 0xb4, 0x00, 0x00, 0x00,
    0xd8, 0x00, 0x00, 0x00, 0xe4, 0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(BioTrackerPlugin, BioTrackerPlugin)

QT_END_MOC_NAMESPACE
