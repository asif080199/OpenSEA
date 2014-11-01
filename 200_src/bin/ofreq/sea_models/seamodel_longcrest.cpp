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
#include "seamodel_longcrest.h"

using namespace std;
using namespace osea;

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
SeaModel_LongCrest::SeaModel_LongCrest() : SeaModel()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
SeaModel_LongCrest::~SeaModel_LongCrest()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
double SeaModel_LongCrest::getWaveEnergy(double dirIn, int freqIndex)
{
    //Get the wave frequency.
    double freqIn = plistWaveFreq.at(freqIndex);

    //Calculate the result
    return getWaveEnergy(dirIn, freqIn);
}


//------------------------------------------Function Separator --------------------------------------------------------
double SeaModel_LongCrest::getWaveEnergy(double dirIn, double freqIn)
{
    double SpecEnergy;      //Calculated wave spectrum energy.

    //Calculate the wave spectrum energy, if the direction is correct.
    if (dirIn == plistWaveDir.at(0))
    {
        SpecEnergy = listWaveSpec(0)->getSpecEnergy(freqIn);
    }
    else
    {
        SpecEnergy = 0;
    }

    /*Do not need to check on wave scaling properties.  The inheritted functions automatically
     *ensure that any added wave spectrum has a wave scaling factor of 1 added by default.
     */

    //Scale and calculate wave spectrum energy.  Return result.
    return SpecEnergy * listWaveScale(0);
}

//------------------------------------------Function Separator --------------------------------------------------------
double SeaModel_LongCrest::getWaveAmp(double dirIn, int freqIndex)
{
    //First calculate the wave energy for the requested direction and frequency.
    double WaveEnergy = getWaveEnergy(dirIn, freqIndex);

    //Next get the frequency step for the defined frequency index.
    double stepsize = getFreqStep(freqIndex);

    //Finally, calculate the wave amplitude
    try
    {
        //Check for negative number.
        if (WaveEnergy < 0)
        {
            std::string msg;                    //Error message.

            msg = string("Interpolated wave energy was negative for the requested wave direction and") +
                  string("frequency.  Please check the sea model.\nWave Direction:  ");
                  std::to_string(dirIn) + string(" rad\n") + string("Wave Frequency:  ") +
                  std::to_string(plistWaveFreq.at(freqIndex)) + string(" rad/s");

            //Throw the error message.
            throw std::runtime_error(msg);
        }


        //Calculate wave amplitude
        return sqrt(2 * WaveEnergy * stepsize);
    }
    catch (const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));
        exit(1);
    }
}



//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions
