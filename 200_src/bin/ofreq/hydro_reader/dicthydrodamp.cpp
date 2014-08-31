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
#include "dicthydrodamp.h"
#include "hydroreader.h"

using namespace std;
using namespace osea;
using namespace arma;

//==========================================Section Separator =========================================================
//Static Initialization

int dictHydroDamp::ORD = 1; /**< The order of derivative that is associated with all data from this file. */

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name constants

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroDamp::dictHydroDamp()
{
    pBodOn = false;
    pHydroBod = NULL;
}

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroDamp::dictHydroDamp(osea::dictHydroDamp *parent)
{
    pParent = parent;
    pBodOn = false;
    pHydroBod = NULL;
}

//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots Functions


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
