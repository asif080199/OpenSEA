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
#include "replocalsolution.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;
using namespace arma;

//==========================================Section Separator =========================================================
//Static variables
string repLocalSolution::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repLocalSolution::KEY_ORDER = "order";   /**< Order of derivative to use for processing the output.*/
string repLocalSolution::KEY_LOCATION = "location";    /**< Coordinate of location of point, relative to Body coordinates. */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repLocalSolution::repLocalSolution()
{
    //Assign a default value to the name.
    this->setName("Body Local Solution");
}

//------------------------------------------Function Separator --------------------------------------------------------
repLocalSolution::repLocalSolution(ofreq::ReportManager *ptIn) : repLocalSolution::repLocalSolution()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void repLocalSolution::calcReport(int freqInd)
{
    try
    {
        complexDouble compI(0,1); //comlex number w/ 0 real, 1 imaginary used below for computations
        complexDouble wavefreq(0,0);   //Wave frequency.
        std::vector<complexDouble> output(3);    //Output list.
        std::vector<complexDouble> motion(6);     //List of translation motions.

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

        //Calculate output list.
        //X-axis outputs. (S_x = T_x - p_y * R_z + p_z * R_y)
        output.at(0) = motion.at(0) - pMomArm.at(1) * motion.at(5) + pMomArm.at(2) * motion.at(4);

        //Y-axis outputs.   (S_y = T_y - p_z * R_x + p_x * R_z)
        output.at(1) = motion.at(1) - pMomArm.at(2) * motion.at(3) + pMomArm.at(0) * motion.at(5);

        //Z-axis outputs.  (S_z = T_z + p_y * R_x + p_x * R_y)
        output.at(2) = motion.at(2) - pMomArm.at(1) * motion.at(3) + pMomArm.at(0) * motion.at(4);

        //Get current frequency, in body encounter frequency.
        wavefreq.real(
                    ptSystem->listWaveFrequencies(freqInd));

        //Calculate derivative.
        for (int i = 0; i < output.size(); i++)
        {
            output.at(i) = pow(wavefreq, pOrd)
                           * pow(compI, pOrd)
                           * output.at(i);
        }

        //Write resutls back to results list.
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
void repLocalSolution::calcRAO(int freqInd)
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

//------------------------------------------Function Separator --------------------------------------------------------
std::string repLocalSolution::getClass()
{
    return "repLocalSolution";
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string repLocalSolution::getFileName()
{
    return "sollocal.out";
}


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
void repLocalSolution::defineConst(int constIndex)
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


//==========================================Section Separator =========================================================
//Private Functions
