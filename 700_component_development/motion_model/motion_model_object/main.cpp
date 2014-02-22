#include <QCoreApplication>
#include "./global_objects/body.h"
#include "./motion_model/motionmodel.h"
//#include "./global_objects/forceactive.h"
//#include "./global_objects/forcecross.h"
//#include "./global_objects/forcereact.h"
//#include "./global_objects/equation.h"
//#include "./global_objects/derivative.h"
#include <vector>

using namespace osea::ofreq;
using namespace std;

void CreateForces();    //Creates force objects to add to the body object
ForceActive Active1;
ForceReact React1;
ForceCross Cross1;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //Variable Declaration
    //==================================================================
    Body BodA;
    Body BodB;
    MotionModel ModelA;

    //Main Program
    //==================================================================
    //Create force objects and add them to the bodies
    BodA.listForceActive_hydro().push_back(&Active1);
    BodA.listForceActive_user().push_back(&Active1);
    BodA.listForceReact_hydro().push_back(&React1);
    BodA.listForceReact_user().push_back(&React1);
    BodA.listForceCross_hydro().push_back(&Cross1);
    BodA.listForceCross_hydro().push_back(&Cross1);
    BodA.listForceCross_user().push_back(&Cross1);
    BodA.listForceCross_user().push_back(&Cross1);
    BodB = BodA.Copy();

    //Now that I have the body defined, add it to the motion model and see what we get.
    ModelA.listBody().push_back(BodA);
    ModelA.listBody().push_back(BodB);
    ModelA.setBody(0);      //Set which body to use

    //Set forces and add some in.
    ModelA.Reset();
    ModelA.useForceActive_hydro(0,0);
    ModelA.useForceActive_user(0,0);
    ModelA.useForceReact_hydro(0,1,0,0);
    ModelA.useForceReact_user(0,1,0,0);




    
    return a.exec();
}

void CreateForces()
{
    //Creates force objects that can be referenced by the body object.
    //You can have multiple references to the same force object.

    for (int i = 0; i < 6; i++)
    {
        Active1.listCoefficients().push_back(complex<double>(i,-i));
    }

    //Create some equations with variables to add into derivative object
    vector<Equation> listEqn;
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
            temp->setCoefficient(j,i*j);
        }

        //Set second derivative
        temp = &(Deriv2.listEquation().at(i));
        temp->setDataIndex(i);
        for (int j = 0; j < 6; j++)
        {
            temp->setCoefficient(2*i,j*i);
        }
    }

    //add the two derivatives into the react force object
    React1.addDerivative(Deriv1, 0);
    React1.addDerivative(Deriv2, 1);

    //Add the two derivatives into the cross force object
    React1.addDerivative(Deriv1, 1);
    React1.addDerivative(Deriv2, 2);

}
