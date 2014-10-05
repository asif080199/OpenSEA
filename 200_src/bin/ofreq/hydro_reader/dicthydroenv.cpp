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
#include "dicthydroenv.h"
#include "hydroreader.h"

using namespace std;
using namespace osea;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants
string dictHydroEnv::OBJECT_PHYSICS = "physics"; /**< String designation for the physics object. */

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name static constants
string dictHydroEnv::KEY_DEPTH = "depth"; /**< String designation for the depth keyword. */
string dictHydroEnv::KEY_DENSITY = "density"; /**< String designation for the density keyword. */
string dictHydroEnv::KEY_GRAVITY = "gravity"; /**< String designation for the gravity keyword. */
string dictHydroEnv::KEY_AMP = "waveamp"; /**< String designation for the wave amplitude keyword. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroEnv::dictHydroEnv()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroEnv::dictHydroEnv(osea::HydroReader *parent)
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
int dictHydroEnv::defineKey(std::string keyIn, std::vector<std::string> valIn)
{
    //Keyword for depth key.
    //----------------------------------------------
    if (keyIn == KEY_DEPTH)
    {
        //convert value
        double out = atof(valIn[0].c_str());

        //Write to hydroreader
        pParent->pDepth = out;

        return 0;
    }

    //Keyword for density
    //----------------------------------------------
    if (keyIn == KEY_DENSITY)
    {
        //Convert value
        double out = atof(valIn[0].c_str());

        //Write to hydroreader
        pParent->pDensity = out;

        return 0;
    }

    //Keyword for gravity
    //----------------------------------------------
    if (keyIn == KEY_GRAVITY)
    {
        for (unsigned int i = 0; i < valIn.size(); i++)
        {
            //Convert value
            double out = atof(valIn[i].c_str());

            //Write to hydroreader
            pParent->pGravity.push_back(out);
        }

        return 0;
    }

    //Keyword for wave amplitude
    //----------------------------------------------
    if (keyIn == KEY_AMP)
    {
        //Convert value
        double out = atof(valIn[0].c_str());

        //Write to hydroreader
        pParent->pWaveAmp = out;
    }

    //----------------------------------------------
    else
    {
        logStd.Notify();
        logErr.Write(string("File:  environment.out \n Unknown key found.  Key:  ") + keyIn);
        return 1;
    }

}

//------------------------------------------Function Separator --------------------------------------------------------
int dictHydroEnv::defineClass(std::string nameIn)
{
    //Keyword for physics object
    //----------------------------------------------
    if (nameIn == OBJECT_PHYSICS)
    {
        //No action necessary.
        return 0;
    }

    //----------------------------------------------
    else
    {
        logStd.Notify();
        logErr.Write(string("File:  environment.out \n Unknown object name found.  Object:  ") + nameIn);
        return 1;
    }
}

//==========================================Section Separator =========================================================
//Private Functions
