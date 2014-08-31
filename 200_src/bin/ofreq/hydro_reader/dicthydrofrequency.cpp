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
#include "dicthydrofrequency.h"
#include "hydroreader.h"

using namespace std;
using namespace osea;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants


//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name static constants
string dictHydroFrequency::KEY_FREQUENCY = "frequency";   /**< Keyword for frequency list designation. */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroFrequency::dictHydroFrequency()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
dictHydroFrequency::dictHydroFrequency(osea::HydroReader *parent)
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
int dictHydroFrequency::defineKey(std::string keyIn, std::vector<std::string> valIn)
{
    //Keyword for frequency list.
    //----------------------------------------------
    if (keyIn == KEY_FREQUENCY)
    {
        //Add the wave frequencies to the hydroreader.
        for (unsigned int i = 0; i < valIn.size(); i ++)
        {
            //Convert output
            double out = atof(valIn[i].c_str());

            //Add to the list of wave frequencies
            pParent->plistWaveFreq.push_back(out);
        }

        return 0;
    }

    //----------------------------------------------
    else
    {
        logStd.Notify();
        logErr.Write(string("File:  frequencies.out \n Unknown key found.  Key:  ") + keyIn);
        return 1;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictHydroFrequency::defineClass(std::string nameIn)
{
    //No class definitions within this file.
    //throw an error
    logStd.Notify();
    logErr.Write("File:  frequencies.out \n No objects are currently supported for this input file.");
    return 1;
}

//==========================================Section Separator =========================================================
//Private Functions

