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
#include "repdirections.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repDirections::repDirections()
{
    //Set the name for this report.
    this->setName("World Wave Directions");

    //Set the standard for calculating the RAO
    pCalcRAO = false;
}

//------------------------------------------Function Separator --------------------------------------------------------
repDirections::repDirections(ReportManager *ptIn) : repDirections::repDirections()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
repDirections::~repDirections()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void repDirections::calcReport(int freqInd)
{
    //Create one set of output data.
    plistData.push_back(Data(-1));

    //Iterate through each wave direction from the system object.
    for (unsigned int i = 0; i < ptSystem->listWaveDirections().size(); i++)
    {
        try
        {
            //Add value to the data object.
            plistData.back().addValue(
                        ptSystem->listWaveDirections(i)
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
void repDirections::calcRAO(int freqInd)
{
    //Do nothing.  Do not calculate RAO.
}

//------------------------------------------Function Separator --------------------------------------------------------
string repDirections::getClass()
{
    return "repDirections";
}

//------------------------------------------Function Separator --------------------------------------------------------
string repDirections::getFileName()
{
    return "directions.out";
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
