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

}

//------------------------------------------Function Separator --------------------------------------------------------
int dictForces::defineClass(string nameIn)
{
    switch (nameIn)
    {
    case KEY_FORCE_ACTIVE:
        //Active force key.  Must create active force object.
        ptSystem->addforceActive_user();

        //Set variable for force type.
        pForceType = 1;

        //Get index of last force created.
        pForceIndex = ptSystem->listforceActive_user().size() - 1;

        //Report back success
        return 0;
        break;

    case KEY_FORCE_REACT:
        //Reactive force key.  Must create reactive force object.
        ptSystem->addforceReact_user();

        //Set variable for force type.
        pForceType = 2;

        //Get index of last force created.
        pForceIndex = ptSystem->listforceReact_user().size() - 1;

        //Report back success
        return 0;
        break;

    case KEY_FORCE_CROSS:
        //Cross-body force key.  Must create cross-body force object.
        ptSystem->addforceReact_user();

        //Set variable for force type.
        pForceType = 3;

        //Get index of last force created.
        pForceIndex = ptSystem->listforceCross_user().size() - 1;

        //Report back success
        return 0;
        break;

    case KEY_DERIVATIVE:
        //Add new derivative
        //Do nothing.  Not necessary to create new derivative object.
        //That will be done automatically when using the refDerivative function to
        //assign properties of new derivative.

        //Set variable of latest derivative as the last derivative, until reassigned.
        setDerivative();

        //Report back success
        return 0;
        break;

    case KEY_EQUATION:
        //Add new equation.
        //Do nothing.  Not necessary to create new equation object.
        //That will be done automatically when using the refEquation function to
        //assign properties of new derivative.

        //Set variable of latest equation, until reassigned.


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
void dictForces::setEquation()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void dictForces::setDerivative()
{
    if (pForceType == 1)
    {
        //Active force type.  Do nothing.  No derivatives.
        pOrd = -1;
    }
    else if (pForceType == 2)
    {
        //Reactive force type.
        pOrd = ptSystem->listforceReact_user()[pForceIndex].getMaxOrd();
    }
    else if (pForceType == 3)
    {
        //Cross-body force type
        pOrd = ptSystem->listforceCross_user()[pForceIndex].getMaxOrd();
    }
}
