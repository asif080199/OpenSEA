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



//######################################### Class Separator ###########################################################
#include "equationofmotion.h"
#include "motionmodel.h"        //Include motion model here to prevent recursive class definitions in headers, but
                                //still access all members of class MotionModel.

using namespace std;
using namespace arma;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Members
int EquationofMotion::undefArg = -1;   /**< Integer value for undefined argument in the summation function.*/

//==========================================Section Separator =========================================================
//Public Members

//------------------------------------------Function Separator --------------------------------------------------------
EquationofMotion::EquationofMotion(MotionModel *modelIn)
{
    //Default constructor.  Requires declaration
    //of the object which created the equation object.
    ConstructorCommon(modelIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
EquationofMotion::EquationofMotion(MotionModel *modelIn, string NameIn)
{
    //Constructor with specifying name.

    //Assign the calling body to private variable.
    ConstructorCommon(modelIn);

    //Assign the equation name
    pName = NameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
EquationofMotion::EquationofMotion(MotionModel *modelIn, string NameIn, int IndexIn)
{
    //Constructor with specifying name and data index of the equation.

    //Assign the calling body to private variable.
    ConstructorCommon(modelIn);

    //Assign the equation name
    pName = NameIn;

    //Assign the data index.
    pDataIndex = IndexIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
EquationofMotion::~EquationofMotion()
{
    //Default destructor
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::Evaluate()
{
    /*Evaluate is just another expression of the Formula object.
     *They are separated because mathematicians will write children of this object.
     *Many of the functions implemented in this object are purely to create names and other nomenclature
     *to mathematicians.
     *
     *For a quick summary:  Evaluate is the function that actually returns the value of the equation
     *of motion object.
     */

    return this->setFormula();
}

//------------------------------------------Function Separator --------------------------------------------------------
void EquationofMotion::setDataIndex(int DataIn)
{
    //Sets the data index.
    pDataIndex = DataIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
int EquationofMotion::getDataIndex()
{
    //Returns the data index.
    return this->eqn();
}

//------------------------------------------Function Separator --------------------------------------------------------
int &EquationofMotion::refDataIndex()
{
    //Exposes the data index object.  Passed by reference.
    //Get index of equation of motion.
    if (pDataIndex < 0)
    {
        //Data index not defined.
        //Use private index.
        return pPrivateIndex;
    }
    else
    {
        return pDataIndex;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void EquationofMotion::setArguments(int argn, vector<double> argv)
{
    //Sets the input arguments.  These are arbitrary arguments that the user may or may not use.
    argcount = argn;
    argvalue = argv;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &EquationofMotion::refName()
{
    //Exposes the object name
    return pName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void EquationofMotion::setName(std::string nameIn)
{
    pName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &EquationofMotion::refDescription()
{
    //Exposes the object description
    return pDescription;
}

//------------------------------------------Function Separator --------------------------------------------------------
void EquationofMotion::setDescription(string descIn)
{
    pDescription = descIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
void EquationofMotion::setDebugData(double freqIn, std::complex<double> solnIn, bool coeffIn)
{
    debugFreq = freqIn;
    debugSoln = solnIn;
    debugCoeffOnly = coeffIn;
}

//==========================================Section Separator =========================================================
//Protected Members

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::setFormula()
{
    //This is just a simple definition of the setFormula function to keep the compiler happy.
    //Eventually, this definition will be overriden by the correct definition of the child
    //class.
    complex<double> output(0,0);

    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::Kronecker(int var1, int var2, bool anti)
{
    complex<double> out(0,0);

    //Implementation of the Kronecker delta function.
    if (var1 == var2)
        out.real(1.0);
    else
        out.real(0.0);

    //Check if using reverse Kronecker delta.
    if (anti)
    {
        if (out.real() == 1.0)
            out.real(0.0);
        else
            out.real(1.0);
    }

    //write output
    return out;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::Ddt(int var, int ord, int bodIn)
{
    //set initial value for bodIn
    if (bodIn == -1)
        bodIn = pBod;

    //Calculate time differential.
    complex<double> out(1.0,0);

//    try
//    {
//        //First check if calculating actual values
//        if (!(pParentModel->CoefficientOnly()))
//        {
//            //Not calculating coefficients.  Get actual values.
//            complex<double> freq(0,0);
//            complex<double> imagI(0,1.0);

//            //get wave frequency.
//            freq = pParentModel->getFreq();

//            //Calculate refDerivative
//            for (int i = 0; i <= ord; i++)
//            {
//                out = out * freq * imagI;
//            }

//            out = out * pParentModel->listData(bodIn).getSolution().at(var,1);
//        }
//    }
//    catch(...)
//    {
        //If any errors.  Resort to debugging code.
        if (!(debugCoeffOnly))
        {
            //Not calculating coefficients.  Get actual values.
            complex<double> freq(0,0);
            complex<double> imagI(0,1.0);

            //get wave frequency.
            freq = debugFreq;

            //Calculate refDerivative
            for (int i = 0; i < ord; i++)
            {
                out = out * freq * imagI;
            }

            out = out * debugSoln;
        }
//    }

    //Write output
    return out;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::ForceActive_hydro()
{
    //Returns the active force object for the currently defined indices.
    complex<double> output(0,0);         //Temporary value for variable.
    complex<double> reverse(-1,0);         //Reverses sign of active force variable.

    //The coefficients in the active force matrix for hydrodynamics can only be one
    //set.  These are the reserved functions.

    //Check out of bounds for body.
    if ((curbody() <= pParentModel->listData().size() - 1) &&
            (pParentModel->listData().size() != 0))
    {
        //Out of bounds check for forces
        if (!(pParentModel->listData(curbody()).listForceActive_hydro().empty()))
        {
            //Add up for all forces
            for (pCurForce = 0; pCurForce < pParentModel->listData(curbody()).listForceActive_hydro().size(); pCurForce ++)
            {
                //Out of bounds check.  Ensure that the requested data index is not out of bounds.
                if (eqn() <= 5 )
                {
                    //Get value and add.
                    output +=
                            pParentModel->listData(curbody()).
                            listForceActive_hydro(force())->
                            getEquation(eqn());
                }
            }
            //Check if need to reverse sign of variable.
            if (pParentModel->getActiveOnly())
                output = output * reverse;
        }
    }

    //write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::ForceActive_user()
{
    //Returns the active force object for the currently defined indices.  User force.
    complex<double> output(0,0);    //Temporary value for variable.
    complex<double> reverse(-1,0);         //Reverses sign of active force variable.

    //The coefficients in the active force matrix must match the equations of motion, in the sequence that they
    //were entered.  Therefore, don't need the eqn object.  This defaults to the private variable.

    //Check out of bounds for body.
    if ((curbody() <= pParentModel->listData().size() - 1) &&
            (pParentModel->listData().size() != 0))
    {
        //Out of bounds check for forces
        if (!(pParentModel->listData(curbody()).listForceActive_user().empty()))
        {
            //Add up for all forces
            for (pCurForce = 0; pCurForce < pParentModel->listData(curbody()).listForceActive_user().size() ;
                 pCurForce++)
            {
                //Out of bounds check for equations
                if (eqn() <= pParentModel->listData(curbody()).
                        listForceActive_user(force())->
                        listEquation().size() - 1)
                {
                    //get value
                    output +=
                            pParentModel->listData(curbody()).
                            listForceActive_user(force())->
                            getEquation(eqn());
                }
            }
            //Check if need to reverse sign of variable.
            if (pParentModel->getActiveOnly())
                output = output * reverse;
        }
    }

    //write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::ForceReact_hydro(unsigned int ordIn, unsigned int varIn)
{
    //Returns the reactive force value for the indices specified by the input variables.
    complex<double> output(0,0);    //Temporary value for variable.

    //Check out of bounds for body.
    if ((curbody() <= pParentModel->listData().size() - 1) &&
            (pParentModel->listData().size() != 0))
    {
        //Add up all force objects.
        for (pCurForce = 0 ; pCurForce < pParentModel->listData(curbody()).listForceReact_hydro().size() ; pCurForce++)
        {
            //Out of bounds check for forces
            if ((force() <= pParentModel->listData(curbody()).
                    listForceReact_hydro().
                    size() - 1) &&
                    !(pParentModel->listData(curbody()).listForceReact_hydro().empty())
                    )
            {
                //Out of bounds check for refDerivatives
                if (ordIn <= pParentModel->
                        listData(curbody()).
                        listForceReact_hydro(force())->
                        listDerivative().
                        size() - 1)
                {
                    //Out of bounds check for variables
                    if (varIn <= pParentModel->listData(curbody()).
                            listForceReact_hydro(force())->
                            listDerivative(ordIn).
                            refIndexEquation(eqn()).
                            getCoefficientListSize())
                    {
                        //get value
                        output.real(
                                    output.real() +
                                    pParentModel->listData(curbody()).
                                    listForceReact_hydro(force())->
                                    listDerivative(ordIn).
                                    listEquation(eqn()).
                                    getCoefficient(varIn)
                                    );
                    }
                }
            }
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::ForceReact_user(unsigned int ordIn, unsigned int varIn)
{
    //Returns the reactive force value for the indices specified by the input variables.
    complex<double> output(0,0);    //Temporary value for variable.

    //Check out of bounds for body.
    if ((curbody() <= pParentModel->listData().size() - 1) &&
            (pParentModel->listData().size() != 0))
    {
        //Add up all force objects.
        for (pCurForce = 0 ; pCurForce < pParentModel->listData(curbody()).listForceReact_user().size() ; pCurForce++)
        {
            //Out of bounds check for forces
            if ((force() <= pParentModel->listData(curbody()).
                    listForceReact_user().
                    size() - 1) &&
                    !(pParentModel->listData(curbody()).listForceReact_user().empty())
                    )
            {
                //Out of bounds check for refDerivatives
                if (ordIn <= pParentModel->
                        listData(curbody()).
                        listForceReact_user(force())->
                        listDerivative().
                        size() - 1)
                {
                    //Out of bounds check for variables
                    if (varIn <= pParentModel->listData(curbody()).
                            listForceReact_user(force())->
                            listDerivative(ordIn).
                            refIndexEquation(eqn()).
                            getCoefficientListSize())
                    {
                        //get value
                        output.real(
                                    output.real() +
                                    pParentModel->listData(curbody()).
                                    listForceReact_user(force())->
                                    listDerivative(ordIn).
                                    listEquation(eqn()).
                                    getCoefficient(varIn)
                                    );
                    }
                }
            }
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::ForceCross_hydro(unsigned int bodIn, unsigned int ordIn, unsigned int varIn)
{
    //Returns the reactive force value for the indices specified by the input variables.
    complex<double> output(0,0);    //Temporary value for variable.

    //Check out of bounds for bodIn
    if ((bodIn <= pParentModel->listData().size() - 1) &&
            (pParentModel->listData().size() - 1 >= 1) &&
            (pParentModel->listData().size() != 0))
    {
        //Add up for all force objects.
        for (pCurForce = 0 ; pCurForce < pParentModel->listData(curbody()).
             listForceCross_hydro().
             size() ; pCurForce++)
        {
            //Out of bounds check for forces
            if ((force() <= pParentModel->listData(curbody()).
                    listForceCross_hydro().
                    size() - 1) &&
                    !(pParentModel->listData(curbody()).listForceCross_hydro().empty())
                    )
            {
                //Out of bounds check for refDerivatives
                if (ordIn <= pParentModel->listData(curbody()).
                        listForceCross_hydro(force())->
                        listDerivative().
                        size() - 1)
                {
                    //Out of bounds check for variables
                    if (varIn <= pParentModel->listData(curbody()).
                            listForceCross_hydro(force())->
                            listDerivative(ordIn).
                            refIndexEquation(eqn()).
                            getCoefficientListSize())
                    {
                        //Check that bodIn matches the body specified by the cross body force.
                        if (bodIn == pParentModel->listCompCrossBod_hydro(pCurForce))
                        {    Body test1 = pParentModel->listData(curbody());
                            complex<double> test2;
                            test2.real(
                                        test1.MassMatrix()(eqn(),varIn)
                                        );
                        //get value
                        output.real(
                                    output.real() +
                                    pParentModel->listData(curbody()).
                                    listForceCross_hydro(force())->
                                    listDerivative(ordIn).
                                    listEquation(eqn()).
                                    getCoefficient(varIn)
                                    );
                        }
                    }
                }
            }
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::ForceCross_user(unsigned int bodIn, unsigned int ordIn, unsigned int varIn)
{
    //Returns the reactive force value for the indices specified by the input variables.
    complex<double> output(0,0);    //Temporary value for variable.

    //Check out of bounds for bodIn
    if ((bodIn <= pParentModel->listData().size() - 1) &&
            (pParentModel->listData().size() - 1 >= 1) &&
            (pParentModel->listData().size() != 0))
    {
        //Add up for all force objects.
        for (pCurForce = 0 ; pCurForce < pParentModel->listData(curbody()).
             listForceCross_user().
             size() ; pCurForce++)
        {
            //Out of bounds check for forces
            if ((force() <= pParentModel->listData(curbody()).
                    listForceCross_user().
                    size() - 1) &&
                    !(pParentModel->listData(curbody()).listForceCross_user().empty())
                    )
            {
                //Out of bounds check for refDerivatives
                if (ordIn <= pParentModel->listData(curbody()).
                        listForceCross_user(force())->
                        listDerivative().
                        size() - 1)
                {
                    //Out of bounds check for variables
                    if (varIn <= pParentModel->listData(curbody()).
                            listForceCross_user(force())->
                            listDerivative(ordIn).
                            refIndexEquation(eqn()).
                            getCoefficientListSize())
                    {
                        //Check that bodIn matches the body specified by the cross body force.
                        if (bodIn == pParentModel->listCompCrossBod_user(pCurForce))
                        {    Body test1 = pParentModel->listData(curbody());
                            complex<double> test2;
                            test2.real(
                                        test1.MassMatrix()(eqn(),varIn)
                                        );
                        //get value
                        output.real(
                                    output.real() +
                                    pParentModel->listData(curbody()).
                                    listForceCross_user(force())->
                                    listDerivative(ordIn).
                                    listEquation(eqn()).
                                    getCoefficient(varIn)
                                    );
                        }
                    }
                }
            }
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
complex<double> EquationofMotion::ForceMass(int varIn)
{
    //Returns the mass object.
    complex<double> output;    //Temporary value for variable.

    //Check out of bounds for body.
    if ((curbody() <= pParentModel->listData().size() - 1) &&
            (pParentModel->listData().size() != 0))
    {
        //Check out of bounds for equation
        if (eqn() <= pParentModel->listData(curbody()).MassMatrix().n_rows)
        {
            //No need to check for out of bounds error.  Object creates enough entries by default.
            //No need to add multiple entries.  There can only be one mass object for each body.
            output.real(
                        pParentModel->listData(curbody()).MassMatrix()(eqn(), varIn)
                        );
            output.imag(0);
        }
    }

    //Write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
int EquationofMotion::var()
{
    //Returns the index integer for iteration on variable.
    return pCurVar;
}

//------------------------------------------Function Separator --------------------------------------------------------
unsigned int EquationofMotion::eqn()
{
    /*Returns the index integer for iteration on equation number.
     *Includes error handling for out-of-bounds checking.
     *
     *The equation function also includes additional provisions to check the Data Index of each equation.
     *The Data index is the solution to a problem:  We want the matrices and data storage to be as few entries
     *as possible.  This is to minimize memory usage.  The presence of hydrodynamic forces provides that
     *there will always be _data_ for at least six forces, which are reserved as the first six entries of any
     *equation vector.
     *
     *However, the equations of motion may not use those indices.  And the use forces may use other higher indices.
     *To allievate this problem, each Equation object, and each Equation of Motion object has a property of DataIndex.
     *The eqn() function searches through the DataIndices and matches these items, instead of matching the index
     *of the equation's position in the vector.  If the DataIndex property is not defined, then the position
     *in the vector is used as the index.
     */

    //Get index of equation of motion.
    if (pDataIndex < 0)
    {
        //Data index not defined.
        //Use private index.
        return pPrivateIndex;
    }
    else
    {
        return pDataIndex;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int EquationofMotion::ord()
{
    //Returns the index integer for iteration on order of derviative.
    //Includes error handling for out-of-bounds checking.
    return pCurOrd;
}

//------------------------------------------Function Separator --------------------------------------------------------
int EquationofMotion::body()
{
    //Returns the integer for the body from the list of bodies.  All includes out-of-bounds error limits.
    return pBod;
}

//------------------------------------------Function Separator --------------------------------------------------------
int EquationofMotion::curbody()
{
    //Returns integer for the current body.  Also includes checks for out-of-bounds error limits.
    return pParentModel->getBody();
}

//------------------------------------------Function Separator --------------------------------------------------------
int EquationofMotion::maxbody()
{
    //Returns the maximum index of body variable.
    return pParentModel->listData().size() - 1;
}

//------------------------------------------Function Separator --------------------------------------------------------
int EquationofMotion::maxord()
{
    //Returns the maximum index of order of refDerivative variable.
    //This will be the maximum order for any of the forces.

    //Only need to check the user forces because the hydro forces can never contain data beyond second order.
    int maxme = 2;          //Initialize tracking variable with second order.
    int temp;               //Retrieves value of order of refDerivative.

    //Check through Reactive forces.
    for (unsigned int i = 0; i < pParentModel->listData(curbody()).listForceReact_user().size(); i++)
    {
        //Get maximum order
        temp = pParentModel->listData(curbody()).listForceReact_user(i)->getMaxOrd();

        //Compare values.
        if (temp > maxme)
            maxme = temp;
    }

    //Check through Cross-body forces
    for (unsigned int i = 0; i < pParentModel->listData(curbody()).listForceCross_user().size(); i++)
    {
        //Get maximum order
        temp = pParentModel->listData(curbody()).listForceCross_user(i)->getMaxOrd();

        //Compare values.
        if (temp > maxme)
            maxme = temp;
    }

    //Write output
    return maxme;
}

//------------------------------------------Function Separator --------------------------------------------------------
int EquationofMotion::maxvar()
{
    //Returns the maximum index of variables.
//    return pParentModel->listEquation().size() - 1;
    return 1;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Sum(std::string FuncName, std::string index, int from, int to)
{
    //Create variable for output
    std::complex<double> output(0,0);        //Output variable

    //Select which index to Sum over.
    //Sum for variable
    //-----------------------------------
    if ((index.compare("var") == 0) ||
            (index.compare("v") == 0) ||
            (index.compare("V") == 0) ||
            (index.compare("Var") == 0) ||
            (index.compare("VAR") == 0))
    {
        //Check for summation limits
        if (from == undefArg)
        {
            //Get limit
            from = 0;
        }

        if (to == undefArg)
        {
            //Get limit
            to = maxvar();
        }

        //Sum for variable count.
        for (pCurVar = from ; pCurVar <= to; pCurVar++)
        {
            output = output + FunctionFind(FuncName);
        }
        //Return counter to max limit
        pCurVar = to;
    }

    //Sum for Derivative Order
    //-----------------------------------
    else if ((index.compare("ord") == 0) ||
             (index.compare("o") == 0) ||
             (index.compare("O") == 0) ||
             (index.compare("Ord") == 0) ||
             (index.compare("ORD") == 0))
    {
        //Check for summation limits
        if (from == undefArg)
        {
            //Get limit
            from = 0;
        }

        if (to == undefArg)
        {
            //Get limit
            to = maxord();
        }

        //Sum for order of refDerivative.
        for (pCurOrd = from ; pCurOrd <= to; pCurOrd++)
        {
            output = output + FunctionFind(FuncName);
        }
        //Return counter to max limit
        pCurOrd = to;
    }

    //Sum for Body
    //-----------------------------------
    else if ( (index.compare("bod") == 0) ||
              (index.compare("b") == 0) ||
              (index.compare("B") == 0) ||
              (index.compare("Bod") == 0) ||
              (index.compare("BOD") == 0) ||
              (index.compare("body") == 0) ||
              (index.compare("Body") == 0))
    {
        //Check for summation limits
        if (from == undefArg)
        {
            //Get limit
            from = 0;
        }

        if (to == undefArg)
        {
            //Get limit
            to = maxbody();
        }

        //Sum for bodies
        for (pBod = from ; pBod <= to; pBod++)
        {
            output = output + FunctionFind(FuncName);
        }
        //Return counter to max limit
        pBod = to;
    }
    //write output
    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Sum(std::complex<double> (*force)(void), int from, int to)
{
    //Create variable for output
    std::complex<double> output(0,0);        //Output variable

    //Sum value
    for (int i = from; i <= to; i++)
    {
        output = output + force();
    }

    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Sum(std::complex<double> force, int from, int to)
{
    //Create variable for output
    std::complex<double> output(0,0);        //Output variable

    //Sum constant value
    for (int i = from; i <= to; i++)
    {
        output = output + force;
    }

    return output;
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func1()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func2()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func3()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func4()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func5()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func6()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func7()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func8()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func9()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func10()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func11()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func12()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func13()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func14()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func15()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func16()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func17()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func18()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func19()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func20()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func21()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func22()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func23()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func24()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func25()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func26()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func27()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func28()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func29()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func30()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func31()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func32()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func33()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func34()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func35()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func36()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func37()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func38()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func39()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func40()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func41()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func42()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func43()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func44()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func45()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func46()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func47()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func48()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func49()
{
    return complex<double>(0,0);
}

//------------------------------------------Function Separator --------------------------------------------------------
std::complex<double> EquationofMotion::Func50()
{
    return complex<double>(0,0);
}

//==========================================Section Separator =========================================================
//Private Members

//------------------------------------------Function Separator --------------------------------------------------------
unsigned int EquationofMotion::force()
{
    //Returns integer for the current force.  Also includes checks for out-of-bounds error limits.
    int output;
    output = pCurForce;

    return output;
}

//------------------------------------------Function Separator ----------------------------------------------------
void EquationofMotion::ConstructorCommon(MotionModel *modelIn)
{
    //Assign the calling body to private variable.
    pParentModel = modelIn;

    //Create default value for data index.
    pDataIndex = -1;

    //Set the private index as a backup to an unset Data index value.
    pPrivateIndex = pParentModel->listEquation().size() - 1;

    //Set starting values for all items.
    pBod = 0;
    pCurForce = 0;
    pCurOrd = 0;
    pCurEqn = 0;
    pCurVar = 0;
}

//------------------------------------------Function Separator ----------------------------------------------------
std::complex<double> EquationofMotion::FunctionFind(std::string FuncName)
{
    //Evaluates functions based on function name.
    if ( (FuncName.compare("Func1()") == 0) ||
         (FuncName.compare("func1()") == 0) ||
         (FuncName.compare("Func1") == 0) ||
         (FuncName.compare("func1") == 0))
    {
        return Func1();
    }

    else if ( (FuncName.compare("Func2()") == 0) ||
         (FuncName.compare("func2()") == 0) ||
         (FuncName.compare("Func2") == 0) ||
         (FuncName.compare("func2") == 0))
    {
        return Func2();
    }

    else if ( (FuncName.compare("Func3()") == 0) ||
         (FuncName.compare("func3()") == 0) ||
         (FuncName.compare("Func3") == 0) ||
         (FuncName.compare("func3") == 0))
    {
        return Func3();
    }

    else if ( (FuncName.compare("Func4()") == 0) ||
         (FuncName.compare("func4()") == 0) ||
         (FuncName.compare("Func4") == 0) ||
         (FuncName.compare("func4") == 0))
    {
        return Func4();
    }

    else if ( (FuncName.compare("Func5()") == 0) ||
         (FuncName.compare("func5()") == 0) ||
         (FuncName.compare("Func5") == 0) ||
         (FuncName.compare("func5") == 0))
    {
        return Func5();
    }

    else if ( (FuncName.compare("Func6()") == 0) ||
         (FuncName.compare("func6()") == 0) ||
         (FuncName.compare("Func6") == 0) ||
         (FuncName.compare("func6") == 0))
    {
        return Func6();
    }

    else if ( (FuncName.compare("Func7()") == 0) ||
         (FuncName.compare("func7()") == 0) ||
         (FuncName.compare("Func7") == 0) ||
         (FuncName.compare("func7") == 0))
    {
        return Func7();
    }

    else if ( (FuncName.compare("Func8()") == 0) ||
         (FuncName.compare("func8()") == 0) ||
         (FuncName.compare("Func8") == 0) ||
         (FuncName.compare("func8") == 0))
    {
        return Func8();
    }

    else if ( (FuncName.compare("Func9()") == 0) ||
         (FuncName.compare("func9()") == 0) ||
         (FuncName.compare("Func9") == 0) ||
         (FuncName.compare("func9") == 0))
    {
        return Func9();
    }

    else if ( (FuncName.compare("Func10()") == 0) ||
         (FuncName.compare("func10()") == 0) ||
         (FuncName.compare("Func10") == 0) ||
         (FuncName.compare("func10") == 0))
    {
        return Func10();
    }

    else if ( (FuncName.compare("Func11()") == 0) ||
         (FuncName.compare("func11()") == 0) ||
         (FuncName.compare("Func11") == 0) ||
         (FuncName.compare("func11") == 0))
    {
        return Func11();
    }

    else if ( (FuncName.compare("Func12()") == 0) ||
         (FuncName.compare("func12()") == 0) ||
         (FuncName.compare("Func12") == 0) ||
         (FuncName.compare("func12") == 0))
    {
        return Func12();
    }

    else if ( (FuncName.compare("Func13()") == 0) ||
         (FuncName.compare("func13()") == 0) ||
         (FuncName.compare("Func13") == 0) ||
         (FuncName.compare("func13") == 0))
    {
        return Func13();
    }

    else if ( (FuncName.compare("Func14()") == 0) ||
         (FuncName.compare("func14()") == 0) ||
         (FuncName.compare("Func14") == 0) ||
         (FuncName.compare("func14") == 0))
    {
        return Func14();
    }

    else if ( (FuncName.compare("Func15()") == 0) ||
         (FuncName.compare("func15()") == 0) ||
         (FuncName.compare("Func15") == 0) ||
         (FuncName.compare("func15") == 0))
    {
        return Func15();
    }

    else if ( (FuncName.compare("Func16()") == 0) ||
         (FuncName.compare("func16()") == 0) ||
         (FuncName.compare("Func16") == 0) ||
         (FuncName.compare("func16") == 0))
    {
        return Func16();
    }

    else if ( (FuncName.compare("Func17()") == 0) ||
         (FuncName.compare("func17()") == 0) ||
         (FuncName.compare("Func17") == 0) ||
         (FuncName.compare("func17") == 0))
    {
        return Func17();
    }

    else if ( (FuncName.compare("Func18()") == 0) ||
         (FuncName.compare("func18()") == 0) ||
         (FuncName.compare("Func18") == 0) ||
         (FuncName.compare("func18") == 0))
    {
        return Func18();
    }

    else if ( (FuncName.compare("Func19()") == 0) ||
         (FuncName.compare("func19()") == 0) ||
         (FuncName.compare("Func19") == 0) ||
         (FuncName.compare("func19") == 0))
    {
        return Func19();
    }

    else if ( (FuncName.compare("Func20()") == 0) ||
         (FuncName.compare("func20()") == 0) ||
         (FuncName.compare("Func20") == 0) ||
         (FuncName.compare("func20") == 0))
    {
        return Func20();
    }

    else if ( (FuncName.compare("Func21()") == 0) ||
         (FuncName.compare("func21()") == 0) ||
         (FuncName.compare("Func21") == 0) ||
         (FuncName.compare("func21") == 0))
    {
        return Func21();
    }

    else if ( (FuncName.compare("Func22()") == 0) ||
         (FuncName.compare("func22()") == 0) ||
         (FuncName.compare("Func22") == 0) ||
         (FuncName.compare("func22") == 0))
    {
        return Func22();
    }

    else if ( (FuncName.compare("Func23()") == 0) ||
         (FuncName.compare("func23()") == 0) ||
         (FuncName.compare("Func23") == 0) ||
         (FuncName.compare("func23") == 0))
    {
        return Func23();
    }

    else if ( (FuncName.compare("Func24()") == 0) ||
         (FuncName.compare("func24()") == 0) ||
         (FuncName.compare("Func24") == 0) ||
         (FuncName.compare("func24") == 0))
    {
        return Func24();
    }

    else if ( (FuncName.compare("Func25()") == 0) ||
         (FuncName.compare("func25()") == 0) ||
         (FuncName.compare("Func25") == 0) ||
         (FuncName.compare("func25") == 0))
    {
        return Func25();
    }

    else if ( (FuncName.compare("Func26()") == 0) ||
         (FuncName.compare("func26()") == 0) ||
         (FuncName.compare("Func26") == 0) ||
         (FuncName.compare("func26") == 0))
    {
        return Func26();
    }

    else if ( (FuncName.compare("Func27()") == 0) ||
         (FuncName.compare("func27()") == 0) ||
         (FuncName.compare("Func27") == 0) ||
         (FuncName.compare("func27") == 0))
    {
        return Func27();
    }

    else if ( (FuncName.compare("Func28()") == 0) ||
         (FuncName.compare("func28()") == 0) ||
         (FuncName.compare("Func28") == 0) ||
         (FuncName.compare("func28") == 0))
    {
        return Func28();
    }

    else if ( (FuncName.compare("Func29()") == 0) ||
         (FuncName.compare("func29()") == 0) ||
         (FuncName.compare("Func29") == 0) ||
         (FuncName.compare("func29") == 0))
    {
        return Func29();
    }

    else if ( (FuncName.compare("Func30()") == 0) ||
         (FuncName.compare("func30()") == 0) ||
         (FuncName.compare("Func30") == 0) ||
         (FuncName.compare("func30") == 0))
    {
        return Func30();
    }

    else if ( (FuncName.compare("Func31()") == 0) ||
         (FuncName.compare("func31()") == 0) ||
         (FuncName.compare("Func31") == 0) ||
         (FuncName.compare("func31") == 0))
    {
        return Func31();
    }

    else if ( (FuncName.compare("Func32()") == 0) ||
         (FuncName.compare("func32()") == 0) ||
         (FuncName.compare("Func32") == 0) ||
         (FuncName.compare("func32") == 0))
    {
        return Func32();
    }

    else if ( (FuncName.compare("Func33()") == 0) ||
         (FuncName.compare("func33()") == 0) ||
         (FuncName.compare("Func33") == 0) ||
         (FuncName.compare("func33") == 0))
    {
        return Func33();
    }

    else if ( (FuncName.compare("Func34()") == 0) ||
         (FuncName.compare("func34()") == 0) ||
         (FuncName.compare("Func34") == 0) ||
         (FuncName.compare("func34") == 0))
    {
        return Func34();
    }

    else if ( (FuncName.compare("Func35()") == 0) ||
         (FuncName.compare("func35()") == 0) ||
         (FuncName.compare("Func35") == 0) ||
         (FuncName.compare("func35") == 0))
    {
        return Func35();
    }

    else if ( (FuncName.compare("Func36()") == 0) ||
         (FuncName.compare("func36()") == 0) ||
         (FuncName.compare("Func36") == 0) ||
         (FuncName.compare("func36") == 0))
    {
        return Func36();
    }

    else if ( (FuncName.compare("Func37()") == 0) ||
         (FuncName.compare("func37()") == 0) ||
         (FuncName.compare("Func37") == 0) ||
         (FuncName.compare("func37") == 0))
    {
        return Func37();
    }

    else if ( (FuncName.compare("Func38()") == 0) ||
         (FuncName.compare("func38()") == 0) ||
         (FuncName.compare("Func38") == 0) ||
         (FuncName.compare("func38") == 0))
    {
        return Func38();
    }

    else if ( (FuncName.compare("Func39()") == 0) ||
         (FuncName.compare("func39()") == 0) ||
         (FuncName.compare("Func39") == 0) ||
         (FuncName.compare("func39") == 0))
    {
        return Func39();
    }

    else if ( (FuncName.compare("Func40()") == 0) ||
         (FuncName.compare("func40()") == 0) ||
         (FuncName.compare("Func40") == 0) ||
         (FuncName.compare("func40") == 0))
    {
        return Func40();
    }

    else if ( (FuncName.compare("Func41()") == 0) ||
         (FuncName.compare("func41()") == 0) ||
         (FuncName.compare("Func41") == 0) ||
         (FuncName.compare("func41") == 0))
    {
        return Func41();
    }

    else if ( (FuncName.compare("Func42()") == 0) ||
         (FuncName.compare("func42()") == 0) ||
         (FuncName.compare("Func42") == 0) ||
         (FuncName.compare("func42") == 0))
    {
        return Func42();
    }

    else if ( (FuncName.compare("Func43()") == 0) ||
         (FuncName.compare("func43()") == 0) ||
         (FuncName.compare("Func43") == 0) ||
         (FuncName.compare("func43") == 0))
    {
        return Func43();
    }

    else if ( (FuncName.compare("Func44()") == 0) ||
         (FuncName.compare("func44()") == 0) ||
         (FuncName.compare("Func44") == 0) ||
         (FuncName.compare("func44") == 0))
    {
        return Func44();
    }

    else if ( (FuncName.compare("Func45()") == 0) ||
         (FuncName.compare("func45()") == 0) ||
         (FuncName.compare("Func45") == 0) ||
         (FuncName.compare("func45") == 0))
    {
        return Func45();
    }

    else if ( (FuncName.compare("Func46()") == 0) ||
         (FuncName.compare("func46()") == 0) ||
         (FuncName.compare("Func46") == 0) ||
         (FuncName.compare("func46") == 0))
    {
        return Func46();
    }

    else if ( (FuncName.compare("Func47()") == 0) ||
         (FuncName.compare("func47()") == 0) ||
         (FuncName.compare("Func47") == 0) ||
         (FuncName.compare("func47") == 0))
    {
        return Func47();
    }

    else if ( (FuncName.compare("Func48()") == 0) ||
         (FuncName.compare("func48()") == 0) ||
         (FuncName.compare("Func48") == 0) ||
         (FuncName.compare("func48") == 0))
    {
        return Func48();
    }

    else if ( (FuncName.compare("Func49()") == 0) ||
         (FuncName.compare("func49()") == 0) ||
         (FuncName.compare("Func49") == 0) ||
         (FuncName.compare("func49") == 0))
    {
        return Func49();
    }

    else if ( (FuncName.compare("Func50()") == 0) ||
         (FuncName.compare("func50()") == 0) ||
         (FuncName.compare("Func50") == 0) ||
         (FuncName.compare("func50") == 0))
    {
        return Func50();
    }

    else
    {
        return complex<double>(0,0);
    }
}
