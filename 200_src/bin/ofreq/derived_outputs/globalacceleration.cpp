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

#include "globalacceleration.h"

//------------------------------------------Function Separator --------------------------------------------------------
GlobalAcceleration::GlobalAcceleration() 
	: GlobalSolution()
{
    ACCELERATION_NAME = "Global Acceleration"; /**< Used for reference in file output. */
    GLOBAL_ACCELERATION_DERIVATIVE = 2;  /**< Specifies 2nd order of the derivative. (acceleration) */

    pName = ACCELERATION_NAME;
	orderDerivative = GLOBAL_ACCELERATION_DERIVATIVE;
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalAcceleration::~GlobalAcceleration()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void GlobalAcceleration::setDerivative(int ord)
{
    GlobalSolution::setDerivative(GLOBAL_ACCELERATION_DERIVATIVE);
}
