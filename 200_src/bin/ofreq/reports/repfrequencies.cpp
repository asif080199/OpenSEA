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
#include "repfrequencies.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repFrequencies::repFrequencies()
{
    //Set the name for this report.
    this->setName("World Wave Frequencies");

    //Turn off RAO calculations.
    pCalcRAO = false;
}

//------------------------------------------Function Separator --------------------------------------------------------
repFrequencies::repFrequencies(ofreq::ReportManager *ptIn) : repFrequencies::repFrequencies()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
repFrequencies::~repFrequencies()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void repFrequencies::calcReport(int freqInd)
{
    //Add one data entry.
    plistData.push_back(Data(-1));

    //Iterate through each wave frequency from the system object.
    for (unsigned int i = 0; i < ptSystem->listWaveFrequencies().size(); i++)
    {
        try
        {
            //Add each wave frequency to the list of data.
            //Add value to the data object.
            plistData.back().addValue(
                        ptSystem->listWaveFrequencies(i)
                        );
        }
        catch(const std::exception &err)
        {
            //Error handler.
            logStd.Notify();
            logErr.Write(ID + string(err.what()));
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repFrequencies::calcRAO(int freqInd)
{
    //Do not calculate RAO.
}

//------------------------------------------Function Separator --------------------------------------------------------
string repFrequencies::getClass()
{
    return "repFrequencies";
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string repFrequencies::getFileName()
{
    return "frequencies.out";
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

