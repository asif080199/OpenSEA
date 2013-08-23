/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date              Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *May 15 2013       Shane Honanie       Initially created
 *Aug 13, 2013      Nicholas Barczak    Updated to reflect new motion model and motion solver paradigm.
 *
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



//######################################### Class Separator ###########################################################
#ifndef GLOBALACCELERATION_H
#define GLOBALACCELERATION_H
#include "globalsolution.h"
#include "outputderived.h"

using namespace std;

const string ACCELERATION_NAME = "Global Acceleration"; /**< Used for reference in file output. */
const int GLOBAL_ACCELERATION_DERIVATIVE = 2;  /**< Specifies 2nd order of the derivative. (acceleration) */

//######################################### Class Separator ###########################################################
//prototype class declarations
class OutputDerived;    //Include prototype declaration to allow proper class definition.
class Solution;

//######################################### Class Separator ###########################################################
/**
 * This class represents the Global Acceleraion Solution.
 */

class GlobalAcceleration : public GlobalSolution
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	GlobalAcceleration(); /**< This default constructor creates a Global Acceleration object. */

    //------------------------------------------Function Separator ----------------------------------------------------
	~GlobalAcceleration(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    void setDerivative(int ord);

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
};
#endif
