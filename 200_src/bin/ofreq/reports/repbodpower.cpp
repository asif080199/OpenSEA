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
#include "repbodpower.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;
using namespace arma;

//==========================================Section Separator =========================================================
//Static variables
string repBodPower::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repBodPower::KEY_ORDER = "order";   /**< Order of derivative to use for processing the output.*/
string repBodPower::KEY_CATEGORY = "category";    /**< Name of the category of force type. */
string repBodPower::KEY_FORCENAME = "forcename"; /**< Name of the specific user forcename you want. */
string repBodPower::SEPARATOR = "::"; /** Separator between force name and linked body name. */

//------------------------------------------Function Separator ----------------------------------------------------
string repBodPower::CAT_REACT = "react";   /**< KEY for total of reactive forces. */
string repBodPower::CAT_REACT_USER = "react_user"; /**< Key for user reactive forces. */
string repBodPower::CAT_REACT_HYDRO = "react_hydro"; /**< Key for hydro reactive forces. */
string repBodPower::CAT_CROSS = "cross";   /**< Key for total of crossbody forces. */
string repBodPower::CAT_CROSS_USER = "cross_user"; /**< Key for user crossbody forces. */
string repBodPower::CAT_CROSS_HYDRO = "cross_hydro"; /**< Key for hydro crossbody forces. */
string repBodPower::CAT_TOTAL = "total";   /**< Key for total forces. */
string repBodPower::CAT_TOTAL_USER = "total_user"; /**< Key for all user forces. */
string repBodPower::CAT_TOTAL_HYDRO = "total_hydro"; /**< Key for all hydro forces. */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repBodPower::repBodPower()
{
    //Set default report name.
    this->setName("Power Report");
}

//------------------------------------------Function Separator --------------------------------------------------------
repBodPower::repBodPower(ofreq::ReportManager *ptIn) : repBodPower()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodPower::calcReport(int freqInd)
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

        //Check to make sure the body was assigned and the motion model.
        if (!ptBody)
            throw std::runtime_error(string("No body found which matches the given body name."));

        if (!ptMotion)
            throw std::runtime_error(string("No motion model assigned."));

        //Set which body to use as the current body in the motion model.
        ptMotion->setBody(pBodIndex);

        //Set the pointer to the solution set.
        ptMotion->setSolutionSet(
                    &(ptSystem->listSolutionSet())
                    );

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

        //Create vector for output.
        std::vector<double> output;
        output.resize(ptMotion->getNumEquations());

        //Activate forces, as requested by user.
        //--------------------------------------
        if (pCategory == CAT_REACT)
        {
            //Calculate power for reactive forces
            std::vector<double> temp;
            temp.clear();
            temp = PowReact_user(pForceName, pOrd); //User reactive force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);

            temp.clear();
            temp = PowReact_hydro(pForceName, pOrd); //Hydro reactive force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        if (pCategory == CAT_REACT_USER)
        {
            //Calculate power for reactive forces
            std::vector<double> temp;
            temp.clear();
            temp = PowReact_user(pForceName, pOrd); //User reactive force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        if (pCategory == CAT_REACT_HYDRO)
        {
            //Calculate power for reactive forces
            std::vector<double> temp;
            temp.clear();
            temp = PowReact_hydro(pForceName, pOrd); //Hydro reactive force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        if (pCategory == CAT_CROSS)
        {
            //Calculate power for crossbody forces
            std::vector<double> temp;
            temp.clear();
            temp = PowCross_user(pForceName, pLinkBody, pOrd); //User crossbody force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);

            temp.clear();
            temp = PowCross_hydro(pForceName, pLinkBody, pOrd); //Hydro crossbody force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        if (pCategory == CAT_CROSS_USER)
        {
            //Calculate power for crossbody forces
            std::vector<double> temp;
            temp.clear();
            temp = PowCross_user(pForceName, pLinkBody, pOrd); //User crossbody force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        if (pCategory == CAT_CROSS_HYDRO)
        {
            //Calculate power for crossbody forces
            std::vector<double> temp;
            temp.clear();
            temp = PowCross_hydro(pForceName, pLinkBody, pOrd); //Hydro crossbody force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        if (pCategory == CAT_TOTAL)
        {
            //Calculate power for all forces
            std::vector<double> temp;
            temp.clear();

            //Calculate power for reactive forces
            temp.clear();
            temp = PowReact_user(pForceName, pOrd); //User reactive force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);

            temp.clear();
            temp = PowReact_hydro(pForceName, pOrd); //Hydro reactive force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);

            //Calculate power for crossbody forces
            temp.clear();
            temp = PowCross_user(pForceName, pLinkBody, pOrd); //User crossbody force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);

            temp.clear();
            temp = PowCross_hydro(pForceName, pLinkBody, pOrd); //Hydro crossbody force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        if (pCategory == CAT_TOTAL_USER)
        {
            //Calculate power for all forces
            std::vector<double> temp;

            //Calculate power for reactive forces
            temp.clear();
            temp = PowReact_user(pForceName, pOrd); //User reactive force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);

            //Calculate power for crossbody forces
            temp.clear();
            temp = PowCross_user(pForceName, pLinkBody, pOrd); //User crossbody force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        if (pCategory == CAT_TOTAL_HYDRO)
        {
            //Calculate power for all forces
            std::vector<double> temp;

            //Calculate power for reactive forces
            temp.clear();
            temp = PowReact_hydro(pForceName, pOrd); //Hydro reactive force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);

            //Calculate power for crossbody forces
            temp.clear();
            temp = PowCross_hydro(pForceName, pLinkBody, pOrd); //Hydro crossbody force
            for (unsigned int k = 0; k < temp.size(); k++)
                output.at(k) += temp.at(k);
        }

        plistData.push_back(Data(freqInd));

        //Calculate results and add to list of data.
        for (unsigned int i = 0; i < output.size(); i++ )
        {
            plistData.back().addValue(
                        output.at(i));
        }

        //All done.
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodPower::calcRAO(int freqInd)
{
    try
    {
        if (pCalcRAO)
        {
            double amp;

            //Get amplitude.
            amp = ptSystem->refActiveSeaModel().getWaveAmp(ptSystem->getCurWaveDirInd(), freqInd);

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
string repBodPower::getClass()
{
    return "repBodPower";
}


//------------------------------------------Function Separator --------------------------------------------------------
string repBodPower::getFileName()
{
    return "bodypower.out";
}


//==========================================Section Separator =========================================================
//Protected Functions



//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
void repBodPower::defineConst(int constIndex)
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
                    //Assign body
                    ptBody = &(ptSystem->listBody(i));

                    //Assign the motion model.
                    string modelName = "";
                    modelName = ptBody->getMotionModel().getName();

                    ptMotion = &(ptSystem->listModel(
                                   ptBody->getMotionModel().getName()
                                   ));

                    //Add the list of equations from the motion model, for user reference in output.
                    plistConst_Key.push_back(string("equation_sequence"));
                    plistConst_Val.push_back(Data());
                    for (unsigned int i = 0; i < ptMotion->listEquation().size(); i++)
                    {
                        plistConst_Val.back().addString(
                                    ptMotion->listEquation(i).getName()
                                    );
                    }

                    break;
                }
            }
        }

        else if (plistConst_Key.at(constIndex) == KEY_ORDER)
        {
            //Process order of derivative.
            pOrd = plistConst_Val.at(constIndex).listValueInt(0);
        }

        else if (plistConst_Key.at(constIndex) == KEY_CATEGORY)
        {
            //Process category.
            pCategory = plistConst_Val.at(constIndex).listString(0);
        }

        else if (plistConst_Key.at(constIndex) == KEY_FORCENAME)
        {
            //Process force name.

            //First check if this is a linked body and includes a separator.
            if (plistConst_Val.at(constIndex).listString(0).find(SEPARATOR) != std::string::npos)
            {
                //Separator specified.  First check that this is a crossbody force.
                if ((pCategory != CAT_CROSS)
                        && (pCategory != CAT_CROSS_USER)
                        && (pCategory != CAT_CROSS_HYDRO))
                {
                    //Improper use of the name.  THrow an error.
                    throw std::runtime_error(string("Cannot specify linked body unless force category is ")
                                             + CAT_CROSS + " or "
                                             + CAT_CROSS_USER + " or "
                                             + CAT_CROSS_HYDRO);
                }

                //Search for position of separator
                std::string::size_type index = plistConst_Val.at(constIndex).listString(0).find(SEPARATOR);
                std::string forceName;      //Name for the force object.
                std::string linkName;       //Name for the linked body.

                forceName = plistConst_Val.at(constIndex).listString(0).substr(0, index);
                linkName = plistConst_Val.at(constIndex).listString(0).substr(index + SEPARATOR.length());

                //First set the force object name.
                pForceName = forceName;

                //Next set the linked body name and get the linked body.
                pLinkBody = linkName;

                //Get the correct body and assign it.
                for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
                {
                    if (ptSystem->listBody(i).getBodyName() == linkName)
                    {
                        //Assign body
                        ptLinkBody = &(ptSystem->listBody(i));
                        break;
                    }
                }
            }
            else
            {
                //Not a linked body object.

                //Set the force name.
                pForceName = plistConst_Val.at(constIndex).listString(0);
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
std::vector<double> repBodPower::PowReact_user(std::string forceName, int ord)
{
    try
    {
        vector<double> power;
        power.resize(
                    ptMotion->getNumEquations()
                    );

        //Check if using one force, or all of them.
        if (forceName == "")
        {
            //Using all forces.
            //--------------------------------------------

            //Check if using one order of derivative, or all of them.
            if (ord < 0)
            {
                //Using all orders of derivative.
                //--------------------------------------------
                //Iterate through all forces and orders of derivative.  Get results.
                for (unsigned int i = 0; i < ptBody->listForceReact_user().size(); i++)
                {
                    for (unsigned int j = 0; j < ptBody->listForceReact_user(i)->getMaxOrd(); j++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowReact_user(i, j);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }

            else
            {
                //Just one order of derivative.
                //--------------------------------------------
                //Iterate through each force, and apply order of derivative.
                for (unsigned int i = 0; i < ptBody->listForceReact_user().size(); i++)
                {
                    //Add up powers
                    vector<double> output =
                            calcPowReact_user(i, ord);

                    for (unsigned int k = 0; k < output.size(); k++)
                        power.at(k) += output.at(k);
                }
            }
        }
        else
        {
            //Just one force.
            //--------------------------------------------
            int forceIndex = -1;     //Index of the force object.

            //Search through list of forces in body and use just one.
            for (unsigned int i = 0; i < ptBody->listForceReact_user().size(); i++)
            {
                if (forceName == ptBody->listForceReact_user(i)->getForceName())
                {
                    //Located the force object.
                    forceIndex = i;
                    break;
                }
            }

            //Check that an index was found.
            if (forceIndex < 0)
            {
                throw std::runtime_error(string("No Reactive force found which matches the given name.  Offending entry:  ") + forceName);
            }

            //Check if using one order of derivative, or all of them.
            if (ord < 0)
            {
                //Using all orders of derivative.
                //--------------------------------------------
                //Iterate through all orders of derivative.  Get results.
                for (unsigned int j = 0; j < ptBody->listForceReact_user(forceIndex)->getMaxOrd(); j++)
                {
                    //Add up powers
                    vector<double> output =
                            calcPowReact_user(forceIndex, j);

                    for (unsigned int k = 0; k < output.size(); k++)
                        power.at(k) += output.at(k);
                }
            }
            else
            {
                //Just one order of derivative.
                //--------------------------------------------
                //Add up powers
                vector<double> output =
                        calcPowReact_user(forceIndex, ord);

                for (unsigned int k = 0; k < output.size(); k++)
                    power.at(k) += output.at(k);
            }
        }

        //Write results
        return power;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> repBodPower::PowReact_hydro(std::string forceName, int ord)
{
    try
    {
        vector<double> power;
        power.resize(
                    ptMotion->getNumEquations()
                    );

        //Check if using one force, or all of them.
        if (forceName == "")
        {
            //Using all forces.
            //--------------------------------------------

            //Check if using one order of derivative, or all of them.
            if (ord < 0)
            {
                //Using all orders of derivative.
                //--------------------------------------------
                //Iterate through all forces and orders of derivative.  Get results.
                for (unsigned int i = 0; i < ptBody->listForceReact_hydro().size(); i++)
                {
                    for (unsigned int j = 0; j < ptBody->listForceReact_hydro(i)->getMaxOrd(); j++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowReact_hydro(i, j);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }

            else
            {
                //Just one order of derivative.
                //--------------------------------------------
                //Iterate through each force, and apply order of derivative.
                for (unsigned int i = 0; i < ptBody->listForceReact_hydro().size(); i++)
                {
                    //Add up powers
                    vector<double> output =
                            calcPowReact_hydro(i, ord);

                    for (unsigned int k = 0; k < output.size(); k++)
                        power.at(k) += output.at(k);
                }
            }
        }
        else
        {
            //Just one force.
            //--------------------------------------------
            int forceIndex = -1;     //Index of the force object.

            //Search through list of forces in body and use just one.
            for (unsigned int i = 0; i < ptBody->listForceReact_hydro().size(); i++)
            {
                if (forceName == ptBody->listForceReact_hydro(i)->getForceName())
                {
                    //Located the force object.
                    forceIndex = i;
                    break;
                }
            }

            //Check that an index was found.
            if (forceIndex < 0)
            {
                throw std::runtime_error(string("No Reactive force found which matches the given name.  Offending entry:  ") + forceName);
            }

            //Check if using one order of derivative, or all of them.
            if (ord < 0)
            {
                //Using all orders of derivative.
                //--------------------------------------------
                //Iterate through all orders of derivative.  Get results.
                for (unsigned int j = 0; j < ptBody->listForceReact_hydro(forceIndex)->getMaxOrd(); j++)
                {
                    //Add up powers
                    vector<double> output =
                            calcPowReact_hydro(forceIndex, j);

                    for (unsigned int k = 0; k < output.size(); k++)
                        power.at(k) += output.at(k);
                }
            }
            else
            {
                //Just one order of derivative.
                //--------------------------------------------
                //Add up powers
                vector<double> output =
                        calcPowReact_hydro(forceIndex, ord);

                for (unsigned int k = 0; k < output.size(); k++)
                    power.at(k) += output.at(k);
            }
        }

        //Write results
        return power;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> repBodPower::PowCross_user(std::string forceName, std::string linkName, int ord)
{
    try
    {
        vector<double> power;
        power.resize(
                    ptMotion->getNumEquations()
                    );

        //Check if using one force, or all of them.
        if (forceName == "")
        {
            //Using all forces.
            //-----------------------------------------

            //Check if using one linkedBody, or all of them.
            if (linkName == "")
            {
                //Using all linked bodies.
                //-----------------------------------------

                //Check if using one order of derivative, or all of them.
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    //-----------------------------------------
                    //Iterate through all forces, linked bodies, and orders of derivative.  Get results.
                    for (unsigned int i = 0; i < ptBody->listForceCross_user().size(); i++)
                    {
                        for (unsigned int j = 0; j < ptBody->listForceCross_user(i)->getMaxOrd(); j++)
                        {
                            //Add up powers
                            vector<double> output =
                                    calcPowCross_user(i, j);

                            for (unsigned int k = 0; k < output.size(); k++)
                                power.at(k) += output.at(k);
                        }
                    }
                }
                else
                {
                    //Just one order of derivative.
                    //-----------------------------------------
                    //Iterate through each force, and apply order of derivative.
                    for (unsigned int i = 0; i < ptBody->listForceCross_user().size(); i++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowCross_user(i, ord);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }
            else
            {
                //Just one linked body.
                //-----------------------------------------

                //Check if using one order of derivative, or all of them.
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("user", 2, forceName, linkName);

                    //Iterate through all listed forces and orders of derivative.  Get results.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        for (unsigned int j = 0; j < ptBody->listForceCross_user(index.at(i))->getMaxOrd(); j++)
                        {
                            //Add up powers
                            vector<double> output =
                                    calcPowCross_user(index.at(i), j);

                            for (unsigned int k = 0; k < output.size(); k++)
                                power.at(k) += output.at(k);
                        }
                    }
                }
                else
                {
                    //Just one order of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("user", 2, forceName, linkName);

                    //Iterate through each force, and apply order of derivative.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowCross_user(index.at(i), ord);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }
        }
        else
        {
            //Just one force.
            //-----------------------------------------

            //Check if using one linkedBody, or all of them.
            if (linkName == "")
            {
                //Using all linked bodies.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("user", 1, forceName, linkName);

                    //Iterate through all listed forces and orders of derivative.  Get results.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        for (unsigned int j = 0; j < ptBody->listForceCross_user(index.at(i))->getMaxOrd(); j++)
                        {
                            //Add up powers
                            vector<double> output =
                                    calcPowCross_user(index.at(i), j);

                            for (unsigned int k = 0; k < output.size(); k++)
                                power.at(k) += output.at(k);
                        }
                    }
                }
                else
                {
                    //Just one order of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("user", 1, forceName, linkName);

                    //Iterate through each force, and apply order of derivative.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowCross_user(index.at(i), ord);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }
            else
            {
                //Just one linked body.
                //-----------------------------------------

                //Check if using one order of derivative, or all of them.
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("user", 0, forceName, linkName);

                    //Iterate through all listed forces and orders of derivative.  Get results.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        for (unsigned int j = 0; j < ptBody->listForceCross_user(index.at(i))->getMaxOrd(); j++)
                        {
                            //Add up powers
                            vector<double> output =
                                    calcPowCross_user(index.at(i), j);

                            for (unsigned int k = 0; k < output.size(); k++)
                                power.at(k) += output.at(k);
                        }
                    }
                }
                else
                {
                    //Just one order of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("user", 0, forceName, linkName);

                    //Iterate through each force, and apply order of derivative.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowCross_user(index.at(i), ord);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }
        }

        //Write results
        return power;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> repBodPower::PowCross_hydro(std::string forceName, std::string linkName, int ord)
{
    try
    {
        vector<double> power;
        power.resize(
                    ptMotion->getNumEquations()
                    );

        //Check if using one force, or all of them.
        if (forceName == "")
        {
            //Using all forces.
            //-----------------------------------------

            //Check if using one linkedBody, or all of them.
            if (linkName == "")
            {
                //Using all linked bodies.
                //-----------------------------------------

                //Check if using one order of derivative, or all of them.
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    //-----------------------------------------
                    //Iterate through all forces, linked bodies, and orders of derivative.  Get results.
                    for (unsigned int i = 0; i < ptBody->listForceCross_hydro().size(); i++)
                    {
                        for (unsigned int j = 0; j < ptBody->listForceCross_hydro(i)->getMaxOrd(); j++)
                        {
                            //Add up powers
                            vector<double> output =
                                    calcPowCross_hydro(i, j);

                            for (unsigned int k = 0; k < output.size(); k++)
                                power.at(k) += output.at(k);
                        }
                    }
                }
                else
                {
                    //Just one order of derivative.
                    //-----------------------------------------
                    //Iterate through each force, and apply order of derivative.
                    for (unsigned int i = 0; i < ptBody->listForceCross_hydro().size(); i++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowCross_hydro(i, ord);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }
            else
            {
                //Just one linked body.
                //-----------------------------------------

                //Check if using one order of derivative, or all of them.
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("hydro", 2, forceName, linkName);

                    //Iterate through all listed forces and orders of derivative.  Get results.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        for (unsigned int j = 0; j < ptBody->listForceCross_hydro(index.at(i))->getMaxOrd(); j++)
                        {
                            //Add up powers
                            vector<double> output =
                                    calcPowCross_hydro(index.at(i), j);

                            for (unsigned int k = 0; k < output.size(); k++)
                                power.at(k) += output.at(k);
                        }
                    }
                }
                else
                {
                    //Just one order of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("hydro", 2, forceName, linkName);

                    //Iterate through each force, and apply order of derivative.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowCross_hydro(index.at(i), ord);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }
        }
        else
        {
            //Just one force.
            //-----------------------------------------

            //Check if using one linkedBody, or all of them.
            if (linkName == "")
            {
                //Using all linked bodies.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("hydro", 1, forceName, linkName);

                    //Iterate through all listed forces and orders of derivative.  Get results.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        for (unsigned int j = 0; j < ptBody->listForceCross_hydro(index.at(i))->getMaxOrd(); j++)
                        {
                            //Add up powers
                            vector<double> output =
                                    calcPowCross_hydro(index.at(i), j);

                            for (unsigned int k = 0; k < output.size(); k++)
                                power.at(k) += output.at(k);
                        }
                    }
                }
                else
                {
                    //Just one order of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("hydro", 1, forceName, linkName);

                    //Iterate through each force, and apply order of derivative.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowCross_hydro(index.at(i), ord);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }
            else
            {
                //Just one linked body.
                //-----------------------------------------

                //Check if using one order of derivative, or all of them.
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("hydro", 0, forceName, linkName);

                    //Iterate through all listed forces and orders of derivative.  Get results.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        for (unsigned int j = 0; j < ptBody->listForceCross_hydro(index.at(i))->getMaxOrd(); j++)
                        {
                            //Add up powers
                            vector<double> output =
                                    calcPowCross_hydro(index.at(i), j);

                            for (unsigned int k = 0; k < output.size(); k++)
                                power.at(k) += output.at(k);
                        }
                    }
                }
                else
                {
                    //Just one order of derivative.
                    //-----------------------------------------
                    vector<int> index = findLinkIndex("hydro", 0, forceName, linkName);

                    //Iterate through each force, and apply order of derivative.
                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        //Add up powers
                        vector<double> output =
                                calcPowCross_hydro(index.at(i), ord);

                        for (unsigned int k = 0; k < output.size(); k++)
                            power.at(k) += output.at(k);
                    }
                }
            }
        }

        //Write results
        return power;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<double> repBodPower::calcPowReact_user(int forceInd, int ord)
{
    try
    {
        //Formula for power calculation.
        // P = 1/2 * pow(-1,(u+3)/2) * X_soln^2 * freq^(u+1) * k(u,v)

        cx_mat coeff;   //Matrix of coefficients retrieved from the motion model.
        vector<double> output;  //Results vector.
        output.resize(ptMotion->getNumEquations(), 0);    //Resize vector.

        if (ord % 2 != 0)       //Checking to make sure that only integrating odd orders of derivative.
        {
            //Setup basic properties for motion model.
            ptMotion->Reset();                              //Give it a reset just for good measure.

            //Get the coefficient matrix for the specified force and order of derivative.
            coeff = ptMotion->getMatForceReact_user(forceInd, ord);

            //Divide out the j component.
            complex<double> j_imag;
            j_imag.imag(1);
            j_imag = pow(j_imag, ord);

            //Multiply negative number and frequency.
            double negative = pow(-1,(ord+3)/2);

            //Add in wave frequency.

            for (unsigned int i = 0; i < coeff.n_rows; i++)
            {
                for (unsigned int j = 0; j < coeff.n_cols; j++)
                {
                    //Combine all three operations in one.
                    coeff(i,j) = coeff(i,j) / j_imag;  //Divide j component.
                    coeff(i,j) = negative * coeff(i,j); //Multiply negative.
                    coeff(i,j) = negative * ptSystem->getCurFreq(); //Multiply frequency.
                }
            }

            //Get the base solution values.
            ofreq::Solution* soln =
                    &(ptSystem->listSolutionSet(this->getBodIndex())
                    .refSolution(
                        ptSystem->getCurWaveDirInd(),
                        ptSystem->getCurFreqInd()));

            //Create matrix of real values and real solution values.
            Mat<double> k_coeff(coeff.n_rows, coeff.n_cols);        //Real value of k_coefficients
            Mat<double> X_soln(soln->refSolnMat().n_rows, 1);         //amplitude of solution.

            //Convert force coefficients into k coefficients.
            for (unsigned int i = 0; i < coeff.n_rows; i++)
            {
                for (unsigned int j = 0; j < coeff.n_cols; j++)
                {
                    k_coeff(i,j) = coeff(i,j).real();
                }
            }

            //Convert solutions into absolute values, squared.
            //Include the 1/2 term to complete the formula for power calculation.
            for (unsigned int i = 0 ; i < soln->refSolnMat().n_rows; i++)
            {
                X_soln(i,0) = 0.5 * pow(
                                  abs(soln->refSolnMat()(i,0)
                                      ),
                                  2);
            }

            //Multiply each term to get the vector of results.
            Mat<double> matOut(soln->refSolnMat().n_rows, 1);
            matOut = k_coeff * X_soln;

            //Extract each term and put it in a vector to write out.
            for (unsigned int i = 0; i < matOut.n_rows; i++)
                output.at(i) = matOut(i,0);
        }

        //Write results.
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
std::vector<double> repBodPower::calcPowReact_hydro(int forceInd, int ord)
{
    try
    {
        //Formula for power calculation.
        // P = 1/2 * pow(-1,(u+3)/2) * X_soln^2 * freq^(u+1) * k(u,v)

        cx_mat coeff;   //Matrix of coefficients retrieved from the motion model.
        vector<double> output;  //Results vector.
        output.resize(ptMotion->getNumEquations(), 0);    //Resize vector.

        if (ord % 2 != 0)       //Checking to make sure that only integrating odd orders of derivative.
        {
            //Setup basic properties for motion model.
            ptMotion->Reset();                              //Give it a reset just for good measure.

            //Get the coefficient matrix for the specified force and order of derivative.
            coeff = ptMotion->getMatForceReact_hydro(forceInd, ord);

            //Divide out the j component.
            complex<double> j_imag;
            j_imag.imag(1);
            j_imag = pow(j_imag, ord);

            //Multiply negative number and frequency.
            double negative = pow(-1,(ord+3)/2);

            //Add in wave frequency.

            for (unsigned int i = 0; i < coeff.n_rows; i++)
            {
                for (unsigned int j = 0; j < coeff.n_cols; j++)
                {
                    //Combine all three operations in one.
                    coeff(i,j) = coeff(i,j) / j_imag;  //Divide j component.
                    coeff(i,j) = negative * coeff(i,j); //Multiply negative.
                    coeff(i,j) = coeff(i,j) * ptSystem->getCurFreq(); //Multiply frequency.
                }
            }

            //Get the base solution values.
            ofreq::Solution* soln =
                    &(ptSystem->listSolutionSet(this->getBodIndex())
                    .refSolution(
                        ptSystem->getCurWaveDirInd(),
                        ptSystem->getCurFreqInd()));

            //Create matrix of real values and real solution values.
            Mat<double> k_coeff(coeff.n_rows, coeff.n_cols);        //Real value of k_coefficients
            Mat<double> X_soln(soln->refSolnMat().n_rows, 1);         //amplitude of solution.

            //Convert force coefficients into k coefficients.
            for (unsigned int i = 0; i < coeff.n_rows; i++)
            {
                for (unsigned int j = 0; j < coeff.n_cols; j++)
                {
                    k_coeff(i,j) = coeff(i,j).real();
                }
            }

            //Convert solutions into absolute values, squared.
            //Include the 1/2 term to complete the formula for power calculation.
            for (unsigned int i = 0 ; i < soln->refSolnMat().n_rows; i++)
            {
                X_soln(i,0) = 0.5 * pow(
                                  abs(soln->refSolnMat()(i,0)
                                      ),
                                  2);
            }

            //Multiply each term to get the vector of results.
            Mat<double> matOut(soln->refSolnMat().n_rows, 1);
            matOut = k_coeff * X_soln;

            //Extract each term and put it in a vector to write out.
            for (unsigned int i = 0; i < matOut.n_rows; i++)
                output.at(i) = matOut(i,0);
        }

        //Write results.
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
std::vector<double> repBodPower::calcPowCross_user(int forceInd, int ord)
{
    try
    {
        //Formula for power calculation.
        // P = 1/2 * pow(-1,(u+3)/2) * X_soln^2 * freq^(u+1) * k(u,v)

        cx_mat coeff;   //Matrix of coefficients retrieved from the motion model.
        vector<double> output;  //Results vector.
        output.resize(ptMotion->getNumEquations(), 0);    //Resize vector.

        if (ord % 2 != 0)       //Checking to make sure that only integrating odd orders of derivative.
        {
            //Setup basic properties for motion model.
            ptMotion->Reset();                              //Give it a reset just for good measure.

            //Get the coefficient matrix for the specified force and order of derivative.
            coeff = ptMotion->getMatForceCross_user(forceInd, ord);

            //Divide out the j component.
            complex<double> j_imag;
            j_imag.imag(1);
            j_imag = pow(j_imag, ord);

            //Multiply negative number and frequency.
            double negative = pow(-1,(ord+3)/2);

            //Add in wave frequency.

            for (unsigned int i = 0; i < coeff.n_rows; i++)
            {
                for (unsigned int j = 0; j < coeff.n_cols; j++)
                {
                    //Combine all three operations in one.
                    coeff(i,j) = coeff(i,j) / j_imag;  //Divide j component.
                    coeff(i,j) = negative * coeff(i,j); //Multiply negative.
                    coeff(i,j) = negative * ptSystem->getCurFreq(); //Multiply frequency.
                }
            }

            //Get index of the linked body.
            int linkId = ptMotion->listCompCrossBod_user(forceInd);

            //Get the base solution values.
            ofreq::Solution* soln =
                    &(ptSystem->listSolutionSet(linkId)
                    .refSolution(
                        ptSystem->getCurWaveDirInd(),
                        ptSystem->getCurFreqInd()));

            //Create matrix of real values and real solution values.
            Mat<double> k_coeff(coeff.n_rows, coeff.n_cols);        //Real value of k_coefficients
            Mat<double> X_soln(soln->refSolnMat().n_rows, 1);         //amplitude of solution.

            //Convert force coefficients into k coefficients.
            for (unsigned int i = 0; i < coeff.n_rows; i++)
            {
                for (unsigned int j = 0; j < coeff.n_cols; j++)
                {
                    k_coeff(i,j) = coeff(i,j).real();
                }
            }

            //Convert solutions into absolute values, squared.
            //Include the 1/2 term to complete the formula for power calculation.
            for (unsigned int i = 0 ; i < soln->refSolnMat().n_rows; i++)
            {
                X_soln(i,0) = 0.5 * pow(
                                  abs(soln->refSolnMat()(i,0)
                                      ),
                                  2);
            }

            //Multiply each term to get the vector of results.
            Mat<double> matOut(soln->refSolnMat().n_rows, 1);
            matOut = k_coeff * X_soln;

            //Extract each term and put it in a vector to write out.
            for (unsigned int i = 0; i < matOut.n_rows; i++)
                output.at(i) = matOut(i,0);
        }

        //Write results.
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
std::vector<double> repBodPower::calcPowCross_hydro(int forceInd, int ord)
{
    try
    {
        //Formula for power calculation.
        // P = 1/2 * pow(-1,(u+3)/2) * X_soln^2 * freq^(u+1) * k(u,v)

        cx_mat coeff;   //Matrix of coefficients retrieved from the motion model.
        vector<double> output;  //Results vector.
        output.resize(ptMotion->getNumEquations(), 0);    //Resize vector.

        if (ord % 2 != 0)       //Checking to make sure that only integrating odd orders of derivative.
        {
            //Setup basic properties for motion model.
            ptMotion->Reset();                              //Give it a reset just for good measure.

            //Get the coefficient matrix for the specified force and order of derivative.
            coeff = ptMotion->getMatForceCross_hydro(forceInd, ord);

            //Divide out the j component.
            complex<double> j_imag;
            j_imag.imag(1);
            j_imag = pow(j_imag, ord);

            //Multiply negative number and frequency.
            double negative = pow(-1,(ord+3)/2);

            //Add in wave frequency.

            for (unsigned int i = 0; i < coeff.n_rows; i++)
            {
                for (unsigned int j = 0; j < coeff.n_cols; j++)
                {
                    //Combine all three operations in one.
                    coeff(i,j) = coeff(i,j) / j_imag;  //Divide j component.
                    coeff(i,j) = negative * coeff(i,j); //Multiply negative.
                    coeff(i,j) = negative * ptSystem->getCurFreq(); //Multiply frequency.
                }
            }

            //Get index of the linked body.
            int linkId = ptMotion->listCompCrossBod_hydro(forceInd);

            //Get the base solution values.
            ofreq::Solution* soln =
                    &(ptSystem->listSolutionSet(linkId)
                    .refSolution(
                        ptSystem->getCurWaveDirInd(),
                        ptSystem->getCurFreqInd()));

            //Create matrix of real values and real solution values.
            Mat<double> k_coeff(coeff.n_rows, coeff.n_cols);        //Real value of k_coefficients
            Mat<double> X_soln(soln->refSolnMat().n_rows, 1);         //amplitude of solution.

            //Convert force coefficients into k coefficients.
            for (unsigned int i = 0; i < coeff.n_rows; i++)
            {
                for (unsigned int j = 0; j < coeff.n_cols; j++)
                {
                    k_coeff(i,j) = coeff(i,j).real();
                }
            }

            //Convert solutions into absolute values, squared.
            //Include the 1/2 term to complete the formula for power calculation.
            for (unsigned int i = 0 ; i < soln->refSolnMat().n_rows; i++)
            {
                X_soln(i,0) = 0.5 * pow(
                                  abs(soln->refSolnMat()(i,0)
                                      ),
                                  2);
            }

            //Multiply each term to get the vector of results.
            Mat<double> matOut(soln->refSolnMat().n_rows, 1);
            matOut = k_coeff * X_soln;

            //Extract each term and put it in a vector to write out.
            for (unsigned int i = 0; i < matOut.n_rows; i++)
                output.at(i) = matOut(i,0);
        }

        //Write results.
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
std::vector<int> repBodPower::findLinkIndex(std::string typeForce, int match, std::string forceName, std::string bodName)
{
    //Search through body and find the index of the linked body.
    bool test = false;  //Boolean to check which type of tests to run.
    vector<int> output; //Vector of output indices.

    try
    {
        if (typeForce == "user")
        {
            //First check that the linked body was assigned.
            if (!ptLinkBody)
            {
                throw std::runtime_error(string("No linked body found which matches the given name.  Name:  ")
                                         + bodName);
            }

            for (unsigned int i = 0; i < ptBody->listCrossBody_user().size(); i++)
            {
                test = false;

                //Check if matching linked body name.
                if ((match == 0) || (match = 2))
                    test = (ptBody->listCrossBody_user(i).getBodyName() == bodName);

                //Check if matching force name.
                if ((match == 0) || (match == 1))
                    test = test && (ptBody->listForceCross_user(i)->getForceName() == forceName);

                if (test)
                {
                    //Correct body found.
                    output.push_back(i);
                    break;
                }
            }
        }

        if (typeForce == "hydro")
        {
            //First check that the linked body was assigned.
            if (!ptLinkBody)
            {
                throw std::runtime_error(string("No linked body found which matches the given name.  Name:  ")
                                         + bodName);
            }

            for (unsigned int i = 0; i < ptBody->listCrossBody_hydro().size(); i++)
            {
                test = false;

                //Check if matching linked body name.
                if ((match == 0) || (match = 2))
                    test = (ptBody->listCrossBody_hydro(i).getBodyName() == bodName);

                //Check if matching force name.
                if ((match == 0) || (match == 1))
                    test = test && (ptBody->listForceCross_hydro(i)->getForceName() == forceName);

                if (test)
                {
                    //Correct body found.
                    output.push_back(i);
                    break;
                }
            }
        }

        //Return result.

        return output;
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}
