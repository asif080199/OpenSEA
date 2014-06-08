/****************************************************************************
** Meta object code from reading C++ file 'filereader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../200_src/bin/ofreq/file_reader/filereader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filereader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_osea__FileReader_t {
    QByteArrayData data[15];
    char stringdata[186];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_osea__FileReader_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_osea__FileReader_t qt_meta_stringdata_osea__FileReader = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 17),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 11),
QT_MOC_LITERAL(4, 48, 6),
QT_MOC_LITERAL(5, 55, 16),
QT_MOC_LITERAL(6, 72, 14),
QT_MOC_LITERAL(7, 87, 16),
QT_MOC_LITERAL(8, 104, 16),
QT_MOC_LITERAL(9, 121, 13),
QT_MOC_LITERAL(10, 135, 11),
QT_MOC_LITERAL(11, 147, 4),
QT_MOC_LITERAL(12, 152, 9),
QT_MOC_LITERAL(13, 162, 14),
QT_MOC_LITERAL(14, 177, 7)
    },
    "osea::FileReader\0outputControlFile\0\0"
    "ObjectGroup\0output\0outputBodiesFile\0"
    "outputDataFile\0outputForcesFile\0"
    "outputSeaEnvFile\0readHydroFile\0"
    "std::string\0path\0setSystem\0ofreq::System*\0"
    "ptInput\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_osea__FileReader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x05,
       5,    1,   52,    2, 0x05,
       6,    1,   55,    2, 0x05,
       7,    1,   58,    2, 0x05,
       8,    1,   61,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       9,    1,   64,    2, 0x0a,
      12,    1,   67,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Int, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void osea::FileReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FileReader *_t = static_cast<FileReader *>(_o);
        switch (_id) {
        case 0: _t->outputControlFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 1: _t->outputBodiesFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 2: _t->outputDataFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 3: _t->outputForcesFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 4: _t->outputSeaEnvFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 5: { int _r = _t->readHydroFile((*reinterpret_cast< std::string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: _t->setSystem((*reinterpret_cast< ofreq::System*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FileReader::*_t)(ObjectGroup );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileReader::outputControlFile)) {
                *result = 0;
            }
        }
        {
            typedef void (FileReader::*_t)(ObjectGroup );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileReader::outputBodiesFile)) {
                *result = 1;
            }
        }
        {
            typedef void (FileReader::*_t)(ObjectGroup );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileReader::outputDataFile)) {
                *result = 2;
            }
        }
        {
            typedef void (FileReader::*_t)(ObjectGroup );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileReader::outputForcesFile)) {
                *result = 3;
            }
        }
        {
            typedef void (FileReader::*_t)(ObjectGroup );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FileReader::outputSeaEnvFile)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject osea::FileReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_osea__FileReader.data,
      qt_meta_data_osea__FileReader,  qt_static_metacall, 0, 0}
};


const QMetaObject *osea::FileReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *osea::FileReader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_osea__FileReader.stringdata))
        return static_cast<void*>(const_cast< FileReader*>(this));
    if (!strcmp(_clname, "osea::ofreq::oFreqCore"))
        return static_cast< osea::ofreq::oFreqCore*>(const_cast< FileReader*>(this));
    return QObject::qt_metacast(_clname);
}

int osea::FileReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void osea::FileReader::outputControlFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void osea::FileReader::outputBodiesFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void osea::FileReader::outputDataFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void osea::FileReader::outputForcesFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void osea::FileReader::outputSeaEnvFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
