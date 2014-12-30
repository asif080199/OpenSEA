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
#include "repcapturewidth.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static variables
string repCaptureWidth::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repCaptureWidth::KEY_POWERREP = "repPower";    /**< Name of the power report to reference. */
string repCaptureWidth::KEY_VARS = "variables";        /**< variable indices used to sum for efficiency calculation. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repCaptureWidth::repCaptureWidth()
{
    //Assign a default value to the name.
    this->setName("Body Capture Width");

    //Clear the list of variables
    plistAdded = false;
    plistVars.clear();
    ptPower = NULL;
}

//------------------------------------------Function Separator --------------------------------------------------------
repCaptureWidth::repCaptureWidth(ReportManager *ptIn) : repCaptureWidth()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void repCaptureWidth::calcReport(int freqInd)
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

            //Add list of equations
            if (!plistAdded)
                addVarNames();

            //Turn off const calculation
            pConstCalc = false;
        }

        //Create a new report.
        ofreq::repBodPower repPower(this->ptManager);

        //Feed the body to the report.
        repPower.setBody(this->ptBody);

        //Feed constants to report.
        for (unsigned int i = 0; i < ptPower->listConstKey().size(); i++)
        {
            repPower.addConst(
                        ptPower->listConstKey(i),
                        ptPower->listConstVal(i)
                        );
        }

        //Calculate the report.
        repPower.calcReport(freqInd);

        //Output vector
        std::vector<double> Pow;

        //Get result.
        for (unsigned int i = 0; i < repPower.listData(0).listValue().size(); i++)
        {
            Pow.push_back(
                        repPower.listData(0).listValueDouble(i)
                        );
        }

        double totPower = 0;

        if (plistVars.size() == 0)
        {
            //Add up the total power, for all variables.
            for (unsigned int i = 0; i < Pow.size(); i++)
                totPower += Pow.at(i);
        }
        else
        {
            //Add up the total power, for only the selected variables.
            for (unsigned int i = 0; i < plistVars.size(); i++)
            {
                totPower += Pow.at(plistVars.at(i));
            }
        }

        //Calculate wave energy available.
        double wavePower = calcWavePow(freqInd);

        //Write results to results list.
        plistData.push_back(Data(freqInd));

        plistData.back().addValue(
                    totPower / wavePower);
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repCaptureWidth::calcRAO(int freqInd)
{
    try
    {
        if (pCalcRAO)
        {
            //Get amplitude.
            double amp = ptSystem->refActiveSeaModel().getWaveAmp(ptSystem->getCurWaveDirInd(), freqInd);

            //Write result to results list.
            plistRAO.push_back(Data(freqInd));

            //Add values to Data object.
            for (unsigned int i = 0; i < plistData.at(freqInd).listValue().size(); i++)
            {
                plistRAO.back().addValue(plistData.at(freqInd).listValueDouble(i)
                                              / amp);
            }
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
string repCaptureWidth::getClass()
{
    //Name of the class
    return "repCaptureWidth";
}

//------------------------------------------Function Separator --------------------------------------------------------
string repCaptureWidth::getFileName()
{
    //Filename for the class.
    return "capture_width.out";
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void repCaptureWidth::defineConst(int constIndex)
{
    //Define constants for report.

    try
    {
        if (plistConst_Key.at(constIndex) == KEY_BODY)
        {
            //Process body.
            plistAdded = false;
            plistVars.clear();
            ptPower = NULL;

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

        if (plistConst_Key.at(constIndex) == KEY_POWERREP)
        {
            //Process power report.
            //Get the correct power report and assign it.
            for (unsigned int i = 0; i < ptManager->listReport().size(); i++)
            {
                if (ptManager->listReportPt(i)->getName() == plistConst_Val.at(constIndex).listString(0))
                {
                    ptPower = ptManager->listReportPt(i);

                    //Add the list of variables
                    if ((!plistAdded)
                            && (plistVars.size() > 0))
                        addVarNames();

                    break;
                }
            }
        }

        if (plistConst_Key.at(constIndex) == KEY_VARS)
        {
            //Process list of variables.

            //Get reference to motion model.
            MotionModel *ptMotion = &(ptBody->getMotionModel());

            //Input is the data index.
            //Use motion model to get the actual index of the equation variable.
            for (unsigned int i = 0; i < plistConst_Val.at(constIndex).listValue().size(); i++)
            {
                for (unsigned int j = 0; j < ptMotion->listEquation().size(); j++)
                {
                    if (ptMotion->listEquation(j).refDataIndex() ==
                            plistConst_Val.at(constIndex).listValueInt(i) - 1)
                    {
                        plistVars.push_back(j);
                        break;
                    }
                }
            }

            //Add the list of variables.
            if ((!plistAdded)
                    && (ptPower))
                addVarNames();
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
double repCaptureWidth::calcWavePow(int freqInd)
{
    try
    {
        //Calculate power available per unit width of wave crest.

        //Get water density from the system
        double dens = ptSystem->listHydroManager(ptBody->getBodyName()).listHydroData(0,0).getDensity();

        //Get the wave amplitude from the system.
        double amp = ptSystem->refActiveSeaModel().getWaveAmp(ptSystem->getCurWaveDirInd(), freqInd);

        //Get the current wave frequency from the system.
        double freq = ptSystem->getCurFreq();

        //Calculate result and write out.
        return 0.25 * dens * pow(GRAVITY, 2) * pow(amp, 2) / freq;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repCaptureWidth::addVarNames()
{
    try
    {
        //Add the list of equations from the motion model, for user reference in output.
        plistConst_Key.push_back(string("variables_included "));
        //Get reference to motion model.
        MotionModel *ptMotion = &(ptBody->getMotionModel());
        plistConst_Val.push_back(Data());
        if (plistVars.size() == 0)
        {
            //Add all equations
            for (unsigned int i = 0; i < ptMotion->listEquation().size(); i++)
            {
                plistConst_Val.back().addString(
                            ptMotion->listEquation(i).getName()
                            );
            }
        }
        else
        {
            //Just add equations in the list of variables.
            for (unsigned int i = 0; i < plistVars.size(); i++)
            {
                plistConst_Val.back().addString(
                            ptMotion->listEquation(
                                plistVars.at(i)
                                ).getName()
                            );
            }
        }

        //Set the boolean to note that the list was already added.
        plistAdded = true;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}
