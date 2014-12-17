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
#include "repglobsolution.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;
using namespace arma;

//==========================================Section Separator =========================================================
//Static variables
string repGlobSolution::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repGlobSolution::KEY_ORDER = "order";   /**< Order of derivative to use for processing the output.*/


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repGlobSolution::repGlobSolution()
{
    //Assign a default value for the name.
    this->setName("Body Global Solution");
}

//------------------------------------------Function Separator --------------------------------------------------------
repGlobSolution::repGlobSolution(ofreq::ReportManager *ptIn) : repGlobSolution::repGlobSolution()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void repGlobSolution::calcReport(int freqInd)
{
    //Define constants
    for (unsigned int i = 0; i < plistConst_Key.size(); i++)
    {
        this->defineConst(i);
    }

    cx_mat* output = new cx_mat();
    complexDouble compI(0,1); //comlex number w/ 0 real, 1 imaginary used below for computations
    complexDouble wavefreq(0,0);   //Wave frequency.

    //Get number of rows for output matrix.
    int nRow = ptSystem->listSolutionSet(pBodIndex).refSolution(0,0).refSolnMat().n_rows;

    //Get current frequency
    wavefreq.real(
                ptSystem->listWaveFrequencies(freqInd));

    //Resize matrix.
    output->set_size(nRow,1);

    //Get base solution values.
    ofreq::Solution* input =
            &(ptSystem->listSolutionSet(this->getBodIndex())
            .refSolution(
                ptSystem->getCurWaveDirInd(),
                freqInd));

    for (int i = 0; i < nRow; i++)
    {
        //Calculate derivative.
        output->at(i,0) = pow(wavefreq, pOrd)
                          * pow(compI, pOrd)
                          * input->refSolnMat()(i,0);
    }

    //Write result to results list.
    plistData.push_back(
                Data(freqInd));

    for (unsigned int i = 0; i < output->n_rows; i++)
    {
        plistData.back().addValue(
                    output->at(i,0));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
string repGlobSolution::getClass()
{
    return "repGlobalSolution";
}

//------------------------------------------Function Separator --------------------------------------------------------
std::string repGlobSolution::getFileName()
{
    return "solglobal.out";
}


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
void repGlobSolution::defineConst(int constIndex)
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

