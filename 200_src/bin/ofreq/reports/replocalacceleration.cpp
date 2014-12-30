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
#include "replocalacceleration.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;
using namespace arma;

//==========================================Section Separator =========================================================
//Pubic Functions

//------------------------------------------Function Separator --------------------------------------------------------
repLocalAcceleration::repLocalAcceleration()
{
    //Assign a default value to the name.
    this->setName("Body Local Acceleration");

    //Add the order of derivative constant.
    plistConst_Key.push_back(
                KEY_ORDER);

    //create a new data object and add it to the constant key values.
    Data keyval;
    keyval.addValue(2);
    plistConst_Val.push_back(keyval);
}

//------------------------------------------Function Separator --------------------------------------------------------
repLocalAcceleration::repLocalAcceleration(ofreq::ReportManager *ptIn) : repLocalAcceleration()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
string repLocalAcceleration::getClass()
{
    return "repLocalAcceleration";
}

//------------------------------------------Function Separator --------------------------------------------------------
string repLocalAcceleration::getFileName()
{
    return "acclocal.out";
}


//==========================================Section Separator =========================================================
//Protected Functions



//==========================================Section Separator =========================================================
//Private Functions
