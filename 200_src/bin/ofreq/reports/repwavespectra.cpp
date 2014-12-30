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
#include "repwavespectra.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Variables
string repWaveSpectra::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repWaveSpectra::KEY_WAVE = "output";  /**< String of whether to calculate wave amplitude or spectral energy.*/

string repWaveSpectra::VAL_AMP = "wave_amp";     /**< String that declares should calculate wave amplitude. */
string repWaveSpectra::VAL_ENERGY = "energy";  /**< String that declares should calculate wave energy. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repWaveSpectra::repWaveSpectra()
{
    //Assign a default value for the name.
    this->setName("Body Wave Spectra");
}

//------------------------------------------Function Separator --------------------------------------------------------
repWaveSpectra::repWaveSpectra(ofreq::ReportManager *ptIn) : repWaveSpectra()
{
    //Assign the report manager.
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void repWaveSpectra::calcReport(int freqInd)
{
    try
    {
        if (pConstCalc)
        {
            //Define constants
            for (unsigned int i = 0; i < plistConst_Key.size(); i++)
            {
                this->defineConst(i);
            }

            //Turn off const calculation
            pConstCalc = false;

            //Check if the wave amplitude was defined.
            bool amp_found = false;
            for (unsigned int i = 0; i < plistConst_Key.size(); i++)
            {
                if (plistConst_Key.at(i) == KEY_WAVE)
                {
                    amp_found = true;
                    break;
                }
            }

            //If not defined, add it.
            if (!amp_found)
            {
                if (pCalcAmp)
                {
                    //Case for calculating amplitude.
                    this->addConst(
                                KEY_WAVE,
                                VAL_AMP
                                );
                }
                else
                {
                    //Case for calculating spectral energy.
                    this->addConst(
                                KEY_WAVE,
                                VAL_ENERGY
                                );
                }
            }

        }

        double output;      //The calculated output value.

        //Get wave energy, or wave amplitude
        if (pCalcAmp)
        {
            //Get wave amplitude.
            output = ptSystem->refActiveSeaModel().getWaveAmp(ptSystem->getCurWaveDirInd(), freqInd);
        }
        else
        {
            //Get wave energy.
            output = ptSystem->refActiveSeaModel().getWaveEnergy(ptSystem->getCurWaveDirInd(),freqInd);
        }

        //Write result to results list.
        plistData.push_back(Data(freqInd));

        //Add values to Data object.
        plistData.back().addValue(output);
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repWaveSpectra::calcRAO(int freqInd)
{
    try
    {
        double output;      //The calculated output value.

        //Calculate output for the given Body.
        //Velocity term first.
        output = 2 * ptSystem->listWaveFrequencies(freqInd)
                 * 0 /*Normally add in Body forward velocity at this point.  Still need to implement it.*/
                 * cos(ptBody->getHeading() - ptSystem->getCurWaveDir());

        //Denominator first.
        output = GRAVITY - output;

        //Numerator term finally.
        output = GRAVITY / output;

        //Write result to results list.
        plistData.push_back(Data(freqInd));

        //Add values to Data object.
        plistData.back().addValue(output);
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
string repWaveSpectra::getClass()
{
    return "repWaveSpectra";
}

//------------------------------------------Function Separator --------------------------------------------------------
string repWaveSpectra::getFileName()
{
    return "wavespectra.out";
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void repWaveSpectra::defineConst(int constIndex)
{
    try
    {
        if (plistConst_Key.at(constIndex) == KEY_BODY)
        {
            //Process body.
            //Get the correct body and assign it.
            for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
            {
                if (ptSystem->listBody(i).getBodyName() == plistConst_Val.at(constIndex).listString(0))
                {
                    ptBody = &(ptSystem->listBody(i));
                    break;
                }
            }
        }

        else if (plistConst_Key.at(constIndex) == KEY_WAVE)
        {
            if (plistConst_Val.at(constIndex).listString(0) == VAL_AMP)
            {
                //Calculate wave amplitude.
                pCalcAmp = true;
            }
            else
                pCalcAmp = false;
        }

        else
        {
            //throw an error.
            throw std::runtime_error(string("Unknown key specified.  Offending key:  ") + plistConst_Key.at(constIndex));
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}
