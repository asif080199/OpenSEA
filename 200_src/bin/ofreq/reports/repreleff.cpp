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
#include "repreleff.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static variables
string repRelEff::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repRelEff::KEY_POWERREP = "repPower";    /**< Name of the power report to reference. */
string repRelEff::KEY_VARS = "variables";        /**< variable indices used to sum for efficiency calculation. */
string repRelEff::KEY_LENGTH = "length";      /**< The user input for length used to calculate relative efficiency. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repRelEff::repRelEff()
{
    //Assign a default value to the name.
    this->setName("Body Relative Efficiency");

    //Clear the list of variables
    plistAdded = false;
    plistVars.clear();
    ptPower = NULL;
}

//------------------------------------------Function Separator --------------------------------------------------------
repRelEff::repRelEff(ReportManager *ptIn) : repRelEff()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void repRelEff::calcReport(int freqInd)
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

        //Check to make sure the body length was defined.
        if (pLength == 0)
            throw std::runtime_error(string("No body length defined.  This must be included for relative efficiency.  Use the keyword ")
                                     + KEY_LENGTH + string("."));

        //Create a new report.
        ofreq::repCaptureWidth repCapWidth(this->ptManager);

        //Feed the body to the report.
        repCapWidth.setBody(this->ptBody);

        //Feed constants to the report
        for (unsigned int i = 0; i < plistConst_Key.size(); i++)
        {
            if (plistConst_Key.at(i) != KEY_LENGTH)
            {
                repCapWidth.addConst(
                            plistConst_Key.at(i),
                            plistConst_Val.at(i)
                            );
            }
        }

        //Calculate the report.
        repCapWidth.calcReport(freqInd);

        //Write results to results list.
        plistData.push_back(Data(freqInd));

        plistData.back().addValue(
                    repCapWidth.listData(0).listValueDouble(0)
                    / pLength
                    );
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repRelEff::calcRAO(int freqInd)
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
std::string repRelEff::getClass()
{
    return "repRelativeEfficiency";
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string repRelEff::getFileName()
{
    return "efficiency_relative.out";
}


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions


//------------------------------------------Function Separator --------------------------------------------------------
void repRelEff::defineConst(int constIndex)
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

        if (plistConst_Key.at(constIndex) == KEY_LENGTH)
        {
            //Process body length
            pLength = plistConst_Val.at(constIndex).listValueDouble(0);
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
void repRelEff::addVarNames()
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
