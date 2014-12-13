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

//==========================================Section Separator =========================================================
//Inclusions and namespaces
#include "repglobacceleration.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;
using namespace arma;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repGlobAcceleration::repGlobAcceleration()
{
    //Assign a default value for the name.
    this->setName("Body Global Motion");

    //Add the order of derivative constant.
    plistConst_Key.push_back(
                KEY_ORDER);

    //create a new data object and add it to the constant key values.
    Data keyval;
    keyval.addString(
                itoa(2)
                );
    plistConst_Val.push_back(keyval);
}

//------------------------------------------Function Separator --------------------------------------------------------
repGlobAcceleration::repGlobAcceleration(ofreq::ReportManager *ptIn) : repGlobAcceleration::repGlobAcceleration()
{
    this->setManager(ptIn);
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
