/****************************************************************************
** Meta object code from reading C++ file 'filereader.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../200_src/bin/ofreq/file_reader/filereader.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filereader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileReader[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   12,   11,   11, 0x05,
      50,   12,   11,   11, 0x05,
      80,   12,   11,   11, 0x05,
     108,   12,   11,   11, 0x05,
     138,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     177,  172,  168,   11, 0x0a,
     207,  199,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FileReader[] = {
    "FileReader\0\0output\0outputControlFile(ObjectGroup)\0"
    "outputBodiesFile(ObjectGroup)\0"
    "outputDataFile(ObjectGroup)\0"
    "outputForcesFile(ObjectGroup)\0"
    "outputSeaEnvFile(ObjectGroup)\0int\0"
    "path\0readHydroFile(string)\0ptInput\0"
    "setSystem(System*)\0"
};

void FileReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileReader *_t = static_cast<FileReader *>(_o);
        switch (_id) {
        case 0: _t->outputControlFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 1: _t->outputBodiesFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 2: _t->outputDataFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 3: _t->outputForcesFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 4: _t->outputSeaEnvFile((*reinterpret_cast< ObjectGroup(*)>(_a[1]))); break;
        case 5: { int _r = _t->readHydroFile((*reinterpret_cast< string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: _t->setSystem((*reinterpret_cast< System*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileReader::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FileReader,
      qt_meta_data_FileReader, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileReader::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileReader::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileReader))
        return static_cast<void*>(const_cast< FileReader*>(this));
    return QObject::qt_metacast(_clname);
}

int FileReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void FileReader::outputControlFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileReader::outputBodiesFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FileReader::outputDataFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileReader::outputForcesFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FileReader::outputSeaEnvFile(ObjectGroup _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
