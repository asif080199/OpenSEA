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

#include "forcereact.h"

using namespace std;
using namespace osea::ofreq;

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact::ForceReact() : Force()
{}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact::~ForceReact()
{}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::setCurDerivative(int newOrder)
{
	currentDerivative = newOrder;
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::setCurEquationNum(int newEquationNum)
{
	currentEquation = newEquationNum -1;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<Derivative> &ForceReact::listDerivative()
{
    return pDerivative;
}

//------------------------------------------Function Separator --------------------------------------------------------
Derivative ForceReact::getDerivative(unsigned int num)
{
    //Return the derivative object specified.

    //Also checks to ensure that the request will not return an out of bounds error.  If requested index
    //is greater than current vector size, the vector size is increased.
    if ((num > pDerivative.size() - 1) || (pDerivative.size() == 0))
    {
        //resize vector
        pDerivative.resize(num + 1);
    }

    return pDerivative[num];
}

//------------------------------------------Function Separator --------------------------------------------------------
Derivative &ForceReact::listDerivative(unsigned int num)
{
    //Return the derivative object specified.
    //Value is a pointer to the derivative.

    //Also checks to ensure that the request will not return an out of bounds error.  If requested index
    //is greater than current vector size, the vector size is increased.
    if ((num > pDerivative.size() - 1) || (pDerivative.size() == 0))
    {
        //resize vector
        pDerivative.resize(num + 1);
    }

    return pDerivative[num];
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::addDerivative()
{
    //Adds a new derivative object.
    pDerivative.push_back(Derivative());
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceReact::addDerivative(Derivative derivIn, unsigned int ordIn)
{
    //Adds a new derivative object

    //Check if the order is high enough.
    if ((ordIn > pDerivative.size() - 1) || (pDerivative.size() == 0))
    {
        //Resize vector
        pDerivative.resize(ordIn + 1);
    }

    //Add in the new derivative.  This will overwrite any pre-existing derivatives.
    pDerivative[ordIn] = derivIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
int ForceReact::getMaxOrd()
{
    return pDerivative.size() - 1;
}
