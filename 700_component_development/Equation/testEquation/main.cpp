#include <QCoreApplication>
#include <QDebug>
#include <QtGlobal>
#include "eqntranslation.h"
#include <iostream>
#include "body.h"
#include "motionmodel.h"

using namespace std;
using namespace osea::ofreq;

void CreateBody(Body &BodA, int offset = 0);

std::complex<double> func1();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Create a motion model object
    MotionModel ModelA;

    //Create an equation of motion object.
    EqnTranslation TransA(&ModelA);

    //Set Equation debug properties.
    TransA.setDebugData(0.540, complex<double>(5,4), false);

    //Test Model Index Properties
    TransA.setDataIndex(1);
    cout << "Model Index = " << TransA.getDataIndex() << endl;
    cout << "Reference Index = " << TransA.refDataIndex() << endl << endl;

    //Test Name Properties
    TransA.setName("Equation 1");
    cout << "Name = " << TransA.refName() << endl << endl;

    //Test Description Properties
    TransA.setDescription("Test Equation");
    cout << "Description = " << TransA.refDescription() << endl << endl;

    //Test Data Retrieval Functions
    cout << "Testing Data Retrieval Functions" << endl;
    cout << "===================================" << endl;
    cout << "ForceMass = " << TransA.ForceMass(1) << endl;
    cout << "ForceActive_hydro = " << TransA.ForceActive_hydro() << endl;
    cout << "ForceActive_user = " << TransA.ForceActive_user() << endl;
    cout << "ForceReact_hydro = " << TransA.ForceReact_hydro(5,4) << endl;
    cout << "ForceReact_user = " << TransA.ForceReact_user(5,4) << endl;
    cout << "ForceCross_hydro = " << TransA.ForceCross_hydro(5,4,3) << endl;
    cout << "ForceCross_user = " << TransA.ForceCross_user(5,4,3) << endl << endl;

    //Test math functions
    cout << "Testing Math Functions" << endl;
    cout << "===============================" << endl;
    cout << "Testing Kronecker" << endl;
    cout << "Kronecker(5,4) = " << TransA.Kronecker(5,4) << endl;
    cout << "Kronecker(5,5) = " << TransA.Kronecker(5,5) << endl;
    cout << "Kronecker(5,4,anti) = " << TransA.Kronecker(5,4,true) << endl;
    cout << "Kronecker(5,5,anti) = " << TransA.Kronecker(5,5,true) << endl << endl;

    cout << "Testing Ddt" << endl;
    cout << "Ddt(0) = " << TransA.Ddt(0,0) << endl;
    cout << "Ddt(1) = " << TransA.Ddt(0,1) << endl;
    cout << "Ddt(2) = " << TransA.Ddt(0,2) << endl << endl;

    cout << "Testing Sum functions" << endl;
    cout << "Sum(0 to 2) = " << TransA.Sum(func1, 0, 2) << endl;
    cout << "Sum(0 to 5) = " << TransA.Sum(func1, 0, 5) << endl;
    cout << "SSum(0 to 2) = " << TransA.Sum(TransA.Sum(func1, 0, 2), 0, 2) << endl << endl;

    cout << "Testing Sum Values" << endl;
    cout << "Sum(0 to 2) = " << TransA.Sum(complex<double>(1,-1), 0, 2) << endl;
    cout << "Sum(0 to 5) = " << TransA.Sum(complex<double>(1,-1), 0, 5) << endl;
    cout << "SSum(0 to 2) = " << TransA.Sum(TransA.Sum(complex<double>(1,-1), 0, 2), 0, 2) << endl << endl;

    //Testing member sum functions
    TransA.Evaluate();

    cout << "Testing QDebug" << endl;
    qDebug() << "Test Debug Output";
    qWarning() << "Test Warning Message";
    qCritical() << "Test Critical Message";

    return a.exec();
}

void CreateBody(Body &BodA, int offset)
{
    //Create a body object

    for (int i = 0; i < 6; i++)
    {
        BodA.ForceMass.push_back(complex<double>(i+offset,0));
        BodA.listForceActive_hydro.push_back(complex<double>(i+offset,i));
        BodA.listForceActive_user.push_back(complex<double>(0,i+offset));
        BodA.listForceCross_hydro.push_back(complex<double>(i+1+offset,0));
        BodA.listForceCross_user.push_back(complex<double>(0,i+1+offset));
        BodA.listForceReact_hydro.push_back(complex<double>(i+2+offset,0));
        BodA.listForceReact_user.push_back(complex<double>(0,i+2+offset));
    }
}

std::complex<double> func1()
{
    return complex<double>(1,-1);
}
