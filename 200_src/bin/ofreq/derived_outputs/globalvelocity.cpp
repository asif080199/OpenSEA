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

#include "globalvelocity.h"

//------------------------------------------Function Separator --------------------------------------------------------
GlobalVelocity::GlobalVelocity() 
	: GlobalSolution()
{
    VELOCITY_NAME = "Global Velocity"; /**< Used for reference in file output. */
    GLOBAL_VELOCITY_DERIVATIVE = 1;  /**< Specifies first order of the derivative. (velocity) */

    pName = VELOCITY_NAME;
	orderDerivative = GLOBAL_VELOCITY_DERIVATIVE;
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalVelocity::~GlobalVelocity()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void GlobalVelocity::setDerivative(int ord)
{
    GlobalSolution::setDerivative(GLOBAL_VELOCITY_DERIVATIVE);
}
