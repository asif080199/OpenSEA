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

#include "dictcontrol.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Names
string dictControl::KEY_SYSTEM = "system"; /**< Keyword for system object.*/

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword Value Pairs
string dictControl::KEY_ANALYSIS = "analysis"; /**< Keyword for analysis specification */
string dictControl::KEY_FREQUENCY = "frequency"; /**< Keyword for frequency specification */
string dictControl::KEY_DIRECTION = "direction"; /**< Keyword for direction list specification. */
string dictControl::KEY_WAVEMODEL = "wavemodel"; /**< Keyword for wave model specification. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictControl::dictControl()
    : Dictionary()
{
}

//==========================================Section Separator =========================================================
//Signals


//==========================================Section Separator =========================================================
//Slots


//==========================================Section Separator =========================================================
//Protected Functions
//------------------------------------------Function Separator --------------------------------------------------------
int dictControl::defineKey(string keyIn, vector<string> valIn)
{
    if (keyIn == KEY_ANALYSIS)
    {
        //Set the analysis type.
        ptSystem->setAnalysisType(valIn[0]);

        //return success
        return 0;
    }

    else if (keyIn == KEY_FREQUENCY)
    {
        //Set the wave frequencies.
        //First convert them from strings to doubles.
        vector<double> freqIn;

        for (unsigned int i = 0; i < valIn.size(); i++)
        {
            //Convert value and add it to the list.
            freqIn.push_back(atof(valIn[i].c_str()));
        }

        //Pass the information to the System object.
        ptSystem->setWaveFrequencies(freqIn);

        //return success
        return 0;
    }

    else if (keyIn == KEY_DIRECTION)
    {
        //Set the wave directions.
        //First convert them from strings to doubles.
        vector<double> dirIn;

        for (unsigned int i = 0; i < valIn.size(); i++)
        {
            //Convert the value and add it to the list.
            dirIn.push_back(atof(valIn[i].c_str()));
        }

        //Pass the information to the System object.
        ptSystem->setWaveDirections(dirIn);

        //return success
        return 0;
    }

    else if (keyIn == KEY_WAVEMODEL)
    {
        //Set the wave spread model
        //Implement later

        //return success
        return 0;
    }

    else
    {
        //Word not found.  Return error code.
        return 1;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictControl::defineClass(string nameIn)
{
    if (nameIn == KEY_SYSTEM)
    {
        //Don't need to do anything for the system object.
        //this one is so fundamental to the program that it automatically gets created at program start.
        return 0;
    }

    else
    {
        //Word not found.  Return error code.
        return 1;
    }
}

//==========================================Section Separator =========================================================
//Private Functions
