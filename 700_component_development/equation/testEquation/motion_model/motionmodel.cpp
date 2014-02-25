/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

//License
/*-------------------------------------------------------------------------------------------------------------------*\
 *Copyright Datawave Marine Solutions, 2013.
 *This file is part of OpenSEA.

 *OpenSEA is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.

 *OpenSEA is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.

 *You should have received a copy of the GNU General Public License
 *along with OpenSEA.  If not, see <http://www.gnu.org/licenses/>.
\*-------------------------------------------------------------------------------------------------------------------*/

#include "motionmodel.h"

using namespace std;
using namespace arma;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel::MotionModel()
{
    //Default constructor.
    ConstructorCommon();
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel::MotionModel(vector<Body> &listBodIn)
{
    ConstructorCommon();

    //Constructor that sets the body reference.
    setlistBody(listBodIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel::~MotionModel()
{
    //Default destructor.

    //Memory cleanup of equations
    for (unsigned int i = 0; i < plistEquations.size(); i++)
    {
        delete plistEquations[i];
    }
    plistEquations.clear();

    //Memory cleanup of forces used in the data set
    this->Reset();
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::setlistBody(vector<Body> &listBodIn)
{
    //Set the list of bodies to use in the motion model.
    plistBody = &listBodIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::setBody(int bod)
{
    //Set the integer for the body to use as the reference.
    curBody = bod;

    //Erase the cross body objects.  No longer valid.
    pCompCrossBod_hydro.clear();
    pCompCrossBod_user.clear();
}

//------------------------------------------Function Separator --------------------------------------------------------
int MotionModel::getBody()
{
    if (curBody < 0)
        return -1;
    else
        return curBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool &MotionModel::CoefficientOnly()
{
    //Determine if the function should calculate coefficients or not.
    return pcalcCoeff;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool MotionModel::getActiveOnly()
{
    return pActiveOnly;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<int> &MotionModel::listCompCrossBod_hydro()
{
    return pCompCrossBod_hydro;
}

//------------------------------------------Function Separator --------------------------------------------------------
int &MotionModel::listCompCrossBod_hydro(int crossbodIn)
{
    return pCompCrossBod_hydro[crossbodIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<int> &MotionModel::listCompCrossBod_user()
{
    return pCompCrossBod_user;
}

//------------------------------------------Function Separator --------------------------------------------------------
int &MotionModel::listCompCrossBod_user(int crossbodIn)
{
    return pCompCrossBod_user[crossbodIn];
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::Reset()
{
    //Resets the motion model.
    /*Simply delete all variables.  This has an important caveat.  All variables are deleted to save time and memory.
     *This is better than the alternative of initializing all the variables as zeros.  However, this means that
     *any function which accesses the list data must be prepared to handle the exception of accessing non-existent
     *memory.  The default value to return when no memory is assigned is zero.
     */

    //memory cleanup of all variables in the data list

    for (unsigned int i = 0; i < plistData.size(); i++)
    {
        //memory cleanup of Active-user forces
        for (unsigned int j = 0; j < plistData[i].listForceActive_user().size(); j++)
        {
            delete plistData[i].listForceActive_user(j);
        }

        //memory cleanup of Active-hydro forces
        for (unsigned int j = 0; j < plistData[i].listForceActive_hydro().size(); j++)
        {
            delete plistData[i].listForceActive_hydro(j);
        }

        //memory cleanup of React-user forces
        for (unsigned int j = 0; j < plistData[i].listForceReact_user().size(); j++)
        {
            delete plistData[i].listForceReact_user(j);
        }

        //memory cleanup of React-hydro forces
        for (unsigned int j = 0; j < plistData[i].listForceReact_hydro().size(); j++)
        {
            delete plistData[i].listForceReact_hydro(j);
        }

        //memory cleanup of Cross-body user forces
        for (unsigned int j = 0; j < plistData[i].listForceCross_user().size(); j++)
        {
            delete plistData[i].listForceCross_user(j);
        }

        //memory cleanup of Cross-body hydro forces
        for (unsigned int j = 0; j < plistData[i].listForceCross_hydro().size(); j++)
        {
            delete plistData[i].listForceCross_hydro(j);
        }
    }

    //Delete all variables.
    plistData.clear();

    //Initialize Active force variable.
    pActiveOnly = true;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::setFreq(double freq)
{
    //Sets the wave frequency.
    pFreq = freq;
}

//------------------------------------------Function Separator --------------------------------------------------------
double MotionModel::getFreq()
{
    //Gets the wave frequency
    return pFreq;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceActive_user(unsigned int force, unsigned int eqn)
{
    //Trigger to use active forces.
    pActiveOnly = pActiveOnly * true;       //Will still evaluate to false if a reactive force was already triggered.

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceActive_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceActive_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceActive_user().size(); i++)
        {
            if (plistData[curBody].listForceActive_user().at(i) == NULL)
            {
                ForceActive* temp = new ForceActive();
                plistData[curBody].listForceActive_user().at(i) = temp;
            }
        }
    }

    //Check if the designed body has enough equations in the force for the specified number.
    if (plistData[curBody].listForceActive_user(force)->listEquation().size() < eqn + 1)
    {
        //Add enough equations.
        plistData[curBody].listForceActive_user(force)->listEquation().resize(eqn + 1);
    }

    //Set the active force, user.
    complex<double> input;

    //Get the input value.
    input = plistBody->at(curBody).listForceActive_user(force)->listEquation(eqn);

    //Write the input value.
    plistData[curBody].listForceActive_user(force)->listEquation(eqn) = input;
    input = 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceActive_user(unsigned int force)
{
    //Get total number of equations
    unsigned int eqn = plistBody->at(curBody).listForceActive_user(force)->listEquation().size() - 1;

    //Trigger to use active forces.
    pActiveOnly = pActiveOnly * true;       //Will still evaluate to false if a reactive force was already triggered.

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceActive_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceActive_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceActive_user().size(); i++)
        {
            if (plistData[curBody].listForceActive_user().at(i) == NULL)
            {
                ForceActive* temp = new ForceActive();
                plistData[curBody].listForceActive_user().at(i) = temp;
            }
        }
    }

    //Check if the designed body has enough equations in the force for the specified number.
    if (plistData[curBody].listForceActive_user(force)->listEquation().size() < eqn + 1)
    {
        //Add enough equations.
        plistData[curBody].listForceActive_user(force)->listEquation().resize(eqn + 1);
    }

    //Iterate through all equations for the designated forces
    for (int i = 0; i <= eqn; i++)
    {
            plistData[curBody].listForceActive_user(force)->listEquation(i) =
                    plistBody->at(curBody).listForceActive_user(force)->listEquation(i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceActive_user()
{
    //Get total number of forces
    int force = plistBody->at(curBody).listForceActive_user().size() - 1;

    //Call to use all the active forces
    for (int i = 0; i <= force; i++)
    {
        useForceActive_user(i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceActive_hydro(unsigned int force, unsigned int eqn)
{
    //Trigger to use active forces.
    pActiveOnly = pActiveOnly * true;       //Will still evaluate to false if a reactive force was already triggered.

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceActive_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceActive_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceActive_hydro().size(); i++)
        {
            if (plistData[curBody].listForceActive_hydro().at(i) == NULL)
            {
                ForceActive* temp = new ForceActive();
                plistData[curBody].listForceActive_hydro().at(i) = temp;
            }
        }
    }

    //Check if the designed body has enough equations in the force for the specified number.
    if (plistData[curBody].listForceActive_hydro(force)->listEquation().size() < eqn + 1)
    {
        //Add enough equations.
        plistData[curBody].listForceActive_hydro(force)->listEquation().resize(eqn + 1);
    }

    //Set the active force, hydro.
    complex<double> input;

    //Get the input value.
    input = plistBody->at(curBody).listForceActive_hydro(force)->listEquation(eqn);

    //Write the input value.
    plistData[curBody].listForceActive_hydro(force)->listEquation(eqn) = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceActive_hydro(unsigned int force)
{
    //Get total number of equations
    unsigned int eqn = plistBody->at(curBody).listForceActive_hydro(force)->listEquation().size() - 1;

    //Trigger to use active forces.
    pActiveOnly = pActiveOnly * true;       //Will still evaluate to false if a reactive force was already triggered.

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceActive_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceActive_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceActive_hydro().size(); i++)
        {
            if (plistData[curBody].listForceActive_hydro().at(i) == NULL)
            {
                ForceActive* temp = new ForceActive();
                plistData[curBody].listForceActive_hydro().at(i) = temp;
            }
        }
    }

    //Check if the designed body has enough equations in the force for the specified number.
    if (plistData[curBody].listForceActive_hydro(force)->listEquation().size() < eqn + 1)
    {
        //Add enough equations.
        plistData[curBody].listForceActive_hydro(force)->listEquation().resize(eqn + 1);
    }

    //Iterate through all equations for the designated forces
    for (unsigned int i = 0; i <= eqn; i++)
    {
            plistData[curBody].listForceActive_hydro(force)->listEquation(i) =
                    plistBody->at(curBody).listForceActive_hydro(force)->listEquation(i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceActive_hydro()
{
    //Get total number of forces
    unsigned int force = plistBody->at(curBody).listForceActive_hydro().size() - 1;

    //Call to use all the active forces
    for (unsigned int i = 0; i <= force; i++)
    {
        useForceActive_hydro(i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_user(unsigned int force, unsigned int ord, unsigned int eqn, unsigned int var)
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceReact_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceReact_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceReact_user().size(); i++)
        {
            if (plistData[curBody].listForceReact_user().at(i) == NULL)
            {
                ForceReact* temp = new ForceReact();
                plistData[curBody].listForceReact_user().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceReact_user(force)->listDerivative().size() <= ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceReact_user(force)->listDerivative().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].
            listForceReact_user(force)->
            listDerivative(ord).
            listEquation().
            size() < eqn + 1)
    {
        plistData[curBody].listForceReact_user(force)->listDerivative(ord).listEquation().resize(eqn + 1);
    }

    //Check if the designated equation has enough coefficients for the current number specified.
    if (plistData[curBody].listForceReact_user(force)->listDerivative(ord).
           listEquation(eqn).listCoefficients().size() < var + 1 )
    {
        plistData[curBody].listForceReact_user(force)->listDerivative(ord).
                listEquation(eqn).listCoefficients().resize(var + 1);
    }

    //temporary variable for data transfer
    double input;

    //Get the input value.
    input = plistBody->at(curBody).listForceReact_user(force)->listDerivative(ord).listEquation(eqn).getCoefficient(var);

    //Write the input value.
    plistData[curBody].listForceReact_user(force)->listDerivative(ord).listEquation(eqn).setCoefficient(var, input);

    int input_int;

    //Copy over the data Index.
    input_int = plistBody->at(curBody).listForceReact_user(force)->listDerivative(ord).listEquation(eqn).refDataIndex();

    //Write the input value.
    plistData[curBody].listForceReact_user(force)->listDerivative(ord).listEquation(eqn).refDataIndex() = input_int;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_user(unsigned int force, unsigned int ord, unsigned int eqn)
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceReact_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceReact_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceReact_user().size(); i++)
        {
            if (plistData[curBody].listForceReact_user().at(i) == NULL)
            {
                ForceReact* temp = new ForceReact();
                plistData[curBody].listForceReact_user().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceReact_user(force)->listDerivative().size() <= ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceReact_user(force)->listDerivative().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].
            listForceReact_user(force)->
            listDerivative(ord).
            listEquation().
            size() < eqn + 1)
    {
        plistData[curBody].listForceReact_user(force)->listDerivative(ord).listEquation().resize(eqn + 1);
    }

    //Get the current number of variables
    int var = plistBody->at(curBody).
              listForceReact_user(force)->
              listDerivative(ord).
              listEquation(eqn).
              listCoefficients().size() - 1;

    //Resize the current number of variables
    plistData[curBody].
            listForceReact_user(force)->
            listDerivative(ord).
            listEquation(eqn).
            listCoefficients().resize(var + 1);

    //For each variable, copy over the value.
    for (int i = 0; i <= var; i++)
    {
        //Copy the input value
        plistData[curBody].listForceReact_user(force)->listDerivative(ord).
                listEquation(eqn).setCoefficient(i,
                                                 plistBody->at(curBody).listForceReact_user(force)->
                                                 listDerivative(ord).listEquation(eqn).
                                                 getCoefficient(i)
                                                 );

        //Copy over the data index
        plistData[curBody].listForceReact_user(force)->listDerivative(ord).listEquation(eqn).refDataIndex() =
                plistBody->at(curBody).listForceReact_user(force)->listDerivative(ord).listEquation(eqn).refDataIndex();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_user(unsigned int force, unsigned int ord)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceReact_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceReact_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceReact_user().size(); i++)
        {
            if (plistData[curBody].listForceReact_user().at(i) == NULL)
            {
                ForceReact* temp = new ForceReact();
                plistData[curBody].listForceReact_user().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceReact_user(force)->listDerivative().size() <= ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceReact_user(force)->listDerivative().resize(ord + 1);
    }

    //Get the current number of equations
    int eqn = plistBody->at(curBody).
              listForceReact_user(force)->
              listDerivative(ord).
              listEquation().size() - 1;

    //Iterate through equation and apply the useForce method.
    for (int i = 0; i <= eqn; i++)
    {
        useForceReact_user(force, ord, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_user(unsigned int force)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceReact_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceReact_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceReact_user().size(); i++)
        {
            if (plistData[curBody].listForceReact_user().at(i) == NULL)
            {
                ForceReact* temp = new ForceReact();
                plistData[curBody].listForceReact_user().at(i) = temp;
            }
        }
    }

    //Get the maximum number of forces
    int ord = plistBody->at(curBody).
              listForceReact_user(force)->
              listDerivative().size() - 1;

    //Iterate through for each derivative and apply the useForce method
    for (int i = 0; i <= ord; i++)
    {
        useForceReact_user(force, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_user()
{
    //Get the number of forces
    unsigned int force = plistBody->at(curBody).
                listForceReact_user().size() - 1;

    //Call the useForce method for each force object.
    for (unsigned int i = 0; i <= force; i++)
    {
        useForceReact_user(i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_hydro(unsigned int force, unsigned int ord, unsigned int eqn, unsigned int var)
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceReact_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceReact_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceReact_hydro().size(); i++)
        {
            if (plistData[curBody].listForceReact_hydro().at(i) == NULL)
            {
                ForceReact* temp = new ForceReact();
                plistData[curBody].listForceReact_hydro().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceReact_hydro(force)->listDerivative().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceReact_hydro(force)->listDerivative().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).listEquation().size() < eqn + 1)
    {
        plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).listEquation().resize(eqn + 1);
    }

    //Check if the designated equation has enough coefficients for the current number specified.
    if (plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).
             listEquation(eqn).listCoefficients().size() < var + 1 )
      {
          plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).
                  listEquation(eqn).listCoefficients().resize(var + 1);
      }

    //temporary variable for data transfer
    double input;

    //Getthe input value.
    input = plistBody->at(curBody).listForceReact_hydro(force)->listDerivative(ord).listEquation(eqn).getCoefficient(var);

    //Write the input value.
    plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).listEquation(eqn).setCoefficient(var, input);

    int input_int;

    //Copy over the data Index.
    input_int = plistBody->at(curBody).listForceReact_hydro(force)->listDerivative(ord).listEquation(eqn).refDataIndex();

    //Write the input value.
    plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).listEquation(eqn).refDataIndex() = input_int;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_hydro(unsigned int force, unsigned int ord, unsigned int eqn)
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceReact_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceReact_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceReact_hydro().size(); i++)
        {
            if (plistData[curBody].listForceReact_hydro().at(i) == NULL)
            {
                ForceReact* temp = new ForceReact();
                plistData[curBody].listForceReact_hydro().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceReact_hydro(force)->listDerivative().size() <= ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceReact_hydro(force)->listDerivative().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].
            listForceReact_hydro(force)->
            listDerivative(ord).
            listEquation().
            size() < eqn + 1)
    {
        plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).listEquation().resize(eqn + 1);
    }

    //Get the current number of variables
    unsigned int var = plistBody->at(curBody).
              listForceReact_hydro(force)->
              listDerivative(ord).
              listEquation(eqn).
              listCoefficients().size() - 1;

    //Resize the current number of variables
    plistData[curBody].
            listForceReact_hydro(force)->
            listDerivative(ord).
            listEquation(eqn).
            listCoefficients().resize(var + 1);

    //For each variable, copy over the value.
    for (unsigned int i = 0; i <= var; i++)
    {
        //Copy the input value
        plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).
                listEquation(eqn).setCoefficient(i,
                                                 plistBody->at(curBody).listForceReact_hydro(force)->
                                                 listDerivative(ord).listEquation(eqn).
                                                 getCoefficient(i)
                                                 );

        //Copy over the data index
        plistData[curBody].listForceReact_hydro(force)->listDerivative(ord).listEquation(eqn).refDataIndex() =
                plistBody->at(curBody).listForceReact_hydro(force)->listDerivative(ord).listEquation(eqn).refDataIndex();
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_hydro(unsigned int force, unsigned int ord)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceReact_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceReact_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceReact_hydro().size(); i++)
        {
            if (plistData[curBody].listForceReact_hydro().at(i) == NULL)
            {
                ForceReact* temp = new ForceReact();
                plistData[curBody].listForceReact_hydro().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceReact_hydro(force)->listDerivative().size() <= ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceReact_hydro(force)->listDerivative().resize(ord + 1);
    }

    //Get the current number of equations
    unsigned int eqn = plistBody->at(curBody).
              listForceReact_hydro(force)->
              listDerivative(ord).
              listEquation().size() - 1;

    //Iterate through equation and apply the useForce method.
    for (unsigned int i = 0; i <= eqn; i++)
    {
        useForceReact_hydro(force, ord, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_hydro(unsigned int force)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceReact_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceReact_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceReact_hydro().size(); i++)
        {
            if (plistData[curBody].listForceReact_hydro().at(i) == NULL)
            {
                ForceReact* temp = new ForceReact();
                plistData[curBody].listForceReact_hydro().at(i) = temp;
            }
        }
    }

    //Get the maximum number of derivatives
    unsigned int ord = plistBody->at(curBody).
              listForceReact_hydro(force)->
              listDerivative().size() - 1;

    //Iterate through for each derivative and apply the useForce method
    for (unsigned int i = 0; i <= ord; i++)
    {
        useForceReact_hydro(force, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceReact_hydro()
{
    //Get the number of forces
    unsigned int force = plistBody->at(curBody).
                listForceReact_hydro().size() - 1;

    //Call the useForce method for each force object.
    for (unsigned int i = 0; i <= force; i++)
    {
        useForceReact_hydro(i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_user(unsigned int force, unsigned int ord, unsigned int eqn, unsigned int var)
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceCross_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceCross_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceCross_user().size(); i++)
        {
            if (plistData[curBody].listForceCross_user().at(i) == NULL)
            {
                ForceCross* temp = new ForceCross();
                plistData[curBody].listForceCross_user().at(i) = temp;
            }
        }
    }

    //Check if the list of linked cross-bodies has enough entries for the current number specified.
    if (pCompCrossBod_user.size() < force + 1)
    {
        //Add enough entries.
        pCompCrossBod_user.resize(force + 1);
    }

    //Check if the designated body has enough linking cross body entries for the current specified number.
    if (plistData[curBody].listCrossBody_user().size() < force + 1)
    {
        //Add enough linking cross body entries.
        plistData[curBody].listCrossBody_user().resize(force + 1);
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceCross_user(force)->listDerivative().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceCross_user(force)->listDerivative().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].listForceCross_user(force)->listDerivative(ord).listEquation().size() < eqn + 1)
    {
        plistData[curBody].listForceCross_user(force)->listDerivative(ord).listEquation().resize(eqn + 1);
    }

    //Check if the designated equation has enough coefficients for the current number specified.
    if (plistData[curBody].listForceCross_user(force)->listDerivative(ord).
           listEquation(eqn).listCoefficients().size() < var + 1 )
    {
        plistData[curBody].listForceCross_user(force)->listDerivative(ord).
                listEquation(eqn).listCoefficients().resize(var + 1);
    }  

    //temporary variable for data transfer
    double input;

    //Getthe input value.
    input = plistBody->at(curBody).listForceCross_user(force)->listDerivative(ord).listEquation(eqn).getCoefficient(var);

    //Write the input value.
    plistData[curBody].listForceCross_user(force)->listDerivative(ord).listEquation(eqn).setCoefficient(var, input);

    //Copy over the data Index.
    plistData[curBody].listForceCross_user(force)->listDerivative(ord).listEquation(eqn).refDataIndex() =
            plistBody->at(curBody).listForceCross_user(force)->listDerivative(ord).listEquation(eqn).refDataIndex();

    //Copy over the linked body reference.
    for (unsigned int i = 0 ; i < plistBody->size() ; i++)
    {
        //Check if the cross body force matches.
        if (plistBody->at(i) == (plistBody->at(curBody).listCrossBody_user(force)))
        {
            //True.  Copy integer and stop.
            pCompCrossBod_user[force] = i;
            break;
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_user(unsigned int force, unsigned int ord, unsigned int eqn)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceCross_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceCross_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceCross_user().size(); i++)
        {
            if (plistData[curBody].listForceCross_user().at(i) == NULL)
            {
                ForceCross* temp = new ForceCross();
                plistData[curBody].listForceCross_user().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceCross_user(force)->listDerivative().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceCross_user(force)->listDerivative().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].
            listForceCross_user(force)->
            listDerivative(ord).
            listEquation().
            size() < eqn + 1)
    {
        plistData[curBody].listForceCross_user(force)->listDerivative(ord).listEquation().resize(eqn + 1);
    }

    //Get the current number of variables
    unsigned int var = plistBody->at(curBody).
              listForceCross_user(force)->
              listDerivative(ord).
              listEquation(eqn).
              listCoefficients().size() - 1;

    //For each variable, call the useForce method.
    for (unsigned int i = 0; i <= var; i++)
    {
        useForceCross_user(force, ord, eqn, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_user(unsigned int force, unsigned int ord)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceCross_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceCross_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceCross_user().size(); i++)
        {
            if (plistData[curBody].listForceCross_user().at(i) == NULL)
            {
                ForceCross* temp = new ForceCross();
                plistData[curBody].listForceCross_user().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceCross_user(force)->listDerivative().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceCross_user(force)->listDerivative().resize(ord + 1);
    }

    //Get the current number of equations
    int eqn = plistBody->at(curBody).
              listForceCross_user(force)->
              listDerivative(ord).
              listEquation().size() - 1;

    //Iterate through equation and apply the useForce method.
    for (int i = 0; i <= eqn; i++)
    {
        useForceCross_user(force, ord, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_user(unsigned int force)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceCross_user().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceCross_user().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceCross_user().size(); i++)
        {
            if (plistData[curBody].listForceCross_user().at(i) == NULL)
            {
                ForceCross* temp = new ForceCross();
                plistData[curBody].listForceCross_user().at(i) = temp;
            }
        }
    }

    //Get the maximum number of forces
    int ord = plistBody->at(curBody).
              listForceCross_user(force)->
              listDerivative().size() - 1;

    //Iterate through for each derivative and apply the useForce method
    for (int i = 0; i <= ord; i++)
    {
        useForceCross_user(force, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_user()
{
    //Get the number of forces
    int force = plistBody->at(curBody).
                listForceCross_user().size() - 1;

    //Call the useForce method for each force object.
    for (int i = 0; i <= force; i++)
    {
        useForceCross_user(i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_hydro(unsigned int force, unsigned int ord, unsigned int eqn, unsigned int var)
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceCross_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceCross_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceCross_hydro().size(); i++)
        {
            if (plistData[curBody].listForceCross_hydro().at(i) == NULL)
            {
                ForceCross* temp = new ForceCross();
                plistData[curBody].listForceCross_hydro().at(i) = temp;
            }
        }
    }

    //Check if the designated body has enough linking cross body entries for the current specified number.
    if (plistData[curBody].listCrossBody_hydro().size() < force + 1)
    {
        //Add enough linking cross body entries.
        plistData[curBody].listCrossBody_hydro().resize(force + 1);
    }

    //Check if the list of linked cross-bodies has enough entries for the current number specified.
    if (pCompCrossBod_hydro.size() < force + 1)
    {
        //Add enough entries.
        pCompCrossBod_hydro.resize(force + 1);
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceCross_hydro(force)->listDerivative().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceCross_hydro(force)->listDerivative().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].listForceCross_hydro(force)->listDerivative(ord).listEquation().size() < eqn + 1)
    {
        plistData[curBody].listForceCross_hydro(force)->listDerivative(ord).listEquation().resize(eqn + 1);
    }

    //Check if the designated equation has enough coefficients for the current number specified.
    if (plistData[curBody].listForceCross_hydro(force)->listDerivative(ord).listEquation(eqn).listCoefficients().size() < var + 1 )
    {
        plistData[curBody].listForceCross_hydro(force)->listDerivative(ord).
                listEquation(eqn).listCoefficients().resize(var + 1);
    }

    //temporary variable for data transfer
    double input;

    //Getthe input value.
    input = plistBody->at(curBody).listForceCross_hydro(force)->listDerivative(ord).listEquation(eqn).getCoefficient(var);

    //Write the input value.
    plistData[curBody].listForceCross_hydro(force)->listDerivative(ord).listEquation(eqn).setCoefficient(var, input);



    //Copy over the data Index.
    plistData[curBody].listForceCross_hydro(force)->listDerivative(ord).listEquation(eqn).refDataIndex() =
            plistBody->at(curBody).listForceCross_hydro(force)->listDerivative(ord).listEquation(eqn).refDataIndex();

    //Copy over the linked body reference.
    for (unsigned int i = 0 ; i < plistBody->size() ; i++)
    {
        //Check if the cross body force matches.
        if (plistBody->at(i) == plistBody->at(curBody).listCrossBody_hydro(force))
        {
            //True.  Copy integer and stop.
            pCompCrossBod_hydro[force] = i;
            break;
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_hydro(unsigned int force, unsigned int ord, unsigned int eqn)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceCross_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceCross_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceCross_hydro().size(); i++)
        {
            if (plistData[curBody].listForceCross_hydro().at(i) == NULL)
            {
                ForceCross* temp = new ForceCross();
                plistData[curBody].listForceCross_hydro().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceCross_hydro(force)->listDerivative().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceCross_hydro(force)->listDerivative().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].
            listForceCross_hydro(force)->
            listDerivative(ord).
            listEquation().
            size() < eqn + 1)
    {
        plistData[curBody].listForceCross_hydro(force)->listDerivative(ord).listEquation().resize(eqn + 1);
    }

    //Get the current number of variables
    unsigned int var = plistBody->at(curBody).
              listForceCross_hydro(force)->
              listDerivative(ord).
              listEquation(eqn).
              listCoefficients().size() - 1;

    //For each variable, call the useForce method.
    for (unsigned int i = 0; i <= var; i++)
    {
        useForceCross_hydro(force, ord, eqn, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_hydro(unsigned int force, unsigned int ord)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceCross_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceCross_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceCross_hydro().size(); i++)
        {
            if (plistData[curBody].listForceCross_hydro().at(i) == NULL)
            {
                ForceCross* temp = new ForceCross();
                plistData[curBody].listForceCross_hydro().at(i) = temp;
            }
        }
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceCross_hydro(force)->listDerivative().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceCross_hydro(force)->listDerivative().resize(ord + 1);
    }

    //Get the current number of equations
    int eqn = plistBody->at(curBody).
              listForceCross_hydro(force)->
              listDerivative(ord).
              listEquation().size() - 1;

    //Iterate through equation and apply the useForce method.
    for (int i = 0; i <= eqn; i++)
    {
        useForceCross_hydro(force, ord, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_hydro(unsigned int force)
{
    //Check if the designated body has enough forces for the current specified number.
    if (plistData[curBody].listForceCross_hydro().size() < force + 1)
    {
        //Add enough forces.
        plistData[curBody].listForceCross_hydro().resize(force + 1);

        for (unsigned int i = 0; i < plistData[curBody].listForceCross_hydro().size(); i++)
        {
            if (plistData[curBody].listForceCross_hydro().at(i) == NULL)
            {
                ForceCross* temp = new ForceCross();
                plistData[curBody].listForceCross_hydro().at(i) = temp;
            }
        }
    }

    //Get the maximum number of forces
    int ord = plistBody->at(curBody).
              listForceCross_hydro(force)->
              listDerivative().size() - 1;

    //Iterate through for each derivative and apply the useForce method
    for (int i = 0; i <= ord; i++)
    {
        useForceCross_hydro(force, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceCross_hydro()
{
    //Get the number of forces
    int force = plistBody->at(curBody).
                listForceCross_hydro().size() - 1;

    //Call the useForce method for each force object.
    for (int i = 0; i <= force; i++)
    {
        useForceCross_hydro(i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceMass(unsigned int eqn, unsigned int var)
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Don't need to check that the mass matrix created successfully.
    //Construction of body object automatically initializes with a mass matrix of the correct size.

    try
    {
        //Check if current body is assigned a value.
        if (curBody < 0 || curBody > plistData.size() - 1)
        {
            //Throw error for current body not defined.
            throw 1;
        }

        //Copy over the mass matrix entry.
        plistData[curBody].MassMatrix()(eqn, var) = plistBody->at(curBody).getMassMatrix()(eqn, var);
    }
    catch (int err)
    {
        //Error handler
        switch(err)
        {
        case 1:     //Case for curBody not defined.
            //Add error handler in here later.
            break;
        default:    //Case for any unknown error.
            int ouch = 1;
        }

    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceMass(unsigned int eqn)
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Don't need to check that the mass matrix created successfully.
    //Construction of body object automatically initializes with a mass matrix of the correct size.

    //Get total number of variables
    int var = plistData.at(curBody).getMassMatrix().n_cols - 1;

    //Copy over all variables in the specified equation
    for (int i = 0; i <= var; i++)
    {
        plistData[curBody].MassMatrix()(eqn, i) =
                plistBody->at(curBody).getMassMatrix()(eqn, i);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::useForceMass()
{
    //Turn off active force evaluation.
    pActiveOnly = false;

    //Check if there are enough bodies for the current called functions.
    //All happens in the fillBodies function.
    fillBodies();

    //Don't need to check that the mass matrix created successfully.
    //Construction of body object automatically initializes with a mass matrix of the correct size.

    //Copy the matrix directly over
    plistData[curBody].MassMatrix() = plistBody->at(curBody).getMassMatrix();
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionModel::getMatForceActive_user(int force)
{
    //Create force matrix.
    cx_mat outputmat;       //output matrix
    int n_row;               //Number of rows

    //resize output matrix
    n_row = plistBody->at(curBody).getEquationCount();
    outputmat.resize(n_row,1);

    //Run evaluation of equations for each equation in the force object.
    //Reset the model
    Reset();
    for (unsigned int i = 0; i < plistEquations.size(); i++)
    {
        //Activate the appropriate user force.
        useForceActive_user(force, i);

        //Evaluate the equation of motion and store in matrix.
        outputmat(i,0) = Evaluate(i);
    }

    //Write output
    return outputmat;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionModel::getMatForceActive_hydro(int force)
{
    //Create force matrix.
    cx_mat outputmat;       //output matrix
    int n_row;               //Number of rows

    //resize output matrix
    n_row = plistBody->at(curBody).getEquationCount();
    outputmat.resize(n_row,1);

    //Run evaluation of equations for each equation in the force object.
    //Reset the model
    Reset();
    for (unsigned int i = 0; i < plistEquations.size(); i++)
    {
        //Activate the appropriate hydro force.
        useForceActive_hydro(force, i);

        //Evaluate the equation of motion and store in matrix.
        outputmat(i,0) = Evaluate(i);
    }

    //Write output
    return outputmat;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionModel::getMatForceReact_user(int force, int ord)
{
    //Create force matrix
    cx_mat outputmat;       //outputmatrix
    int n_row;              //number of rows for new matrix.

    //resize output matrix.
    n_row = plistBody->at(curBody).getEquationCount();
    outputmat.resize(n_row, n_row);         //Only needed one dimension because the matrix must be square.

    //Run evaluation for each variable in the force object.
    for (unsigned int i = 0; i < plistEquations.size(); i++)
    {
        //Reset the model
        Reset();

        //Run evaluation for each equation in the force object.
        for (unsigned int j = 0; j < plistEquations.size(); j++)
        {
            //Promote the reactive force to use, on the correct order of derivative.
            useForceReact_user(force, ord, j, i);

            //Evaluate the equation of motion and store in matrix.
            outputmat(j,i) = Evaluate(j);
        }
    }

    //Write output
    return outputmat;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionModel::getMatForceReact_hydro(int force, int ord)
{
    //Create force matrix
    cx_mat outputmat;       //outputmatrix
    int n_row;              //number of rows for new matrix.

    //resize output matrix.
    n_row = plistBody->at(curBody).getEquationCount();
    outputmat.resize(n_row, n_row);         //Only needed one dimension because the matrix must be square.

    //Run evaluation for each variable in the force object.
    for (unsigned int i = 0; i < plistEquations.size(); i++)
    {
        //Reset the model
        Reset();

        //Run evaluation for each equation in the force object.
        for (unsigned int j = 0; j < plistEquations.size(); j++)
        {
            //Promote the reactive force to use, on the correct order of derivative.
            useForceReact_hydro(force, ord, j, i);

            //Evaluate the equation of motion and store in matrix.
            outputmat(j,i) = Evaluate(j);
        }
    }

    //Write output
    return outputmat;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionModel::getMatForceCross_user(int force, int ord)
{
    //Create force matrix
    cx_mat outputmat;       //outputmatrix
    int n_row;              //number of rows for new matrix.

    //resize output matrix.
    n_row = plistBody->at(curBody).getEquationCount();
    outputmat.resize(n_row, n_row);         //Only needed one dimension because the matrix must be square.

    //Run evaluation for each variable in the force object.
    for (unsigned int i = 0; i < plistEquations.size(); i++)
    {
        //Reset the model
        Reset();

        //Run evaluation for each equation in the force object.
        for (unsigned int j = 0; j < plistEquations.size(); j++)
        {
            //Promote the reactive force to use, on the correct order of derivative.
            useForceCross_user(force, ord, j, i);

            //Evaluate the equation of motion and store in matrix.
            outputmat(j,i) = Evaluate(j);
        }
    }

    //Write output
    return outputmat;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionModel::getMatForceCross_hydro(int force, int ord)
{
    //Create force matrix
    cx_mat outputmat;       //outputmatrix
    int n_row;              //number of rows for new matrix.

    //resize output matrix.
    n_row = plistBody->at(curBody).getEquationCount();
    outputmat.resize(n_row, n_row);         //Only needed one dimension because the matrix must be square.

    //Run evaluation for each variable in the force object.
    for (unsigned int i = 0; i < plistEquations.size(); i++)
    {
        //Reset the model
        Reset();

        //Run evaluation for each equation in the force object.
        for (unsigned int j = 0; j < plistEquations.size(); j++)
        {
            //Promote the reactive force to use, on the correct order of derivative.
            useForceCross_hydro(force, ord, j, i);

            //Evaluate the equation of motion and store in matrix.
            outputmat(j,i) = Evaluate(j);
        }
    }

    //Write output
    return outputmat;
}

//------------------------------------------Function Separator --------------------------------------------------------
cx_mat MotionModel::getMatForceMass()
{
    //Create force matrix
    cx_mat outputmat;       //outputmatrix
    int n_row;              //number of rows for new matrix.

    //resize output matrix.
    n_row = plistBody->at(curBody).getEquationCount();
    outputmat.resize(n_row, n_row);         //Only needed one dimension because the matrix must be square.

    //Run evaluation for each variable in the force object.
    for (unsigned int i = 0; i < plistEquations.size(); i++)
    {
        //Reset the model
        Reset();

        //Run evaluation for each equation in the force object.
        for (unsigned int j = 0; j < plistEquations.size(); j++)
        {
            //Promote the reactive force to use, on the correct order of derivative.
            useForceMass(j, i);

            //Evaluate the equation of motion and store in matrix.
            outputmat(j,i) = Evaluate(j);
        }
    }

    //Write output
    return outputmat;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> MotionModel::Evaluate(int eqn)
{
    //Trigger evaluation of the equation objects.
    return plistEquations[eqn]->Evaluate();
}

//------------------------------------------Function Separator --------------------------------------------------------
int MotionModel::numEquations()
{
    //Reports the number of equations used.
    return plistEquations.size();
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<int> &MotionModel::refDataIndex()
{
    //Create a vector containing the data indices used by the equations.
    vector<int> dataOut(this->numEquations());

    for (int i = 0; i < this->numEquations(); i++)
    {
        dataOut[i] = plistEquations[i]->refDataIndex();
    }

    //Write output
    return dataOut;
}

//------------------------------------------Function Separator --------------------------------------------------------
int MotionModel::MaxDataIndex()
{
    vector<int> listIndex;

    //Get the list.
    listIndex = this->refDataIndex();

    //Search through the list to find the largest size.
    int maxSizeLocal = -1;
    for (unsigned int i = 0; i < listIndex.size() ; i++)
    {
        if (maxSizeLocal < listIndex[i])
        {
            maxSizeLocal = listIndex[i];
        }
    }

    //Write output
    return maxSizeLocal;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::setName(string nameIn)
{
    pName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
string MotionModel::getName()
{
    return pName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::setDescription(string DescIn)
{
    pDesc = DescIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
string MotionModel::getDescription()
{
    return pDesc;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body> &MotionModel::listBody()
{
    return *plistBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &MotionModel::listBody(int bodIn)
{
    return plistBody->at(bodIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body> &MotionModel::listData()
{
    return plistData;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &MotionModel::listData(int dataIn)
{
    return plistData.at(dataIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<EquationofMotion *> &MotionModel::listEquation()
{
    return plistEquations;
}

//------------------------------------------Function Separator --------------------------------------------------------
EquationofMotion &MotionModel::listEquation(int eqIn)
{
    return *plistEquations.at(eqIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::AddEquation(EquationofMotion *eqIn)
{
    //EquationofMotion *eqTemp = new EquationofMotion(this);
    //*eqTemp = eqIn;
    plistEquations.push_back(eqIn);
}


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::DefineEquations()
{
    //Just a basic definition to keep the compiler happy.
}

//==========================================Section Separator =========================================================
//Private Functions
//------------------------------------------Function Separator --------------------------------------------------------
Mat<double> MotionModel::CopyZero(Mat<double> & copyMat)
{
    //Copies the matrix and outputs a matrix of the same size with zeros.
    Mat<double> matOut;

    matOut.zeros(copyMat.n_rows, copyMat.n_cols);

    return matOut;
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::fillBodies()
{
    //Fills out the body array
    if (plistBody->size() != plistData.size())
    {
        //Not equal sized lists.  Create the list.
        plistData.resize(plistBody->size());
        for (unsigned int i = 0; i < plistBody->size(); i++)
        {
            //Assign some critical information that should always be carried through.
            plistData[i].setBodyName(
                        plistBody->at(i).getBodyName());
            plistData[i].setHydroBodName(
                        plistBody->at(i).getHydroBodName());
            plistData[i].setHeading(
                        plistBody->at(i).getHeading());
            plistData[i].refPosn() = plistBody->at(i).getPosn();
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::ConstructorCommon()
{
    //Initialize Active force variable.
    pActiveOnly = true;

    //Create initial value for wave frequency
    pFreq = 0.0;

    //Create initial value for Body.
    curBody = -1;

    //Define the equations
    DefineEquations();
}
