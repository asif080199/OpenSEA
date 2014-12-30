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
#include "repstaticbodyaccel.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static variables
string repStaticBodyAccel::KEY_BODY = "body";    /**< Name of the body to associate with.*/


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repStaticBodyAccel::repStaticBodyAccel()
{
    //Assign a default value for the name.
    this->setName("Body Static Acceleration)");
}

//------------------------------------------Function Separator --------------------------------------------------------
repStaticBodyAccel::repStaticBodyAccel(ofreq::ReportManager *ptIn) : repStaticBodyAccel()
{
    //Assign report manager.
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
string repStaticBodyAccel::getClass()
{
    return "repStaticBodyAcceleration";
}

//------------------------------------------Function Separator --------------------------------------------------------
string repStaticBodyAccel::getFileName()
{
    return "accelstaticbod.out";
}

//------------------------------------------Function Separator --------------------------------------------------------
void repStaticBodyAccel::calcReport(int freqInd)
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

        std::vector<complexDouble> output(3);    //Output list.
        std::vector<complexDouble> motion(6);     //List of translation motions.

        //Get base solution values.
        ofreq::Solution* input =
                &(ptSystem->listSolutionSet(this->getBodIndex())
                .refSolution(
                    ptSystem->getCurWaveDirInd(),
                    freqInd));

        //Create list of equation indices to match data index.
        std::vector<int> eqIndex(6);
        for (unsigned int i = 0; i < eqIndex.size(); i++)
        {
            //Assign pointer for motion model.
            ofreq::MotionModel *ptModel = &(ptBody->getMotionModel());
            //Search through equation indices.
            for (unsigned int j = 0; j < ptModel->listDataIndex().size(); j++)
            {
                if (ptModel->listDataIndex(j) == i)
                {
                    eqIndex.at(i) = j;
                    break;
                }
                else
                {
                    eqIndex.at(i) = -1;
                }
            }
        }

        //Retrieve translations and rotations from solution.
        for (unsigned int i = 0; i < motion.size(); i++)
        {
            if (eqIndex.at(i) == -1)
                //No data available for that index.
                motion.at(i) = complex<double>(0,0);
            else
                motion.at(i) = input->getSolnMat()(eqIndex.at(i),0);
        }

        //Calculate output list
        //Calculate X-axis static  acceleration.
        output.at(0) = polar(GRAVITY * sin(abs(motion.at(5)))
                             ,
                             arg(motion.at(5))
                           );

        //Calculate Y-axis static acceleration.
        output.at(1) = polar(GRAVITY * sin(abs(motion.at(4)))
                             ,
                             arg(motion.at(4))
                             );

        //Calculate Z-axis static acceleration.
        output.at(2) = polar(GRAVITY * cos(abs(motion.at(5))) * cos(abs(motion.at(5)))
                             ,
                             arg(motion.at(5)) + arg(motion.at(4))
                             );

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
void repStaticBodyAccel::calcRAO(int freqInd)
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
void repStaticBodyAccel::defineConst(int constIndex)
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
