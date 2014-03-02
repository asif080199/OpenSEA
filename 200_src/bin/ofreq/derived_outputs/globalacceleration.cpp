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

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static variable initialization
string GlobalAcceleration::ACCELERATION_NAME = "Global Acceleration"; /**< Used for reference in file output. */
int GlobalAcceleration::GLOBAL_ACCELERATION_DERIVATIVE = 2;  /**< Specifies 2nd order of the derivative. (acceleration) */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
GlobalAcceleration::GlobalAcceleration() 
	: GlobalSolution()
{
    pName = ACCELERATION_NAME;
    orderDerivative = GLOBAL_ACCELERATION_DERIVATIVE;
    pClassName = "GlobalAcceleration";
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalAcceleration::GlobalAcceleration(OutputsBody* input)
    : GlobalSolution(input)
{
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

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
