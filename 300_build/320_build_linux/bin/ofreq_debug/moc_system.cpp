/****************************************************************************
** Meta object code from reading C++ file 'system.h'
**
** Created:
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../200_src/bin/ofreq/system_objects/system.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'system.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_osea__ofreq__System[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   21,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   20,   20,   20, 0x0a,
      90,   84,   20,   20, 0x0a,
     104,   20,   20,   20, 0x0a,
     114,   84,   20,   20, 0x0a,
     137,   20,   20,   20, 0x0a,
     149,   84,   20,   20, 0x0a,
     182,   20,   20,   20, 0x0a,
     204,   84,   20,   20, 0x0a,
     235,   20,   20,   20, 0x0a,
     256,   84,   20,   20, 0x0a,
     287,   20,   20,   20, 0x0a,
     314,  308,   20,   20, 0x0a,
     330,   20,   20,   20, 0x0a,
     348,  342,   20,   20, 0x0a,
     374,  364,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_osea__ofreq__System[] = {
    "osea::ofreq::System\0\0mySystem\0"
    "ReferenceSystem(System*)\0"
    "setAnalysisType(std::string)\0input\0"
    "addBody(Body)\0addBody()\0addOutput(OutputsBody)\0"
    "addOutput()\0addForceActive_user(ForceActive)\0"
    "addForceActive_user()\0"
    "addForceReact_user(ForceReact)\0"
    "addForceReact_user()\0"
    "addForceCross_user(ForceCross)\0"
    "addForceCross_user()\0bodID\0linkBodies(int)\0"
    "listModel()\0index\0listModel(uint)\0"
    "modelName\0listModel(std::string)\0"
};

void osea::ofreq::System::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
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
    }
}

const QMetaObjectExtraData osea::ofreq::System::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject osea::ofreq::System::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_osea__ofreq__System,
      qt_meta_data_osea__ofreq__System, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &osea::ofreq::System::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *osea::ofreq::System::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *osea::ofreq::System::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_osea__ofreq__System))
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
