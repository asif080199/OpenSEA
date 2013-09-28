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

#include "dictforces.h"

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictForces::dictForces()
{
    //------------------------------------------Function Separator ----------------------------------------------------
    //Class Names
    OBJECT_FORCE_ACTIVE = "force_active"; /**< Keyword for force_active definition.*/
    OBJECT_FORCE_REACT = "force_reactive"; /**< Keyword for force_react class definition.*/
    OBJECT_FORCE_CROSS = "force_crossbody"; /**< Keyword for force_cross class definition.*/
    OBJECT_DERIVATIVE = "derivative"; /**< Keyword for derivative class definition. */
    OBJECT_EQUATION = "equation"; /**< Keyword for equation designation.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    //Keyword Names
    KEY_NAME = "name"; /**< Keyword for the name the user assigns for a force.*/
    KEY_COEFF = "coeff"; /**< Keyword for coefficient designation.*/
    KEY_NUMBER = "number"; /**< Keyword for equation number designation. */
    KEY_ORDER = "order"; /**< Keyword for order of derivative designation. */
    KEY_FORCE = "force"; /**< Keyword for force coefficients designation.*/
    KEY_EQUATION = "equation"; /**< Keyword for equation designation.*/
}

//==========================================Section Separator =========================================================
//Signals

//==========================================Section Separator =========================================================
//Public Slots

//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int dictForces::defineKey(string keyIn, vector<string> valIn)
{
    switch (keyIn)
    {
    case KEY_NAME:
        //Set the name of the force object.
        if (pForceType == 1)
        {
            //Active force type.
            ptSystem->refForceActive_user(pForceIndex).setForceName(valIn);
        }
        else if (pForceType == 2)
        {
            //Reactive force type
            ptSystem->refForceReact_user(pForceIndex).setForceName(valIn);
        }
        else if (pForceType == 3)
        {
            //Cross-body force type
            ptSystem->refForceCross_user(pForceIndex).setForceName(valIn);
        }
        //Report success
        return 0;
        break;

    case KEY_COEFF:
        //Set the coefficient of the current force equation specified.
        //Only applicable to ForceActive objects.
        if (pForceType == 1)
        {
            //convert data type and input.
            ptSystem->refForceActive_user(pForceIndex).setCoeff(
                        convertComplex(valIn[0].c_str()), pEqn);
            //Report success
            return 0;
        }
        else
        {
            //Report error
            return 2;
        }
        break;

    case KEY_NUMBER:
        //Set the equation number
        pEqn = atoi(valIn[0].c_str());
        //Report success
        return 0;
        break;

    case KEY_ORDER:
        //Set the derivative number
        pOrd = atoi(valIn[0].c_str());
        //Report success
        return 0;
        break;

    case KEY_FORCE:
        //Create a vector of force coefficients
        vector<double> coeffIn;
        for (unsigned int i = 0; i < valIn.size(); i++)
        {
            coeffIn.push_back(atof(valIn[i].c_str()));
        }
        //Add the vector to the appropriate derivative and equation.
        if (pForceType == 2)
        {
            //Reactive force object
            ptSystem->refForceReact_user(pForceIndex).refDerivative(pOrd).refEquation(pEqn).listCoefficients().clear();
            ptSystem->refForceReact_user(pForceIndex).refDerivative(pOrd).refEquation(pEqn).listCoefficients() = coeffIn;
            //Report success
            return 0;
        }
        else if (pForceType == 3)
        {
            //Cross-body force object
            ptSystem->refForceCross_user(pForceIndex).refDerivative(pOrd).refEquation(pEqn).listCoefficients().clear();
            ptSystem->refForceCross_user(pForceIndex).refDerivative(pOrd).refEquation(pEqn).listCoefficients() = coeffIn;
            //Report success
            return 0;
        }
        else
        {
            //Incorrect force object.  return error.
            return 2;
        }
        break;

    case KEY_EQUATION:
        //Check that the current active force object is an active force.
        //Otherwise, the key is invalid.
        if (pForceType == 1)
        {
            //Convert value
            int indexIn;
            indexIn = atoi(valIn[0].c_str());

            //Active force type.  Set the equation index.
            setEquation(indexIn);

            //Report success
            return 0;
        }
        else
        {
            //Invalid use of key.  Report error.
            return 2;
        }
        break;

    default:
        //Key not found.  Report error
        return 1;
        break;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictForces::defineClass(string nameIn)
{
    switch (nameIn)
    {
    case OBJECT_FORCE_ACTIVE:
        //Active force key.
        //Set variable for force type.
        pForceType = 1;

        //Set index of new force
        setForceIndex();

        //Report back success
        return 0;
        break;

    case OBJECT_FORCE_REACT:
        //Reactive force key.
        //Set variable for force type.
        pForceType = 2;

        //Set index of new force
        setForceIndex();

        //Report back success
        return 0;
        break;

    case OBJECT_FORCE_CROSS:
        //Cross-body force key.
        //Set variable for force type.
        pForceType = 3;

        //Set index of new force
        setForceIndex();

        //Report back success
        return 0;
        break;

    case OBJECT_DERIVATIVE:
        //Add new derivative
        //Do nothing.  Not necessary to create new derivative object.
        //That will be done automatically when using the refDerivative function to
        //assign properties of new derivative.

        //Set variable of latest derivative as the last derivative, until reassigned.
        setDerivative();

        //Report back success
        return 0;
        break;

    case OBJECT_EQUATION:
        //Add new equation.
        //Do nothing.  Not necessary to create new equation object.
        //That will be done automatically when using the refEquation function to
        //assign properties of new derivative.

        //Set variable of latest equation, until reassigned.
        setEquation();

        //Report back success
        return 0;
        break;

    default:
        //Case for no keyword found.
        //return an error code.
        return 1;
    }
}

//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
 void dictForces::setForceIndex(int forceIn = -1)
 {
     if (forceIn < 0)
     {
         if (pForceType == 1)
         {
             //Active forces
             pForceIndex = ptSystem->listForceActive_user().size();
         }
         else if (pForceType == 2)
         {
             //Reactive Forces
             pForceIndex = ptSystem->listForceReact_user().size();

         }
         else if (pForceType == 3)
         {
             //Cross-body forces
            pForceIndex = ptSystem->listForceCross_user().size();
         }
     }
     else
     {
         pForceIndex = forceIn;
     }
 }

//------------------------------------------Function Separator --------------------------------------------------------
void dictForces::setEquation(int eqIn)
{
    if (eqIn < 0)
    {
        if (pForceType == 1)
        {
            //Active force type.
            //Get latest equation
            pEqn = ptSystem->listForceActive_user()[pForceIndex].listEquations().size();
        }
        else if (pForceType == 2)
        {
            //Reactive force type.
            pEqn = ptSystem->listForceReact_user()[pForceIndex].refDerivative(pOrd).listEquations().size();
        }
        else if (pForceType == 3)
        {
            //Cross-body force type.
            pEqn = ptSystem->listForceReact_user()[pForceIndex].refDerivative(pOrd).listEquations().size();
        }
    }
    else
    {
        pEqn = eqIn;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void dictForces::setDerivative(int ordIn)
{
    if (ordIn < 0)
    {
        if (pForceType == 1)
        {
            //Active force type.  Do nothing.  No derivatives.
            pOrd = -1;
        }
        else if (pForceType == 2)
        {
            //Reactive force type.
            if (ordIn < 0)
            pOrd = ptSystem->listForceReact_user()[pForceIndex].getMaxOrd() + 1;
        }
        else if (pForceType == 3)
        {
            //Cross-body force type
            pOrd = ptSystem->listForceCross_user()[pForceIndex].getMaxOrd() + 1;
        }
    }
    else
    {
        pOrd = ordIn;
    }
}
