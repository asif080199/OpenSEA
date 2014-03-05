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

#include "globalmotion.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static initializations
string GlobalMotion::MOTION_NAME = "Global Motion"; /**< Used for reference in file output. */
int GlobalMotion::GLOBAL_MOTION_DERIVATIVE = 0;  /**< Specifies zero order of the derivative. (motion) */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
GlobalMotion::GlobalMotion() 
	: GlobalSolution()
{
    pName = MOTION_NAME;
	orderDerivative = GLOBAL_MOTION_DERIVATIVE;
    pClassName = "GlobalMotion";
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalMotion::GlobalMotion(OutputsBody* input)
    : GlobalSolution(input)
{
    pName = MOTION_NAME;
    orderDerivative = GLOBAL_MOTION_DERIVATIVE;
    pClassName = "GlobalMotion";
}

//------------------------------------------Function Separator --------------------------------------------------------
GlobalMotion::~GlobalMotion()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
void GlobalMotion::setDerivative(int ord)
{
    GlobalSolution::setDerivative(GLOBAL_MOTION_DERIVATIVE);
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
