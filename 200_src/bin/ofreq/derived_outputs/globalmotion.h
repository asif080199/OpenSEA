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
#ifndef GLOBALMOTION_H
#define GLOBALMOTION_H
#include "globalsolution.h"

using namespace std;

//######################################### Class Separator ###########################################################
//Prototype class declarations
class OutputDerived;    //Include prototype declaration to allow proper class definition.

//######################################### Class Separator ###########################################################
/**
 * This class represents the Global Motion Solution.
 */

class GlobalMotion : public GlobalSolution
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	GlobalMotion(); /**< This default constructor creates a Global Motion object. */

    //------------------------------------------Function Separator ----------------------------------------------------
	~GlobalMotion(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    void setDerivative(int ord);

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    static string MOTION_NAME; /**< Used for reference in file output. */
    static int GLOBAL_MOTION_DERIVATIVE;  /**< Specifies zero order of the derivative. (motion) */

};
#endif
