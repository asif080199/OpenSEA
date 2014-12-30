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
#include "repdesignaccel.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;
using namespace arma;

//==========================================Section Separator =========================================================
//Static variables
string repDesignAccel::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repDesignAccel::KEY_LOCATION = "location";    /**< Location for the local acceleration. */
string repDesignAccel::KEY_ORDER = "ord";   /**< Order of derivative. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repDesignAccel::repDesignAccel()
{
    //Assign default value for the name
    this->setName("Local Design Acceleration");

    //Add the order of derivative constant.
    plistConst_Key.push_back(
                KEY_ORDER);

    //create a new data object and add it to the constant key values.
    Data keyval;
    keyval.addValue(2);
    plistConst_Val.push_back(keyval);
}

//------------------------------------------Function Separator --------------------------------------------------------
repDesignAccel::repDesignAccel(ofreq::ReportManager *ptIn) : repDesignAccel()
{
    //Set manager.
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
string repDesignAccel::getClass()
{
    return "repDesignAccel";
}

//------------------------------------------Function Separator --------------------------------------------------------
string repDesignAccel::getFileName()
{
    return "design_accel.out";
}

//------------------------------------------Function Separator --------------------------------------------------------
void repDesignAccel::calcReport(int freqInd)
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
        }

        //Create an internal report for static acceleration and local acceleration
        repStaticBodyAccel repStatic(this->ptManager);
        repLocalAcceleration repAccel(this->ptManager);

        //Set body for both reports.
        repStatic.setBody(this->ptBody);
        repAccel.setBody(this->ptBody);

        //Set location for local acceleration.
        for (unsigned int i = 0; i < plistConst_Key.size(); i++)
        {
            if (plistConst_Key.at(i) == KEY_LOCATION)
            {
                repAccel.addConst(
                            plistConst_Key.at(i)
                            ,
                            plistConst_Val.at(i).listValue());

                break;
            }
        }

        //Calculate static accel
        repStatic.calcReport(freqInd);

        //Calculate local accel
        repAccel.calcReport(freqInd);

        //Get the last entry and combine.
        vector<double> output(3);

        //Get X-axis
        output.at(0) = repAccel.listData().back().listValueDouble(0)
                       + repStatic.listData().back().listValueDouble(0);

        //Get Y-axis
        output.at(1) = repAccel.listData().back().listValueDouble(1)
                       - repStatic.listData().back().listValueDouble(1);

        //Get Z-axis
        output.at(2) = repAccel.listData().back().listValueDouble(2)
                       + repStatic.listData().back().listValueDouble(2);

        //Write results back to results list.
        plistData.push_back(
                    Data(freqInd));

        for (unsigned int i = 0; i < output.size(); i++)
        {
            plistData.back().addValue(
                        output.at(i));
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
void repDesignAccel::calcRAO(int freqInd)
{
    try
    {
        if (pCalcRAO)
        {
            complex<double> amp;

            //Get amplitude.
            amp.real(ptSystem->refActiveSeaModel().getWaveAmp(ptSystem->getCurWaveDirInd(), freqInd));

            //Write result to results list.
            plistRAO.push_back(Data(freqInd));

            //Add values to Data object.
            for (unsigned int i = 0; i < plistData.at(freqInd).listValue().size(); i++)
            {
                plistRAO.back().addValue(plistData.at(freqInd).listValue(i)
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


//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void repDesignAccel::defineConst(int constIndex)
{
    //Define constants for report.

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

        else if (plistConst_Key.at(constIndex) == KEY_ORDER)
        {
            //Process order of derivative.
            pOrd = plistConst_Val.at(constIndex).listValueInt(0);
        }

        else if (plistConst_Key.at(constIndex) == KEY_LOCATION)
        {
            //Process location coordinates
            pMomArm.push_back(
                        std::complex<double>(
                        plistConst_Val.at(constIndex).listValueDouble(0)
                            ,
                            0)
                        );     //X-axis coordinate

            pMomArm.push_back(
                        std::complex<double>(
                        plistConst_Val.at(constIndex).listValueDouble(1)
                            ,
                            0)
                        );     //Y-axis coordinate

            pMomArm.push_back(
                        std::complex<double>(
                        plistConst_Val.at(constIndex).listValueDouble(2)
                            ,
                            0)
                        );     //Z-axis coordinate
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
