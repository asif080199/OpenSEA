/****************************************************************************
** Meta object code from reading C++ file 'system.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../200_src/bin/ofreq/system_objects/system.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'system.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_osea__ofreq__System_t {
    QByteArrayData data[23];
    char stringdata[259];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_osea__ofreq__System_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_osea__ofreq__System_t qt_meta_stringdata_osea__ofreq__System = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 15),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 7),
QT_MOC_LITERAL(4, 45, 8),
QT_MOC_LITERAL(5, 54, 15),
QT_MOC_LITERAL(6, 70, 11),
QT_MOC_LITERAL(7, 82, 7),
QT_MOC_LITERAL(8, 90, 4),
QT_MOC_LITERAL(9, 95, 5),
QT_MOC_LITERAL(10, 101, 9),
QT_MOC_LITERAL(11, 111, 11),
QT_MOC_LITERAL(12, 123, 19),
QT_MOC_LITERAL(13, 143, 11),
QT_MOC_LITERAL(14, 155, 18),
QT_MOC_LITERAL(15, 174, 10),
QT_MOC_LITERAL(16, 185, 18),
QT_MOC_LITERAL(17, 204, 10),
QT_MOC_LITERAL(18, 215, 10),
QT_MOC_LITERAL(19, 226, 5),
QT_MOC_LITERAL(20, 232, 9),
QT_MOC_LITERAL(21, 242, 5),
QT_MOC_LITERAL(22, 248, 9)
    },
    "osea::ofreq::System\0ReferenceSystem\0"
    "\0System*\0mySystem\0setAnalysisType\0"
    "std::string\0addBody\0Body\0input\0addOutput\0"
    "OutputsBody\0addForceActive_user\0"
    "ForceActive\0addForceReact_user\0"
    "ForceReact\0addForceCross_user\0ForceCross\0"
    "linkBodies\0bodID\0listModel\0index\0"
    "modelName\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_osea__ofreq__System[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       5,    1,   97,    2, 0x0a,
       7,    1,  100,    2, 0x0a,
       7,    0,  103,    2, 0x0a,
      10,    1,  104,    2, 0x0a,
      10,    0,  107,    2, 0x0a,
      12,    1,  108,    2, 0x0a,
      12,    0,  111,    2, 0x0a,
      14,    1,  112,    2, 0x0a,
      14,    0,  115,    2, 0x0a,
      16,    1,  116,    2, 0x0a,
      16,    0,  119,    2, 0x0a,
      18,    1,  120,    2, 0x0a,
      20,    0,  123,    2, 0x0a,
      20,    1,  124,    2, 0x0a,
      20,    1,  127,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,   21,
    QMetaType::Void, 0x80000000 | 6,   22,

       0        // eod
};

void osea::ofreq::System::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        System *_t = static_cast<System *>(_o);
        switch (_id) {
        case 0: _t->ReferenceSystem((*reinterpret_cast< System*(*)>(_a[1]))); break;
        case 1: _t->setAnalysisType((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 2: _t->addBody((*reinterpret_cast< Body(*)>(_a[1]))); break;
        case 3: _t->addBody(); break;
        case 4: _t->addOutput((*reinterpret_cast< OutputsBody(*)>(_a[1]))); break;
        case 5: _t->addOutput(); break;
        case 6: _t->addForceActive_user((*reinterpret_cast< ForceActive(*)>(_a[1]))); break;
        case 7: _t->addForceActive_user(); break;
        case 8: _t->addForceReact_user((*reinterpret_cast< ForceReact(*)>(_a[1]))); break;
        case 9: _t->addForceReact_user(); break;
        case 10: _t->addForceCross_user((*reinterpret_cast< ForceCross(*)>(_a[1]))); break;
        case 11: _t->addForceCross_user(); break;
        case 12: _t->linkBodies((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->listModel(); break;
        case 14: _t->listModel((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 15: _t->listModel((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< System* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (System::*_t)(System * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&System::ReferenceSystem)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject osea::ofreq::System::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_osea__ofreq__System.data,
      qt_meta_data_osea__ofreq__System,  qt_static_metacall, 0, 0}
};


const QMetaObject *osea::ofreq::System::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *osea::ofreq::System::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_osea__ofreq__System.stringdata))
        return static_cast<void*>(const_cast< System*>(this));
    if (!strcmp(_clname, "osea::ofreq::oFreqCore"))
        return static_cast< osea::ofreq::oFreqCore*>(const_cast< System*>(this));
    return QObject::qt_metacast(_clname);
}

int osea::ofreq::System::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void osea::ofreq::System::ReferenceSystem(System * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
