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

#include "forceactive.h"

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive::ForceActive(): Force()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive::~ForceActive()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void ForceActive::setCoeff(complex<double> coeffIn, unsigned int index)
{
    //Check if list is large enough for specified index.
    if (index > pCoefficients.size() - 1)
    {
        //Resize list
        pCoefficients.resize(index + 1);
    }

    //Assign coefficient
    pCoefficients[index] = coeffIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<complexDouble> &ForceActive::listCoefficients()
{
    return pCoefficients;
}

//------------------------------------------Function Separator --------------------------------------------------------
complexDouble &ForceActive::listCoefficients(unsigned int index)
{
    return pCoefficients[index];
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<complexDouble> &ForceActive::listEquation()
{
    return this->listCoefficients();
}

//------------------------------------------Function Separator --------------------------------------------------------
complexDouble &ForceActive::listEquation(unsigned int index)
{
    return this->listCoefficients(index);
}

//------------------------------------------Function Separator --------------------------------------------------------
complexDouble ForceActive::getEquation(int number)
{
    //Return the input coefficient for the equation specified.
    return pCoefficients[number];
}

