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
#include "repbodforces.h"
#include "../system_objects/system.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static variables
string repBodForces::KEY_BODY = "body";    /**< Name of the body to associate with.*/
string repBodForces::KEY_ORDER = "order";   /**< Order of derivative to use for processing the output.*/
string repBodForces::KEY_CATEGORY = "category";    /**< Name of the category of force type. */
string repBodForces::KEY_FORCENAME = "forcename"; /**< Name of the specific user forcename you want. */
string repBodForces::SEPARATOR = "::"; /** Separator between force name and linked body name. */

//------------------------------------------Function Separator ----------------------------------------------------
string repBodForces::CAT_REACT = "react";   /**< KEY for total of reactive forces. */
string repBodForces::CAT_REACT_USER = "react_user"; /**< Key for user reactive forces. */
string repBodForces::CAT_REACT_HYDRO = "react_hydro"; /**< Key for hydro reactive forces. */
string repBodForces::CAT_MASS = "mass";    /**< Key for mass forces. */
string repBodForces::CAT_CROSS = "cross";   /**< Key for total of crossbody forces. */
string repBodForces::CAT_CROSS_USER = "cross_user"; /**< Key for user crossbody forces. */
string repBodForces::CAT_CROSS_HYDRO = "cross_hydro"; /**< Key for hydro crossbody forces. */
string repBodForces::CAT_ACTIVE = "active"; /**< Key for total of active forces.  */
string repBodForces::CAT_ACTIVE_USER = "active_user"; /**< Key for user active forces. */
string repBodForces::CAT_ACTIVE_HYDRO = "active_hydro"; /**< Key for hydro active forces. */
string repBodForces::CAT_TOTAL = "total";   /**< Key for total forces. */
string repBodForces::CAT_TOTAL_USER = "total_user"; /**< Key for all user forces. */
string repBodForces::CAT_TOTAL_HYDRO = "total_hydro"; /**< Key for all hydro forces. */


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
repBodForces::repBodForces()
{
    //set default report name.
    this->setName("Force Report");
}

//------------------------------------------Function Separator --------------------------------------------------------
repBodForces::repBodForces(ofreq::ReportManager *ptIn) : repBodForces()
{
    this->setManager(ptIn);
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodForces::calcReport(int freqInd)
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

        //Let the model know to calculate true values
        ptMotion->CoefficientOnly() = false;

        //Reset, just for good measure.
        ptMotion->Reset();

        //Activate forces, as requested by user.
        //--------------------------------------
        if (pCategory == CAT_ACTIVE)
        {
            //Turn on both active forces.
            reqForceActive_user(pForceName);    //User force
            reqForceActive_hydro(pForceName);   //Hydro Force
        }

        if (pCategory == CAT_ACTIVE_USER)
        {
            //Turn on active forces, user
            reqForceActive_user(pForceName);
        }

        if (pCategory == CAT_ACTIVE_HYDRO)
        {
            //Turn on active forces, hydro
            reqForceActive_hydro(pForceName);
        }

        if (pCategory == CAT_MASS)
        {
            //Turn on mass forces
            reqForceMass();
        }

        if (pCategory == CAT_REACT)
        {
            //Turn on both reactive forces.
            reqForceReact_user(pForceName, pOrd);   //user force
            reqForceReact_hydro(pForceName, pOrd);  //hydro force
        }

        if (pCategory == CAT_REACT_USER)
        {
            //Turn on user reactive forces
            reqForceReact_user(pForceName, pOrd);
        }

        if (pCategory == CAT_REACT_HYDRO)
        {
            //Turn on hydro reactive forces
            reqForceReact_hydro(pForceName, pOrd);
        }

        if (pCategory == CAT_CROSS)
        {
            //Turn on both crossbody forces.
            reqForceCross_hydro(pForceName, pLinkBody, pOrd);   //Hydro force
            reqForceCross_user(pForceName, pLinkBody, pOrd);    //User force
        }

        if (pCategory == CAT_CROSS_USER)
        {
            //Turn on crossbody force, user
            reqForceCross_user(pForceName, pLinkBody, pOrd);
        }

        if (pCategory == CAT_CROSS_HYDRO)
        {
            //Turn on crossbody force, hydro
            reqForceCross_hydro(pForceName, pLinkBody, pOrd);
        }

        if (pCategory == CAT_TOTAL)
        {
            //Turn on all forces.

            //Turn on both active forces.
            reqForceActive_user(pForceName);    //User force
            reqForceActive_hydro(pForceName);   //Hydro Force

            //Turn on mass forces
            reqForceMass();

            //Turn on both reactive forces.
            reqForceReact_user(pForceName, pOrd);   //user force
            reqForceReact_hydro(pForceName, pOrd);  //hydro force

            //Turn on both crossbody forces.
            reqForceCross_hydro(pForceName, pLinkBody, pOrd);   //Hydro force
            reqForceCross_user(pForceName, pLinkBody, pOrd);    //User force
        }

        if (pCategory == CAT_TOTAL_USER)
        {
            //Turn on all forces, user.

            //Turn on both active forces.
            reqForceActive_user(pForceName);    //User force

            //Turn on both reactive forces.
            reqForceReact_user(pForceName, pOrd);   //user force

            //Turn on both crossbody forces.
            reqForceCross_user(pForceName, pLinkBody, pOrd);    //User force
        }

        if (pCategory == CAT_TOTAL_HYDRO)
        {
            //Turn on all forces, hydro.

            //Turn on both active forces.
            reqForceActive_hydro(pForceName);   //Hydro Force

            //Turn on both reactive forces.
            reqForceReact_hydro(pForceName, pOrd);  //hydro force

            //Turn on both crossbody forces.
            reqForceCross_user(pForceName, pLinkBody, pOrd);    //User force
        }

        plistData.push_back(Data(freqInd));

        //Calculate results and add to list of data.
        for (unsigned int i = 0; i < ptMotion->listEquation().size(); i++ )
        {
            plistData.back().addValue(
                        ptMotion->Evaluate(i));
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
void repBodForces::calcRAO(int freqInd)
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
string repBodForces::getClass()
{
    return "repBodForces";
}


//------------------------------------------Function Separator --------------------------------------------------------
string repBodForces::getFileName()
{
    return "bodyforces.out";
}


//==========================================Section Separator =========================================================
//Protected Functions



//==========================================Section Separator =========================================================
//Private Functions


//------------------------------------------Function Separator --------------------------------------------------------
void repBodForces::defineConst(int constIndex)
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
void repBodForces::reqForceMass()
{
    try
    {
        //Apply mass forces
        ptMotion->useForceMass();
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodForces::reqForceActive_user(std::string forceName)
{
    try
    {
        //Check if using one force, or all of them.
        if (forceName == "")
        {
            //Using all forces.
            ptMotion->useForceActive_user();
        }
        else
        {
            //Search through list of forces in body and use just one.
            for (unsigned int i = 0; i < ptBody->listForceActive_user().size(); i++)
            {
                if (forceName == ptBody->listForceActive_user(i)->getForceName())
                {
                    //Use the force.
                    ptMotion->useForceActive_user(i);
                    break;
                }
            }
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));  //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodForces::reqForceActive_hydro(std::string forceName)
{
    try
    {
        //Check if using one force, or all of them.
        if (forceName == "")
        {
            //Using all forces.
            ptMotion->useForceActive_hydro();
        }
        else
        {
            //Search through list of forces in body and use just one.
            for (unsigned int i = 0; i < ptBody->listForceActive_hydro().size(); i++)
            {
                if (forceName == ptBody->listForceActive_hydro(i)->getForceName())
                {
                    //Use the force.
                    ptMotion->useForceActive_hydro(i);
                    break;
                }
            }
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));  //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodForces::reqForceReact_user(std::string forceName, int ord)
{
    try
    {
        //Check if using one force, or all of them.
        if (forceName == "")
        {
            //Using all forces.

            //Check if using one order of derivative, or all of them.
            if (ord < 0)
            {
                //Using all orders of derivative.
                ptMotion->useForceReact_user();
            }
            else
            {
                //Just one order of derivative.

                //Iterate through each force, and apply order of derivative.
                for (unsigned int i = 0; i < ptBody->listForceReact_user().size(); i++)
                {
                    ptMotion->useForceReact_user(i, pOrd);
                }
            }
        }
        else
        {
            //Just one force.
            int forceIndex = -1;     //Index of the force object.

            //Search through list of forces in body and use just one.
            bool forceFound = false;
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
                throw std::runtime_error(string("No reactive force found which matches the given name.  Offending entry:  ") + forceName);
            }

            //Check if using one order of derivative, or all of them.
            if (ord < 0)
            {
                //Using all orders of derivative.
                ptMotion->useForceReact_user(forceIndex);
            }
            else
            {
                //Just one order of derivative.
                ptMotion->useForceReact_user(forceIndex, pOrd);
            }
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodForces::reqForceReact_hydro(std::string forceName, int ord)
{
    try
    {
        //Check if using one force, or all of them.
        if (forceName == "")
        {
            //Using all forces.

            //Check if using one order of derivative, or all of them.
            if (ord < 0)
            {
                //Using all orders of derivative.
                ptMotion->useForceReact_hydro();
            }
            else
            {
                //Just one order of derivative.

                //Iterate through each force, and apply order of derivative.
                for (unsigned int i = 0; i < ptBody->listForceReact_hydro().size(); i++)
                {
                    ptMotion->useForceReact_hydro(i, pOrd);
                }
            }
        }
        else
        {
            //Just one force.
            int forceIndex = -1;     //Index of the force object.

            //Search through list of forces in body and use just one.
            bool forceFound = false;
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
                throw std::runtime_error(string("No reactive force found which matches the given name.  Offending entry:  ") + forceName);
            }

            //Check if using one order of derivative, or all of them.
            if (ord < 0)
            {
                //Using all orders of derivative.
                ptMotion->useForceReact_hydro(forceIndex);
            }
            else
            {
                //Just one order of derivative.
                ptMotion->useForceReact_hydro(forceIndex, pOrd);
            }
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodForces::reqForceCross_user(std::string forceName, std::string linkName, int ord)
{
    try
    {
        //Check if using one force, or all of them.
        //-----------------------------------------
        if (forceName == "")
        {
            //Using all forces.

            //Check if using one linkedBody, or all of them.
            //-----------------------------------------
            if (linkName == "")
            {
                //Using all linked bodies.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    ptMotion->useForceCross_hydro();
                }
                else
                {
                    //Just one order of derivative.
                    for (unsigned int i = 0; i < ptBody->listCrossBody_hydro().size(); i++)
                    {
                        ptMotion->useForceCross_hydro(i, ord);
                    }
                }
            }
            else
            {
                //Just one linked body.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    vector<int> index = findLinkIndex("hydro", 2, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i));
                    }
                }
                else
                {
                    //Just one order of derivative.
                    vector<int> index = findLinkIndex("hydro", 2, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i), ord);
                    }
                }
            }
        }
        else
        {
            //Just one force.

            //Check if using one linkedBody, or all of them.
            //-----------------------------------------
            if (linkName == "")
            {
                //Using all linked bodies.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    vector<int> index = findLinkIndex("hydro", 1, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i));
                    }
                }
                else
                {
                    //Just one order of derivative.
                    vector<int> index = findLinkIndex("hydro", 1, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i), ord);
                    }
                }
            }
            else
            {
                //Just one linked body.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    vector<int> index = findLinkIndex("hydro", 0, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i));
                    }
                }
                else
                {
                    //Just one order of derivative.
                    vector<int> index = findLinkIndex("hydro", 0, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i), ord);
                    }
                }
            }
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void repBodForces::reqForceCross_hydro(std::string forceName, std::string linkName, int ord)
{
    try
    {
        //Check if using one force, or all of them.
        //-----------------------------------------
        if (forceName == "")
        {
            //Using all forces.

            //Check if using one linkedBody, or all of them.
            //-----------------------------------------
            if (linkName == "")
            {
                //Using all linked bodies.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    ptMotion->useForceCross_hydro();
                }
                else
                {
                    //Just one order of derivative.
                    for (unsigned int i = 0; i < ptBody->listCrossBody_hydro().size(); i++)
                    {
                        ptMotion->useForceCross_hydro(i, ord);
                    }
                }
            }
            else
            {
                //Just one linked body.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    vector<int> index = findLinkIndex("hydro", 2, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i));
                    }
                }
                else
                {
                    //Just one order of derivative.
                    vector<int> index = findLinkIndex("hydro", 2, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i), ord);
                    }
                }
            }
        }
        else
        {
            //Just one force.

            //Check if using one linkedBody, or all of them.
            //-----------------------------------------
            if (linkName == "")
            {
                //Using all linked bodies.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    vector<int> index = findLinkIndex("hydro", 1, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i));
                    }
                }
                else
                {
                    //Just one order of derivative.
                    vector<int> index = findLinkIndex("hydro", 1, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i), ord);
                    }
                }
            }
            else
            {
                //Just one linked body.

                //Check if using one order of derivative, or all of them.
                //-----------------------------------------
                if (ord < 0)
                {
                    //Using all orders of derivative.
                    vector<int> index = findLinkIndex("hydro", 0, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i));
                    }
                }
                else
                {
                    //Just one order of derivative.
                    vector<int> index = findLinkIndex("hydro", 0, forceName, linkName);

                    for (unsigned int i = 0; i < index.size(); i++)
                    {
                        ptMotion->useForceCross_hydro(index.at(i), ord);
                    }
                }
            }
        }
    }
    catch(const std::exception &err)
    {
        //Error handler.
        logStd.Notify();
        logErr.Write(ID + string(err.what()), 0);   //Throw an error.
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
std::vector<int> repBodForces::findLinkIndex(std::string typeForce, int match, std::string forceName, std::string bodName)
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
