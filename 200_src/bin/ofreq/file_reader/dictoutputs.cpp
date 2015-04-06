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
#include "dictoutputs.h"

using namespace std;
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Class Name constants
string dictOutputs::OBJECT_REPGLOBACCELERATION = "repGlobalAcceleration";
string dictOutputs::OBJECT_REPGLOBVELOCITY = "repGlobalVelocity";
string dictOutputs::OBJECT_REPGLOBMOTION = "repGlobalMotion";
string dictOutputs::OBJECT_REPGLOBSOLUTION = "repGlobalSolution";
string dictOutputs::OBJECT_REPLOCALACCELERATION = "repLocalAcceleration";
string dictOutputs::OBJECT_REPLOCALSOLUTION = "repLocalSolution";
string dictOutputs::OBJECT_REPWAVESPECTRA = "repWaveSpectra";
string dictOutputs::OBJECT_REPSTATICBODACCEL = "repStaticBodyAcceleration";
string dictOutputs::OBJECT_REPDESIGNACCEL = "repDesignAcceleration";
string dictOutputs::OBJECT_REPDYNFREEBOARD = "repDynamicFreeboard";
string dictOutputs::OBJECT_REPBODFORCES = "repBodyForce";
string dictOutputs::OBJECT_REPBODPOWER = "repBodyPower";
string dictOutputs::OBJECT_REPCAPWIDTH = "repCaptureWidth";
string dictOutputs::OBJECT_REPRELEFF = "repRelativeEfficiency";
string dictOutputs::OBJECT_REPABSEFF = "repAbsoluteEfficiency";
string dictOutputs::OBJECT_REPBODYACCELERATION = "repBodyAcceleration";
string dictOutputs::OBJECT_REPBODYVELOCITY = "repBodyVelocity";
string dictOutputs::OBJECT_REPBODYMOTION = "repBodyMotion";
string dictOutputs::OBJECT_REPBODYSOLUTION = "repBodSolution";

//------------------------------------------Function Separator ----------------------------------------------------
//Keyword name static constants
string dictOutputs::KEY_BODY = "body";    /**< Key to declare which body report is associated with. */
string dictOutputs::KEY_NAME = "name";    /**< Key to declare name of report object. */
string dictOutputs::KEY_RAO = "RAO";     /**< Key to calculate RAO or not. */

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
dictOutputs::dictOutputs()
{

}

//==========================================Section Separator =========================================================
//Signal Functions


//==========================================Section Separator =========================================================
//Public Slots Functions


//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int dictOutputs::defineKey(std::string keyIn, std::vector<std::string> valIn)
{
    //Assign properties to object.
    if (keyIn == KEY_BODY)
    {
        //Set body association.
        for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
        {
            if (ptSystem->listBody(i).getBodyName() == valIn.at(0))
            {
                //Set body
                ptRep->setBody(
                            &(ptSystem->listBody(i))
                            );
                break;
            }
        }

        //Also add key to list of constants.
        ptRep->listConstKey().push_back(keyIn);
        ptRep->listConstVal().push_back(ofreq::Data());

        ptRep->listConstVal().back().addString(valIn.at(0));

        //Report success
        return 0;
    }

    else if (keyIn == KEY_NAME)
    {
        //Set key name
        ptRep->setName(valIn.at(0));

        //Report success
        return 0;
    }

    else
    {
        //Simply add key to list of constants.      
        ptRep->listConstKey().push_back(keyIn);

        ptRep->listConstVal().push_back(ofreq::Data());

        for (unsigned int i = 0; i < valIn.size(); i++)
        {
            //First check if value is a complex variable.
            std::size_t index;
            bool test = false;
            index = valIn.at(i).find(BRACKET);
            if (index != std::string::npos)
                test = true | test;

            index = valIn.at(i).find(COMMA);
            if (index != std::string::npos)
                test = true | test;

            index = valIn.at(i).find(IMAGINARY,1);
            if (index != std::string::npos)
                test = true | test;

            //if true, add complex number.
            if (test)
            {
                ptRep->listConstVal().back().addValue(
                            convertComplex(
                                valIn.at(i)
                                )
                            );
                continue;
            }

            //next test to see if a string.
            if (valIn.at(i).find_first_not_of("0123456789.") != std::string::npos)
            {
                //Process as string
                ptRep->listConstVal().back().addString(
                            valIn.at(i));

                continue;
            }

            //Next test if double value or integer.
            double convDble = atof(valIn.at(i).c_str());
            if ((fmod(convDble,1) == 0)
                    && (valIn.at(i).find(".") == std::string::npos))
            {
                //Actually an integer operator.  Convert to integer and add as such.
                ptRep->listConstVal().back().addValue(
                            atoi(valIn.at(i).c_str()
                                 )
                            );
                continue;
            }
            else
            {
                //Actually a double.  Add to double.
                ptRep->listConstVal().back().addValue(
                            convDble);
                continue;
            }
        }

        //Report success
        return 0;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int dictOutputs::defineClass(std::string nameIn)
{
    if (nameIn == OBJECT_REPGLOBACCELERATION)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repGlobAcceleration(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPGLOBVELOCITY)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repGlobVelocity(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPGLOBMOTION)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repGlobMotion(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPGLOBSOLUTION)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repGlobSolution(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPLOCALSOLUTION)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repLocalSolution(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPLOCALACCELERATION)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repLocalAcceleration(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPWAVESPECTRA)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repWaveSpectra(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPSTATICBODACCEL)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repStaticBodyAccel(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPDESIGNACCEL)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repDesignAccel(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPDYNFREEBOARD)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repDynFreeboard(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPBODFORCES)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repBodForces(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager.
        ptSystem->refReportManager().addReport(ptRep);
    }

    else if (nameIn == OBJECT_REPBODPOWER)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repBodPower(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager.
        ptSystem->refReportManager().addReport(ptRep);
    }

    else if (nameIn == OBJECT_REPCAPWIDTH)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repCaptureWidth(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager.
        ptSystem->refReportManager().addReport(ptRep);
    }

    else if (nameIn == OBJECT_REPRELEFF)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repRelEff(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager.
        ptSystem->refReportManager().addReport(ptRep);
    }

    else if (nameIn == OBJECT_REPABSEFF)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repAbsEff(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager.
        ptSystem->refReportManager().addReport(ptRep);
    }

    if (nameIn == OBJECT_REPBODYACCELERATION)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repBodAcceleration(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPBODYVELOCITY)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repBodVelocity(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPBODYMOTION)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repBodMotion(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else if (nameIn == OBJECT_REPBODYSOLUTION)
    {
        //Create new Report object and add it to the Reports manager.
        ptRep = new repBodSolution(
                    &(ptSystem->refReportManager()));

        //Add report to Report manager
        ptSystem->refReportManager().addReport(ptRep);

        return 0;
    }

    else
    {
        //Return error
        return 1;
        throw std::runtime_error(string("Object declaration keyword not found.  Offending keyword:  ") + nameIn);
    }
}

//==========================================Section Separator =========================================================
//Private Functions
