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

#include "syshydro.h"

using namespace osea::ofreq;
using namespace std;

//==========================================Section Separator =========================================================
//Static initialization

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
sysHydro::sysHydro()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
sysHydro::~sysHydro()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector< std::vector< hydroData > > &sysHydro::listHydroData()
{
    return plistHydroData;
}

//------------------------------------------Function Separator --------------------------------------------------------
hydroData &sysHydro::listHydroData(int ampInd, int dirInd)
{
    return plistHydroData.at(ampInd).at(dirInd);
}

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::addHydroData(int ampInd, ofreq::hydroData dataIn)
{
    if (ampInd < 0)
    {
        //no amp index specified.  Add new index.
        plistHydroData.push_back(vector< hydroData >(dataIn));
    }
    else
    {
        //amp index specified.  Add new wave direction at given amplitude index.
        plistHydroData.at(ampInd).push_back(dataIn);
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string sysHydro::getHydroBodyName()
{
    return pHydroBodyName;
}

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::setHydroBodyName(std::string nameIn)
{
    pHydroBodyName = nameIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
ForceActive sysHydro::getForceActive(double waveAmp, double waveFreq)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
ForceReact sysHydro::getForceReact(double waveAmp, double waveFreq)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
ForceCross sysHydro::getForceCross(double waveAmp, double waveFreq, std::string linkedBodName)
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::setWaveDir(double dirIn)
{
    pWaveDir = dirIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
double sysHydro::getWaveDir()
{
    return pWaveDir;
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void sysHydro::buildWaveSubset()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
T sysHydro::waveScale(double ampIn, T* Data1, T* Data2)
{

}
