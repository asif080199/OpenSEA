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
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   21,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   20,   20,   20, 0x0a,
      96,   20,   84,   20, 0x0a,
     120,  114,   20,   20, 0x0a,
     134,   20,   20,   20, 0x0a,
     144,  114,   20,   20, 0x0a,
     177,   20,   20,   20, 0x0a,
     199,  114,   20,   20, 0x0a,
     230,   20,   20,   20, 0x0a,
     251,  114,   20,   20, 0x0a,
     282,   20,   20,   20, 0x0a,
     309,  303,   20,   20, 0x0a,
     325,   20,   20,   20, 0x0a,
     343,  337,   20,   20, 0x0a,
     369,  359,   20,   20, 0x0a,
     400,  392,   20,   20, 0x0a,
     428,  392,   20,   20, 0x0a,
     470,  392,   20,   20, 0x0a,
     514,  392,   20,   20, 0x0a,
     552,  392,   20,   20, 0x0a,
     588,   20,   20,   20, 0x0a,
     609,  602,   20,   20, 0x0a,
     641,  602,   20,   20, 0x0a,
     669,  602,   20,   20, 0x0a,
     706,  602,   20,   20, 0x0a,
     737,  602,   20,   20, 0x0a,
     763,   20,   20,   20, 0x0a,
     777,   20,   20,   20, 0x0a,
     800,   20,   20,   20, 0x0a,
     819,  337,   20,   20, 0x0a,
     851,  841,   20,   20, 0x0a,
     889,  881,   20,   20, 0x0a,
     926,   20,   20,   20, 0x0a,
     944,   20,   20,   20, 0x0a,
     963,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_osea__ofreq__System[] = {
    "osea::ofreq::System\0\0mySystem\0"
    "ReferenceSystem(System*)\0"
    "setAnalysisType(std::string)\0std::string\0"
    "getAnalysisType()\0input\0addBody(Body)\0"
    "addBody()\0addForceActive_user(ForceActive)\0"
    "addForceActive_user()\0"
    "addForceReact_user(ForceReact)\0"
    "addForceReact_user()\0"
    "addForceCross_user(ForceCross)\0"
    "addForceCross_user()\0bodID\0linkBodies(int)\0"
    "listModel()\0index\0listModel(uint)\0"
    "modelName\0listModel(std::string)\0"
    "modelIn\0addSeaModel(osea::SeaModel)\0"
    "addSeaModel(osea::SeaModel_DualDirection)\0"
    "addSeaModel(osea::SeaModel_SingleDirection)\0"
    "addSeaModel(osea::SeaModel_LongCrest)\0"
    "addSeaModel(osea::SeaModel_Uniform)\0"
    "addSeaModel()\0specIn\0"
    "addWaveSpec(osea::WaveSpecBase)\0"
    "addWaveSpec(osea::WaveSpec)\0"
    "addWaveSpec(osea::SpecBretschneider)\0"
    "addWaveSpec(osea::SpecJONSWAP)\0"
    "addWaveSpec(osea::SpecPM)\0addWaveSpec()\0"
    "SearchActiveSeaModel()\0listHydroManager()\0"
    "listHydroManager(int)\0bodNameIn\0"
    "listHydroManager(std::string)\0HydroIn\0"
    "addHydroManager(ofreq::HydroManager)\0"
    "addHydroManager()\0updateHydroForce()\0"
    "refReportManager()\0"
};

void osea::ofreq::System::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        System *_t = static_cast<System *>(_o);
        switch (_id) {
        case 0: _t->ReferenceSystem((*reinterpret_cast< System*(*)>(_a[1]))); break;
        case 1: _t->setAnalysisType((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 2: { std::string _r = _t->getAnalysisType();
            if (_a[0]) *reinterpret_cast< std::string*>(_a[0]) = _r; }  break;
        case 3: _t->addBody((*reinterpret_cast< Body(*)>(_a[1]))); break;
        case 4: _t->addBody(); break;
        case 5: _t->addForceActive_user((*reinterpret_cast< ForceActive(*)>(_a[1]))); break;
        case 6: _t->addForceActive_user(); break;
        case 7: _t->addForceReact_user((*reinterpret_cast< ForceReact(*)>(_a[1]))); break;
        case 8: _t->addForceReact_user(); break;
        case 9: _t->addForceCross_user((*reinterpret_cast< ForceCross(*)>(_a[1]))); break;
        case 10: _t->addForceCross_user(); break;
        case 11: _t->linkBodies((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->listModel(); break;
        case 13: _t->listModel((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 14: _t->listModel((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 15: _t->addSeaModel((*reinterpret_cast< osea::SeaModel(*)>(_a[1]))); break;
        case 16: _t->addSeaModel((*reinterpret_cast< osea::SeaModel_DualDirection(*)>(_a[1]))); break;
        case 17: _t->addSeaModel((*reinterpret_cast< osea::SeaModel_SingleDirection(*)>(_a[1]))); break;
        case 18: _t->addSeaModel((*reinterpret_cast< osea::SeaModel_LongCrest(*)>(_a[1]))); break;
        case 19: _t->addSeaModel((*reinterpret_cast< osea::SeaModel_Uniform(*)>(_a[1]))); break;
        case 20: _t->addSeaModel(); break;
        case 21: _t->addWaveSpec((*reinterpret_cast< osea::WaveSpecBase(*)>(_a[1]))); break;
        case 22: _t->addWaveSpec((*reinterpret_cast< osea::WaveSpec(*)>(_a[1]))); break;
        case 23: _t->addWaveSpec((*reinterpret_cast< osea::SpecBretschneider(*)>(_a[1]))); break;
        case 24: _t->addWaveSpec((*reinterpret_cast< osea::SpecJONSWAP(*)>(_a[1]))); break;
        case 25: _t->addWaveSpec((*reinterpret_cast< osea::SpecPM(*)>(_a[1]))); break;
        case 26: _t->addWaveSpec(); break;
        case 27: _t->SearchActiveSeaModel(); break;
        case 28: _t->listHydroManager(); break;
        case 29: _t->listHydroManager((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->listHydroManager((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 31: _t->addHydroManager((*reinterpret_cast< ofreq::HydroManager(*)>(_a[1]))); break;
        case 32: _t->addHydroManager(); break;
        case 33: _t->updateHydroForce(); break;
        case 34: _t->refReportManager(); break;
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
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
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
