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
#include "repabseff.h"
#include "../system_objects/system.h"


using namespace std;
using namespace osea::ofreq;
using namespace arma;

//==========================================Section Separator =========================================================
//Static variables
string repAbsEff::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repAbsEff::KEY_POWERREP = "repPower";    /**< Name of the power report to reference. */
string repAbsEff::KEY_VARS = "variables";        /**< variable indices used to sum for efficiency calculation. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repAbsEff::repAbsEff()
{
    //Assign a default value to the name.
    this->setName("Body Absolute Efficiency");

    //Clear the list of variables
    plistAdded = false;
    plistVars.clear();
    ptPower = NULL;
}

//------------------------------------------Function Separator --------------------------------------------------------
repAbsEff::repAbsEff(ReportManager *ptIn) : repAbsEff()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void repAbsEff::calcReport(int freqInd)
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
void repAbsEff::calcRAO(int freqInd)
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
string repAbsEff::getClass()
{
    //Name of the class
    return "repAbsoluteEfficiency";
}

//------------------------------------------Function Separator --------------------------------------------------------
string repAbsEff::getFileName()
{
    //Filename for the class.
    return "efficiency_absolute.out";
}

//==========================================Section Separator =========================================================
//Protected Functions


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void repAbsEff::defineConst(int constIndex)
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
double repAbsEff::calcWavePow(int freqInd)
{
    try
    {
        //Calculate maximum wave power that can be captured by the device.

        //Create pointer to motion model.
        MotionModel *ptMotion = &(ptBody->getMotionModel());

        //Set which body to use as the current body in the motion model.
        ptMotion->setBody(pBodIndex);

        //Set the current wave direction index.
        ptMotion->setWaveDirInd(
                    ptSystem->getCurWaveDirInd()
                    );

        //Set the current wave frequency to use
        ptMotion->setFreq(freqInd,
                          ptSystem->listWaveFrequencies(freqInd)
                          );

        //Let the model know to calculate true coefficients.
        ptMotion->CoefficientOnly() = true;

        //Set the current frequency index for the system.
        ptSystem->setCurFreqInd(freqInd);

        //Reset, just for good measure.
        ptMotion->Reset();

        //Build up the formula for maximum power calculation.
        //P_max = 0.125 * SUM from v=1 to 6 { |forceActive_hydro|^2 / coeffReact(1,v) }

        //Create output matrix for hydrodynamic active forces.
        cx_mat CoeffActive(ptMotion->getNumEquations(), 1);

        //Iterate through each hydrodynamic force and add to the total.
        for (unsigned int i = 0; i < ptBody->listForceActive_hydro().size(); i++)
        {
            CoeffActive = CoeffActive + ptMotion->getMatForceActive_hydro(i);
        }

        //Convert outputs to real numbers, squared.
        //Include the division by 1/8.
        std::vector<double> ForceAct;
        ForceAct.resize(ptMotion->getNumEquations());
        for (unsigned int i = 0; i < CoeffActive.n_rows; i++)
            ForceAct.at(i) = 0.125 * pow( abs(CoeffActive(i,0)), 2);

        //Get the reactive force coefficients.
        std::vector<double> kReact;
        kReact.resize(ptMotion->getNumEquations());

        for(unsigned int i = 0; i < ptBody->listForceReact_hydro().size(); i++)
        {
            //Get output.
            std::vector<double> temp = calcForceReact_hydro(i);

            for(unsigned int j = 0; j < temp.size(); j++)
            {
                //Add output to the total coefficients.
                kReact.at(j) += temp.at(j);
            }
        }

        //Combine the two for total max power output
        double maxPow = 0;

        for (unsigned int i = 0; i < kReact.size(); i++)
        {
            try
            {
                maxPow += ForceAct.at(i) / kReact.at(i);
            }
            catch(...)
            {
                //No action needed.
            }
        }


        return maxPow;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> repAbsEff::calcForceReact_hydro(int forceInd)
{
    try
    {
        //Create pointer to motion model.
        MotionModel *ptMotion = &(ptBody->getMotionModel());

        cx_mat coeff;   //Matrix of coefficients retrieved from the motion model.
        //Setup basic properties for motion model.
        ptMotion->Reset();                              //Give it a reset just for good measure.

        //Get the coefficient matrix for the specified force and first order of derivative.
        try
        {
            coeff = ptMotion->getMatForceReact_hydro(forceInd, 1);
        }
        catch(const std::exception &err)
        {
            //Just return a matrix of zeros.
            coeff.zeros(ptMotion->getNumEquations(), ptMotion->getNumEquations());
        }

        //Prepare the j component and wave frequency to divide them out.
        complex<double> scalar;
        scalar.imag(ptSystem->getCurFreq());

        vector<double> output;     //Output matrix.
        output.resize(coeff.n_rows);

        for (unsigned int i = 0; i < coeff.n_rows; i++)
        {
            //Divide out wave frequency and j component.
            coeff(i,i) = coeff(i,i) / scalar;

            //Convert back into real values
            output.at(i) = coeff(i,i).real();
        }

        //Write output
        return output;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repAbsEff::addVarNames()
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
