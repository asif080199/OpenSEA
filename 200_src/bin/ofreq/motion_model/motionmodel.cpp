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

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel::MotionModel()
{
    //Default constructor.

    //Initialize Active force variable.
    pActiveOnly = true;

    //Create initial value for wave frequency
    pFreq = 0.0;
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel::MotionModel(vector<Body> &listBodIn)
{
    //Constructor that sets the body reference.
    setListBodies(listBodIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
MotionModel::~MotionModel()
{
    //Default destructor.
}

//------------------------------------------Function Separator --------------------------------------------------------
void MotionModel::setListBodies(vector<Body> &listBodIn)
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
    }

    //Check if the designed body has enough equations in the force for the specified number.
    if (plistData[curBody].listForceActive_user()[force]->listEquations().size() < eqn + 1)
    {
        //Add enough equations.
        plistData[curBody].listForceActive_user()[force]->listEquations().resize(eqn + 1);
    }

    //Set the active force, user.
    complex<double> input;

    //Get the input value.
    input = plistBody->at(curBody).listForceActive_user()[force]->listEquations()[eqn];

    //Write the input value.
    plistData[curBody].listForceActive_user()[force]->listEquations()[eqn] = input;
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
    }

    //Check if the designed body has enough equations in the force for the specified number.
    if (plistData[curBody].listForceActive_hydro(force)->listEquations().size() < eqn + 1)
    {
        //Add enough equations.
        plistData[curBody].listForceActive_hydro(force)->listEquations().resize(eqn + 1);
    }

    //Set the active force, hydro.
    complex<double> input;

    //Get the input value.
    input = plistBody->at(curBody).listForceActive_hydro(force)->listEquations()[eqn];

    //Write the input value.
    plistData[curBody].listForceActive_hydro(force)->listEquations()[eqn] = input;
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
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceReact_user(force)->listDerivatives().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceReact_user(force)->listDerivatives().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].
            listForceReact_user(force)->
            getDerivative(ord).
            listEquations().
            size() < eqn + 1)
    {
        plistData[curBody].listForceReact_user(force)->getDerivative(ord).listEquations().resize(eqn + 1);
    }

    //Check if the designated equation has enough coefficients for the current number specified.
    if (plistData[curBody].listForceReact_user(force)->getDerivative(ord).
           refEquation(eqn).listCoefficients().size() < var + 1 )
    {
        plistData[curBody].listForceReact_user(force)->getDerivative(ord).
                refEquation(eqn).listCoefficients().resize(var + 1);
    }

    //temporary variable for data transfer
    double input;

    //Getthe input value.
    input = plistBody->at(curBody).listForceReact_user(force)->getDerivative(ord).refEquation(eqn).getCoefficient(var);

    //Write the input value.
    plistData[curBody].listForceReact_user(force)->getDerivative(ord).refEquation(eqn).setCoefficient(var, input);

    int input_int;

    //Copy over the data Index.
    input_int = plistBody->at(curBody).listForceReact_user(force)->getDerivative(ord).refEquation(eqn).refDataIndex();

    //Write the input value.
    plistData[curBody].listForceReact_user(force)->getDerivative(ord).refEquation(eqn).refDataIndex() = input_int;
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
    }

    //Check if the designated force has enough derivatives for the current number specified.
    if (plistData[curBody].listForceReact_hydro()[force]->listDerivatives().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceReact_hydro()[force]->listDerivatives().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].listForceReact_hydro()[force]->getDerivative(ord).listEquations().size() < eqn + 1)
    {
        plistData[curBody].listForceReact_hydro()[force]->getDerivative(ord).listEquations().resize(eqn + 1);
    }

    //Check if the designated equation has enough coefficients for the current number specified.
    if (plistData[curBody].listForceReact_hydro()[force]->getDerivative(ord).
           refEquation(eqn).listCoefficients().size() < var + 1 )
    {
        plistData[curBody].listForceReact_hydro()[force]->getDerivative(ord).
                refEquation(eqn).listCoefficients().resize(var + 1);
    }

    //temporary variable for data transfer
    double input;

    //Getthe input value.
    input = plistBody->at(curBody).listForceReact_hydro()[force]->getDerivative(ord).refEquation(eqn).getCoefficient(var);

    //Write the input value.
    plistData[curBody].listForceReact_hydro()[force]->getDerivative(ord).refEquation(eqn).setCoefficient(var, input);

    int input_int;

    //Copy over the data Index.
    input_int = plistBody->at(curBody).listForceReact_hydro()[force]->getDerivative(ord).refEquation(eqn).refDataIndex();

    //Write the input value.
    plistData[curBody].listForceReact_hydro()[force]->getDerivative(ord).refEquation(eqn).refDataIndex() = input_int;
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
    if (plistData[curBody].listForceCross_user()[force]->listDerivatives().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceCross_user()[force]->listDerivatives().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].listForceCross_user()[force]->getDerivative(ord).listEquations().size() < eqn + 1)
    {
        plistData[curBody].listForceCross_user()[force]->getDerivative(ord).listEquations().resize(eqn + 1);
    }

    //Check if the designated equation has enough coefficients for the current number specified.
    if (plistData[curBody].listForceCross_user()[force]->getDerivative(ord).
           refEquation(eqn).listCoefficients().size() < var + 1 )
    {
        plistData[curBody].listForceCross_user()[force]->getDerivative(ord).
                refEquation(eqn).listCoefficients().resize(var + 1);
    }  

    //temporary variable for data transfer
    double input;

    //Getthe input value.
    input = plistBody->at(curBody).listForceCross_user()[force]->getDerivative(ord).refEquation(eqn).getCoefficient(var);

    //Write the input value.
    plistData[curBody].listForceCross_user()[force]->getDerivative(ord).refEquation(eqn).setCoefficient(var, input);

    //Copy over the data Index.
    plistData[curBody].listForceCross_user()[force]->getDerivative(ord).refEquation(eqn).refDataIndex() =
            plistBody->at(curBody).listForceCross_user()[force]->getDerivative(ord).refEquation(eqn).refDataIndex();

    //Copy over the linked body reference.
    for (unsigned int i = 0 ; i < plistBody->size() ; i++)
    {
        //Check if the cross body force matches.
        if (plistBody->at(i) == *(plistBody->at(curBody).listCrossBody_user()[force]))
        {
            //True.  Copy integer and stop.
            pCompCrossBod_user[force] = i;
            break;
        }
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
    if (plistData[curBody].listForceCross_hydro()[force]->listDerivatives().size() < ord + 1)
    {
        //Add enough derivatives.
        plistData[curBody].listForceCross_hydro()[force]->listDerivatives().resize(ord + 1);
    }

    //Check if the designated derivative has enough equations for the current number specified.
    if (plistData[curBody].listForceCross_hydro()[force]->getDerivative(ord).listEquations().size() < eqn + 1)
    {
        plistData[curBody].listForceCross_hydro()[force]->getDerivative(ord).listEquations().resize(eqn + 1);
    }

    //Check if the designated equation has enough coefficients for the current number specified.
    if (plistData[curBody].listForceCross_hydro()[force]->getDerivative(ord).refEquation(eqn).listCoefficients().size() < var + 1 )
    {
        plistData[curBody].listForceCross_hydro()[force]->getDerivative(ord).
                refEquation(eqn).listCoefficients().resize(var + 1);
    }

    //temporary variable for data transfer
    double input;

    //Getthe input value.
    input = plistBody->at(curBody).listForceCross_hydro()[force]->getDerivative(ord).refEquation(eqn).getCoefficient(var);

    //Write the input value.
    plistData[curBody].listForceCross_hydro()[force]->getDerivative(ord).refEquation(eqn).setCoefficient(var, input);



    //Copy over the data Index.
    plistData[curBody].listForceCross_hydro()[force]->getDerivative(ord).refEquation(eqn).refDataIndex() =
            plistBody->at(curBody).listForceCross_hydro()[force]->getDerivative(ord).refEquation(eqn).refDataIndex();

    //Copy over the linked body reference.
    for (unsigned int i = 0 ; i < plistBody->size() ; i++)
    {
        //Check if the cross body force matches.
        if (plistBody->at(i) == *(plistBody->at(curBody).listCrossBody_hydro()[force]))
        {
            //True.  Copy integer and stop.
            pCompCrossBod_hydro[force] = i;
            break;
        }
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

    //Copy over the mass matrix entry.
    plistData[curBody].getMassMatrix()(eqn, var) = plistBody->at(curBody).getMassMatrix()(eqn, var);
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
    return plistEquations[eqn].Evaluate();
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
        dataOut[i] = plistEquations[i].refDataIndex();
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
vector<Body> &MotionModel::listBodies()
{
    return *plistBody;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Body> &MotionModel::listData()
{
    return plistData;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<EquationofMotion> &MotionModel::listEquations()
{
    return plistEquations;
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &MotionModel::refBody(int bodIn)
{
    return plistBody->at(bodIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
Body &MotionModel::refData(int dataIn)
{
    return plistData.at(dataIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
EquationofMotion &MotionModel::refEquation(int eqIn)
{
    return plistEquations.at(eqIn);
}

//==========================================Section Separator =========================================================
//Protected Functions


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
