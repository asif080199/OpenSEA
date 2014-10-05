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
#include "dicthydromass.h"
#include "hydroreader.h"

using namespace std;
using namespace osea;
using namespace arma;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants


//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name static constants


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroMass::dictHydroMass()
{
    pBodOn = false;
    pHydroBod = NULL;
    ORD = 2;
}

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroMass::dictHydroMass(osea::HydroReader *parent)
{
    pBodOn = false;
    pHydroBod = NULL;
    pParent = parent;
    ORD = 2;
}

//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots Functions


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------


//------------------------------------------Function Separator --------------------------------------------------------


//==========================================Section Separator =========================================================
//Private Functions
