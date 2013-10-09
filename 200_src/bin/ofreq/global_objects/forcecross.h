/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date          Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *May 15, 2013	Shane Honanie       Initially created
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
#ifndef FORCECROSSBODY_H
#define FORCECROSSBODY_H
#include "forcereact.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//######################################### Class Separator ###########################################################
/**
 * This class holds data for a cross body force.  A cross body force is very closely related to a reactive force object
 * and they behave almost exactly the same.  The only difference is that the force within a reactive force object
 * which is owned by Body A, they are dependant on the motions of that Body.  But for a cross-body force object:
 * The forces from the Cross-body force owned by Body A are dependant on the motions of another body.
 */

class ForceCross: public ForceReact
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    ForceCross(); /**< This default constructor creates a Body object. */

    //------------------------------------------Function Separator ----------------------------------------------------
    ~ForceCross(); /**< The default destructor, nothing happens here. */

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:

};
#endif

