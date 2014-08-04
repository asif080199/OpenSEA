/****************************************************************************
** Meta object code from reading C++ file 'dictionary.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../200_src/bin/ofreq/file_reader/dictionary.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dictionary.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_osea__Dictionary_t {
    QByteArrayData data[8];
    char stringdata[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_osea__Dictionary_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_osea__Dictionary_t qt_meta_stringdata_osea__Dictionary = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 9),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 11),
QT_MOC_LITERAL(4, 40, 5),
QT_MOC_LITERAL(5, 46, 9),
QT_MOC_LITERAL(6, 56, 14),
QT_MOC_LITERAL(7, 71, 7)
    },
    "osea::Dictionary\0setObject\0\0ObjectGroup\0"
    "input\0setSystem\0ofreq::System*\0ptInput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_osea__Dictionary[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       5,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void osea::Dictionary::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Dictionary *_t = static_cast<Dictionary *>(_o);
        switch (_id) {
        case 0: _t->setObject((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 1: _t->setSystem((*reinterpret_cast< ofreq::System*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject osea::Dictionary::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_osea__Dictionary.data,
      qt_meta_data_osea__Dictionary,  qt_static_metacall, 0, 0}
};


const QMetaObject *osea::Dictionary::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *osea::Dictionary::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_osea__Dictionary.stringdata))
        return static_cast<void*>(const_cast< Dictionary*>(this));
    if (!strcmp(_clname, "osea::ofreq::oFreqCore"))
        return static_cast< osea::ofreq::oFreqCore*>(const_cast< Dictionary*>(this));
    return QObject::qt_metacast(_clname);
}

int osea::Dictionary::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
