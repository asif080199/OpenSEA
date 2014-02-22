//#include <QCoreApplication>
#include <QDebug>
#include <QtGlobal>
#include "motion_model/eqntranslation.h"
#include <iostream>
#include "global_objects/body.h"
#include "motion_model/motionmodel.h"

using namespace osea::ofreq;
using namespace std;

void CreateForces();    //Creates force objects to add to the body object
ForceActive Active1;
ForceReact React1;
ForceCross Cross1;
void CreateBody(Body &BodA);

std::complex<double> func1();

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    //Create a body object and populate with force objects
    Body BodA;
    Body BodB;
    CreateBody(BodA);
    BodB = BodA.Copy();

    //Create a motion model object
    MotionModel ModelA;

    vector<Body> listBod;
    listBod.push_back(BodA);
    listBod.push_back(BodB);
    BodA = listBod.at(0);
    BodB = listBod.at(1);

    ModelA.setlistBody(listBod);
    ModelA.setBody(0);

    //Create an equation of motion object.
    EqnTranslation TransA(&ModelA);

    //Set Equation debug properties.
    TransA.setDebugData(0.540, complex<double>(5,4), false);

    //Testing Equation Object Properties
    //====================================================

    //Test Model Index Properties
    TransA.setDataIndex(0);
    cout << "Model Index = " << TransA.getDataIndex() << endl;
    cout << "Reference Index = " << TransA.refDataIndex() << endl << endl;

    //Test Name Properties
    TransA.setName("Equation 1");
    cout << "Name = " << TransA.refName() << endl << endl;

    //Test Description Properties
    TransA.setDescription("Test Equation");
    cout << "Description = " << TransA.refDescription() << endl << endl;

    //Test Data Application Functions
    cout << "Testing Data Application Functions" << endl;
    cout << "===============================" << endl;
    cout << "Testing ForceMass" << endl;
    ModelA.useForceMass(0,0);
    cout << "Mass(0,0) = " << ModelA.listData().at(0).MassMatrix()(0,0) << endl;
    cout << "Testing Full Matrix Force Mass" << endl;
    ModelA.useForceMass(2);
    cout << "Mass(0,0) = " << ModelA.listData().at(0).MassMatrix()(0,0) << endl;
    cout << "Mass(2,2) = " << ModelA.listData().at(0).MassMatrix()(2,2) << endl;
    ModelA.useForceMass();
    cout << "Mass(0,0) = " << ModelA.listData().at(0).MassMatrix()(0,0) << endl;
    cout << "Mass(2,2) = " << ModelA.listData().at(0).MassMatrix()(2,2) << endl << endl;

    cout << "Testing Force Active User" << endl;
    ModelA.useForceActive_user(0,1);
    cout << "ForceActive(0) = " << ModelA.listData().at(0).listForceActive_user().at(0)->listCoefficients(1) << endl;
    ModelA.useForceActive_user(1);
    cout << "ForceActive(1) = " << ModelA.listData().at(0).listForceActive_user().at(1)->listCoefficients(2) << endl;
    ModelA.useForceActive_user();
    cout << "ForceActive(0) = " << ModelA.listData().at(0).listForceActive_user().at(0)->listCoefficients(1) << endl;
    cout << "ForceActive(1) = " << ModelA.listData().at(0).listForceActive_user().at(1)->listCoefficients(2) << endl << endl;

    cout << "Testing Force Active hydro" << endl;
    ModelA.useForceActive_hydro(0,1);
    cout << "ForceActive(0) = " << ModelA.listData().at(0).listForceActive_hydro().at(0)->listCoefficients(1) << endl;
    ModelA.useForceActive_hydro(1);
    cout << "ForceActive(1) = " << ModelA.listData().at(0).listForceActive_hydro().at(1)->listCoefficients(2) << endl;
    ModelA.useForceActive_hydro();
    cout << "ForceActive(0) = " << ModelA.listData().at(0).listForceActive_hydro().at(0)->listCoefficients(1) << endl;
    cout << "ForceActive(1) = " << ModelA.listData().at(0).listForceActive_hydro().at(1)->listCoefficients(2) << endl << endl;

    cout << "Testing Reactive Force User" << endl;
    ModelA.useForceReact_user(0,1,1,1);
    cout << "ForceReact(0,1,1,1,1) = " << ModelA.listData().at(0).listForceReact_user().at(0)->
            listDerivative(1).
            listEquation(1).
            listCoefficients(1) << endl;
    ModelA.useForceReact_user(0,1,2);
    cout << "ForceReact(0,1,2) = " << ModelA.listData().at(0).listForceReact_user().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
    ModelA.useForceReact_user(0,1);
    cout << "ForceReact(0,1,2) = " << ModelA.listData().at(0).listForceReact_user().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
//    cout << "ForceReact(0,1,3) = " << ModelA.listData().at(0).listForceReact_user().at(0)->
//            listDerivative(1).
//            listEquation(3).
//            listCoefficients(1) << endl;
    ModelA.useForceReact_user(0);
    cout << "ForceReact(0,1,2) = " << ModelA.listData().at(0).listForceReact_user().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
    ModelA.useForceReact_user();
    cout << "ForceReact(0,1,2) = " << ModelA.listData().at(0).listForceReact_user().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
    cout << "ForceReact(1,1,2) = " << ModelA.listData().at(0).listForceReact_user().at(1)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl << endl;

    cout << "Testing Reactive Force hydro" << endl;
    ModelA.useForceReact_hydro(0,1,1,1);
    cout << "ForceReact(0,1,1,1,1) = " << ModelA.listData().at(0).listForceReact_hydro().at(0)->
            listDerivative(1).
            listEquation(1).
            listCoefficients(1) << endl;
    ModelA.useForceReact_hydro(0,1,2);
    cout << "ForceReact(0,1,2) = " << ModelA.listData().at(0).listForceReact_hydro().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
    ModelA.useForceReact_hydro(0,1);
    cout << "ForceReact(0,1,2) = " << ModelA.listData().at(0).listForceReact_hydro().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
    cout << "ForceReact(0,2,2) = " << ModelA.listData().at(0).listForceReact_hydro().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
    ModelA.useForceReact_hydro(0);
    cout << "ForceReact(0,1,2) = " << ModelA.listData().at(0).listForceReact_hydro().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
    ModelA.useForceReact_hydro();
    cout << "ForceReact(0,1,2) = " << ModelA.listData().at(0).listForceReact_hydro().at(0)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl;
    cout << "ForceReact(1,1,2) = " << ModelA.listData().at(0).listForceReact_hydro().at(1)->
            listDerivative(1).
            listEquation(2).
            listCoefficients(1) << endl << endl;

    cout << endl;

    //Test Data Retrieval Functions
    //First apply all the forces
    ModelA.Reset();
    ModelA.useForceActive_hydro();
    ModelA.useForceActive_user();
    ModelA.useForceReact_hydro();
    ModelA.useForceReact_user();
//    ModelA.useForceCross_hydro();
//    ModelA.useForceCross_user();
    ModelA.useForceMass();

    //Testing
    cout << "Testing Data Retrieval Functions" << endl;
    cout << "===================================" << endl;
    cout << "ForceMass = " << TransA.ForceMass(0) << endl;
    cout << "ForceActive_hydro = " << TransA.ForceActive_hydro() << endl;
    cout << "ForceActive_user = " << TransA.ForceActive_user() << endl;
    cout << "ForceReact_hydro = " << TransA.ForceReact_hydro(1,1) << endl;
    cout << "ForceReact_user = " << TransA.ForceReact_user(1,1) << endl;
    cout << "ForceCross_hydro = " << TransA.ForceCross_hydro(0,1,1) << endl;
    cout << "ForceCross_user = " << TransA.ForceCross_user(0,1,1) << endl << endl;

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

//    return a.exec();
    return 0;
}

void CreateBody(Body &BodA)
{
    //Create forces
    CreateForces();

    //Populate Body object with forces
    BodA.setMass(50);
    BodA.listForceActive_hydro().push_back(&Active1);
    BodA.listForceActive_hydro().push_back(&Active1);

    BodA.listForceActive_user().push_back(&Active1);
    BodA.listForceActive_user().push_back(&Active1);

    BodA.listForceReact_hydro().push_back(&React1);
    BodA.listForceReact_hydro().push_back(&React1);

    BodA.listForceReact_user().push_back(&React1);
    BodA.listForceReact_user().push_back(&React1);

    BodA.listForceCross_hydro().push_back(&Cross1);
    BodA.listForceCross_hydro().push_back(&Cross1);

    BodA.listForceCross_user().push_back(&Cross1);
    BodA.listForceCross_user().push_back(&Cross1);

}

std::complex<double> func1()
{
    return complex<double>(1,-1);
}

void CreateForces()
{
    //Creates force objects that can be referenced by the body object.
    //You can have multiple references to the same force object.

    for (int i = 0; i < 6; i++)
    {
        Active1.listCoefficients().push_back(complex<double>(i+1,-i-1));
    }

    //Create some equations with variables to add into derivative object
    Derivative Deriv1;
    Derivative Deriv2;

    Equation* temp;

    for (int i = 0; i < 6; i++)
    {
        Deriv1.listEquation().push_back(Equation());
        Deriv2.listEquation().push_back(Equation());

        temp = &(Deriv1.listEquation().at(i));
        temp->setDataIndex(i);

        //Set first derivative
        for (int j = 0; j < 6; j++)
        {
            temp->setCoefficient(j+1,j+1);
        }

        //Set second derivative
        temp = &(Deriv2.listEquation().at(i));
        temp->setDataIndex(i);
        for (int j = 0; j < 6; j++)
        {
            temp->setCoefficient(2*i+1,j*i+1);
        }
    }

    //add the two derivatives into the react force object
    React1.addDerivative(Deriv1, 0);
    React1.addDerivative(Deriv2, 1);

    //Add the two derivatives into the cross force object
    Cross1.addDerivative(Deriv1, 0);
    Cross1.addDerivative(Deriv2, 1);

}
