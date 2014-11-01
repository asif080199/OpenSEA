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
#include "dicthydrodirection.h"
#include "hydroreader.h"

using namespace std;
using namespace osea;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants
string dictHydroDirection::OBJECT_DIRECTION = "direction";    /**< Object name for wave directions object. */

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name static constants
string dictHydroDirection::KEY_DIRECTION = "direction"; /**< Key for wave directions specification. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroDirection::dictHydroDirection()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroDirection::dictHydroDirection(osea::HydroReader *parent)
{
    pParent = parent;
}


//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots Functions


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int dictHydroDirection::defineKey(std::string keyIn, std::vector<std::string> valIn)
{
    //Wave directions keyword
    //-----------------------------------------------
    if (keyIn == KEY_DIRECTION)
    {
        //Add the wave directions to the hydro reader.
        for (unsigned int i = 0; i < valIn.size(); i++)
        {
            //Convert output
            double out = atof(valIn.at(i).c_str());

            //Add to the list of wave directions
            pParent->plistWaveDir.push_back(out);
        }

        return 0;
    }

    //-----------------------------------------------
    else
    {
        logStd.Notify();
        logErr.Write(string("File:  directions.out \n Unknown key found.  Key:  ") + keyIn);
        return 1;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictHydroDirection::defineClass(std::string nameIn)
{
    //Wave directions object.
    //-----------------------------------------------
    if (nameIn == OBJECT_DIRECTION)
    {
        //No action necessary.
        return 0;
    }

    //-----------------------------------------------
    else
    {
        //throw an error
        logStd.Notify();
        logErr.Write("File:  directions.out \n No objects are currently supported for this input file.");
        return 1;
    }
}

//==========================================Section Separator =========================================================
//Private Functions

