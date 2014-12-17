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

#include "filereader.h"

using namespace std;
using namespace osea;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
// Directory Names
string FileReader::SYS = "system"; /**< The system directory name */
string FileReader::CONST = "constant"; /**< The constant directory name */

//------------------------------------------Function Separator ----------------------------------------------------
// Input File Names
string FileReader::CONTROL = "control.in"; /**< The filename for the control file. */
string FileReader::BODIES = "bodies.in"; /**< The filename for the bodies control file. */
string FileReader::DATA = "data.in"; /**< The filename for the data control file. */
string FileReader::FORCES = "forces.in"; /**< The filename for the forces control file. */
string FileReader::SEAENV = "seaenv.in"; /**< The filename for the sea environment control file. */
string FileReader::OUTPUTS = "outputs.in"; /**< The filename for the outputs control file. */

//------------------------------------------Function Separator ----------------------------------------------------
// Class Name Designators
string FileReader::OBJ_SYSTEM = "system"; /**< The string designation for a system object. */
string FileReader::OBJ_HYDROFILE = "hydrofiles"; /**< The string designation for a hydrofile object. */
string FileReader::OBJ_FORCE_ACTIVE = "force_active"; /**< The string designation for an active force object. */
string FileReader::OBJ_FORCE_REACT = "force_reactive"; /**< The string designation for a reactive force object. */
string FileReader::OBJ_FORCE_CROSS = "force_crossbody"; /**< The string designation for a cross-body force object.*/

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
FileReader::FileReader()
{
    initClass();
}

//------------------------------------------Function Separator --------------------------------------------------------
FileReader::FileReader(string Path)
{
    initClass();
    this->setPath(Path);
}

//------------------------------------------Function Separator --------------------------------------------------------
FileReader::~FileReader()
{
    //Destructor.
}

//------------------------------------------Function Separator --------------------------------------------------------
void FileReader::setPath(string input)
{
    //Reset all files and internal references.  Setting a new path is the equivalent of starting a new system.
    plistObjects.clear();

    //check if input has a slash at the end.
    //All functions assume no slash at the end.
    if (input.at(input.length() - 1) == SLASH.at(0))
    {
        //End slash found.  Remove it.
        input.erase(input.length() - 1, 1);
    }

    try
    {
        //Next check for a leading relative reference. (. or ..)
        //The system object must be set before reading the path.

        if (input.find(DDOT) != std::string::npos)
        {
            //Replace relative reference with absolute path.
            //First remove the dots.
            std::string::size_type iter = input.find(DDOT);
            input.erase(iter, iter + DDOT.length() + 1);

            //Check if the system object was not set.
            if (!ptSystem)
                throw std::runtime_error("Pointer to system object not set.");

            std::string workingDir = ptSystem->getPath();

            //Find position of last slash mark.
            iter = workingDir.find_last_of(SLASH);

            //Erase up to and including that slash mark.
            workingDir.erase(iter, std::string::npos);

            //Add the workingDir string onto the input.
            input = workingDir + SLASH + input;
        }
        else if (input.find(DOT) != std::string::npos)
        {
            //Replace relative reference with absolute path.
            //First remove the dots.
            std::string::size_type iter = input.find(DDOT);
            input.erase(iter, iter + DDOT.length() + 1);

            //Check if the system object was not set.
            if (!ptSystem)
                throw std::runtime_error("Pointer to system object not set.");

            std::string workingDir = ptSystem->getPath();

            //Add the workingDir string into the input.
            input = workingDir + SLASH + input;
        }
    }
    catch(const std::exception &err)
    {
        //Write error message and stop program execution.
        logStd.Notify();
        logErr.Write(ID + string(err.what()));
    }

    pPath = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readControl()
{
    //Read control input file
    logStd.Write("Reading control input file . . .",3);

    //Set filename
    string filename;
    filename = pPath + SLASH + SYS + SLASH + CONTROL;

    //clear the list of objects.
    plistObjects.clear();

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done.",3);

    //Write output
    return out;

    bool Stop = false;       //Boolean to record if warning should require stopping program execution.

    //Perform some sensibility checks to ensure program is sensible.
    try
    {
        //Check that the analysis type was set.
        //-------------------------------------
        if (ptSystem->getAnalysisType() == "")
        {
            //no analysis type set.  Throw an error.
            Stop = true;
            throw std::runtime_error("No Analsysis type set.  You must use the ""analysis"" keyword, under the system object.");
        }

        //Check that the analysis type selected is one of the valid options.
        //-------------------------------------
        std::vector<std::string> typeOptions;
        typeOptions.push_back("response");
        typeOptions.push_back("resonant");
        bool test = false;
        for (unsigned int i = 0; i < typeOptions.size(); i++)
        {
             if (typeOptions.at(i).compare(ptSystem->getAnalysisType()) == 0)
            {
                 test = true;
                 break;
            }
        }
        if(!test)
        {
            Stop = false;
            throw std::runtime_error("Set analysis type does not match list of possible types.");
        }

        //Check that there is at least one wave frequency defined.
        //-------------------------------------
        if (ptSystem->listWaveFrequencies().size() == 0)
        {
            Stop = true;
            throw std::runtime_error("System must have at least one wave frequency defined.");
        }

        //Check that there are at least two wave frequencies
        //Specific to hydrodynamic forces.
        //-------------------------------------
        if (ptSystem->listWaveFrequencies().size() < 2)
        {
            Stop = false;
            throw std::runtime_error("Hydrodynamic forces will require at least two wave frequencies defined.  This is necessary to calculate the wave amplitude from the supplied wave spectrum.");
        }

        //Check that there is at least on wave direction defined.
        //-------------------------------------
        if (ptSystem->listWaveDirections().size() == 0)
        {
            Stop = true;
            throw std::runtime_error("System must have at least one wave direction defined.");
        }
    }
    catch(const std::exception &err)
    {
        std::string file = CONTROL + " input file >> ";
        if (Stop)
        {
            //Critical error encountered.  Must stop program.
            logStd.Notify();
            logErr.Write(file + string(err.what()));
        }
        else
        {
            /* Do not stop program execution.
             * These sensibility checks are only considered warnings.
             * They are not guaranteed to produce errors, but deviate from typical program
             * behavior.
             * If the user knows what they are doing, this may be fine.
             */
            logStd.Notify(std::string("Warnings encountered.  Check the error log for more details."));
            logErr.Write(file + string(err.what()), 1);
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readBodies()
{
    //Read Bodies input file
    logStd.Write("Reading bodies input file . . .",3);

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + BODIES;

    //clear the list of objects.
    plistObjects.clear();

    //Read file
    int out = readFile(filename);

    for (unsigned int i; i < ptSystem->listBody().size(); i++)
    {
        //Link bodies for cross-body forces
        ptSystem->linkBodies(i);
    }

    logStd.Write(". . . done.",3);

    //write output
    return out;

    bool Stop = false;       //Boolean to record if warning should require stopping program execution.

    //Perform some sensibility checks to ensure program is sensible.
    try
    {
        //Check that least one body is defined.
        //-------------------------------------
        if (ptSystem->listBody().size() == 0)
        {
            Stop = true;
            throw std::runtime_error("At least one body must be defined.");
        }

        //Ensure that the body has a name.
        //-------------------------------------
        for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
        {
            if (ptSystem->listBody(i).getBodyName() == "")
            {
                Stop = true;
                throw std::runtime_error("At least one body definition is missing a name.  All body definitions must have the ""name"" keyword assigned.");
                break;
            }
        }

        //Check that mass properties were set.
        //-------------------------------------
        for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
        {
            if (ptSystem->listBody(i).getMass() == 0)
            {
                Stop = false;
                std::string msg = "No mass set on one of the bodies.  A default value of 0.0 kg was assigned.";
                msg = msg + " Body:  " + ptSystem->listBody(i).getBodyName();
                throw std::runtime_error(msg);
                break;
            }
        }

        //Check that center of gravity was set.
        //-------------------------------------
        for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
        {
            if ((ptSystem->listBody(i).getCenX() == 0)
                    && (ptSystem->listBody(i).getCenY() == 0)
                    && (ptSystem->listBody(i).getCenZ() == 0))
            {
                Stop = false;
                std::string msg = "The center of gravity was set at 0,0,0 in body coordinate system.";
                msg = msg + " Body:  " + ptSystem->listBody(i).getBodyName();
                throw std::runtime_error(msg);
                break;
            }
        }

        //Check that the body has a motion model set.
        //-------------------------------------
        for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
        {
            if (!&(ptSystem->listBody(i).getMotionModel()))
            {
                Stop = true;
                std::string msg = "No motion model set for one of the body definitions.";
                msg = msg + " Body:  " + ptSystem->listBody(i).getBodyName();
                throw std::runtime_error(msg);
                break;
            }
        }

        //Check that each crossbody force has a linked body specified.
        //-------------------------------------
        for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
        {
            //Assign body
            ofreq::Body *myBod = &(ptSystem->listBody(i));

            //Check that the two force sizes match.
            if (myBod->listCrossBody_user().size() != myBod->listForceCross_user().size())
            {
                Stop = true;
                std::string msg = "Number of linked bodies do not match number of crossbody forces defined.";
                msg = msg + "  Body in question:  " + myBod->getBodyName();
                throw std::runtime_error(msg);
            }
        }

        //Check that each linked body also has a user crossbody force back to the original body.
        //-------------------------------------
        for (unsigned int i = 0; i < ptSystem->listBody().size(); i++)
        {
            ofreq::Body *myBod;
            myBod = &(ptSystem->listBody(i));
            for (unsigned int j = 0; j < myBod->listCrossBody_user().size(); j++)
            {
                //Check that the assigned crossbody link also has a matching link.
                ofreq::Body *linkBod;
                linkBod = &(myBod->listCrossBody_user(j));
                //Search through linked body to see if it has a matching body.
                bool match = false;
                for (unsigned int k = 0; k < linkBod->listCrossBody_user().size(); k++)
                {
                    if (&(linkBod->listCrossBody_user(k)) == myBod)
                    {
                        match = true;
                        break;
                    }
                }

                //If no match found, throw a warning message.
                if (!match)
                {
                    Stop = false;
                    std::string msg = "No matching user crossbody force found for one of the bodies.  Did you intend to break Newton's third law of motion?";
                    msg = msg + " Body:  " + ptSystem->listBody(i).getBodyName();
                    throw std::runtime_error(msg);
                }
            }
        }
    }
    catch(const std::exception &err)
    {
        std::string file = BODIES + " input file >> ";
        if (Stop)
        {
            //Critical error encountered.  Must stop program.
            logStd.Notify();
            logErr.Write(file + string(err.what()));
        }
        else
        {
            /* Do not stop program execution.
             * These sensibility checks are only considered warnings.
             * They are not guaranteed to produce errors, but deviate from typical program
             * behavior.
             * If the user knows what they are doing, this may be fine.
             */
            logStd.Notify(std::string("Warnings encountered.  Check the error log for more details."));
            logErr.Write(file + string(err.what()), 1);
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readForces()
{
    //Read forces input file
    logStd.Write("Reading forces input file . . .",3);

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + FORCES;

    //clear the list of objects.
    plistObjects.clear();

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done.",3);

    //Write output
    return out;

    bool Stop = false;       //Boolean to record if warning should require stopping program execution.

    //Perform some sensibility checks to ensure program is sensible.
    try
    {
        //Check that active forces have a name assigned.
        //----------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listForceActive_user().size(); i++)
        {
            if (ptSystem->listForceActive_user(i).getForceName() == "")
            {
                Stop = true;
                std::string msg = "No name assigned to one or more force_active objects.";
                throw std::runtime_error(msg);
            }
        }

        //Check that reactive forces have a name assigned.
        //----------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listForceReact_user().size(); i++)
        {
            if (ptSystem->listForceReact_user(i).getForceName() == "")
            {
                Stop = true;
                std::string msg = "No name assigned to one or more force_reactive objects.";
                throw std::runtime_error(msg);
            }
        }

        //Check that crossbody forces have a name assigned.
        //----------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listForceCross_user().size(); i++)
        {
            if (ptSystem->listForceCross_user(i).getForceName() == "")
            {
                Stop = true;
                std::string msg = "No name assigned to one or more force_crossbody objects.";
                throw std::runtime_error(msg);
            }
        }

        //Check that active forces have at least one equation coefficient defined.
        //----------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listForceActive_user().size(); i++)
        {
            if (ptSystem->listForceActive_user(i).listDataEquation().size() == 0)
            {
                Stop = false;
                std::string msg = "A force_active object has no equation coefficients defined.  ";
                msg = msg + "Object name:  " + ptSystem->listForceActive_user(i).getForceName();
                throw std::runtime_error(msg);
            }
        }

        //Check that reactive forces have at least one equation coefficient defined.
        //----------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listForceReact_user().size(); i++)
        {
            if (ptSystem->listForceReact_user(i).listDerivative().size() == 0)
            {
                Stop = false;
                std::string msg = "A force_reactive object has no derivatives defined.  ";
                msg = msg + "Object name:  " + ptSystem->listForceReact_user(i).getForceName();
                throw std::runtime_error(msg);
            }

            //Check if derivative has any equations defined.
            for (unsigned int j = 0; j < ptSystem->listForceReact_user(i).listDerivative().size(); j++)
            {
                if (ptSystem->listForceReact_user(i).listDerivative(j).listEquation().size() == 0)
                {
                    Stop = false;
                    std::string msg = "A force_reactive object has no equations defined for a derivative.  ";
                    msg = msg + "Object name:  " + ptSystem->listForceReact_user(i).getForceName();
                    msg = msg + "  Derivative order:  " + itoa(j);
                    throw std::runtime_error(msg);
                }

                //Assign derivative
                ofreq::Derivative *myDeriv;
                myDeriv = &(ptSystem->listForceReact_user(i).listDerivative(j));

                //Check if equation has any coefficients assigned.
                for (unsigned int k = 0; k < myDeriv->listEquation().size(); k++)
                {
                    if (myDeriv->listEquation(k).listCoefficient().size() == 0)
                    {
                        Stop = false;
                        std::string msg = "A force_reactive object has no coefficients defined for an equation.  ";
                        msg = msg + "Object name:  " + ptSystem->listForceReact_user(i).getForceName();
                        msg = msg + "  Derivative order:  " + itoa(j);
                        msg = msg + "  Equation number:  " + itoa(myDeriv->listDataEquation(k).getDataIndex());
                        throw std::runtime_error(msg);
                    }
                }
            }
        }

        //Check that crossbody forces have at least one equation coefficient defined.
        //----------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listForceCross_user().size(); i++)
        {
            if (ptSystem->listForceCross_user(i).listDerivative().size() == 0)
            {
                Stop = false;
                std::string msg = "A force_reactive object has no derivatives defined.  ";
                msg = msg + "Object name:  " + ptSystem->listForceCross_user(i).getForceName();
                throw std::runtime_error(msg);
            }

            //Check if derivative has any equations defined.
            for (unsigned int j = 0; j < ptSystem->listForceCross_user(i).listDerivative().size(); j++)
            {
                if (ptSystem->listForceCross_user(i).listDerivative(j).listEquation().size() == 0)
                {
                    Stop = false;

                    std::string msg = "A force_reactive object has no equations defined for a derivative.  ";
                    msg = msg + "Object name:  " + ptSystem->listForceCross_user(i).getForceName();
                    msg = msg + "  Derivative order:  " + itoa(j);
                    throw std::runtime_error(msg);
                }

                //Assign derivative
                ofreq::Derivative *myDeriv;
                myDeriv = &(ptSystem->listForceCross_user(i).listDerivative(j));

                //Check if equation has any coefficients assigned.
                for (unsigned int k = 0; k < myDeriv->listEquation().size(); k++)
                {
                    if (myDeriv->listEquation(k).listCoefficient().size() == 0)
                    {
                        Stop = false;
                        std::string msg = "A force_reactive object has no coefficients defined for an equation.  ";
                        msg = msg + "Object name:  " + ptSystem->listForceCross_user(i).getForceName();
                        msg = msg + "  Derivative order:  " + itoa(j);
                        msg = msg + "  Equation number:  " + itoa(myDeriv->listEquation(k).getDataIndex());
                        throw std::runtime_error(msg);
                    }
                }
            }
        }
    }
    catch(const std::exception &err)
    {
        std::string file = FORCES + " input file >> ";
        if (Stop)
        {
            //Critical error encountered.  Must stop program.
            logStd.Notify();
            logErr.Write(file + string(err.what()));
        }
        else
        {
            /* Do not stop program execution.
             * These sensibility checks are only considered warnings.
             * They are not guaranteed to produce errors, but deviate from typical program
             * behavior.
             * If the user knows what they are doing, this may be fine.
             */
            logStd.Notify(std::string("Warnings encountered.  Check the error log for more details."));
            logErr.Write(file + string(err.what()), 1);
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readSeaEnv()
{
    //Read sea environment input file
    logStd.Write("Reading seaenv input file . . .",3);

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + SEAENV;

    //clear the list of objects.
    plistObjects.clear();   

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done",3);

    //Write output
    return out;

    bool Stop = false;       //Boolean to record if warning should require stopping program execution.

    //Perform some sensibility checks to ensure program is sensible.
    try
    {
        //Check that all wave spectrums have a name defined.
        //--------------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listWaveSpec().size(); i++)
        {
            if (ptSystem->listWaveSpec(i).getName() == "")
            {
                Stop = true;
                throw std::runtime_error("One or more wave spectrums were defined without a name.");
            }
        }

        //Check that all sea models have a name defined.
        //--------------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listSeaModel().size(); i++)
        {
            if (ptSystem->listSeaModel(i).getName() == "")
            {
                Stop = true;
                throw std::runtime_error("One or more sea models were defined without a name.");
            }
        }

        //Check settings on individual wave spectrums.
        //--------------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listWaveSpec().size(); i++)
        {
            //Assign wave spectrum to variable.
            osea::WaveSpecBase *mySpec;
            osea::SpecBretschneider *myBret;
            osea::SpecJONSWAP *myJON;
            osea::SpecPM *myPM;

            //Check on items for Bretschneider wave spectra.
            if (typeid(*(ptSystem->listWaveSpecPt(i))) == typeid(*myBret))
            {
                //Assign wave spectra.
                myBret = static_cast<osea::SpecBretschneider *>(ptSystem->listWaveSpecPt(i));

                //Check that the wave spectrum includes a significant wave height.
                //-------------------------------------------
                if (myBret->getSigWaveHeight() == 0)
                {
                    Stop = false;
                    std::string msg = "Significant wave height not set on wave spectrum.";
                    msg = msg + "  Spectrum name:  " + mySpec->getName();
                    throw std::runtime_error(msg);
                }

                //Check that the wave spectrum includes a wave period defined.
                //-------------------------------------------
                if (myBret->getTMean() == 0)
                {
                    Stop = false;
                    std::string msg = "Wave period not set on wave spectrum.";
                    msg = msg + "  Spectrum name:  " + mySpec->getName();
                    throw std::runtime_error(msg);
                }
            }

            //Check on items for JONSWAP wave spectra.
            if (typeid(*(ptSystem->listWaveSpecPt(i))) == typeid(*myJON))
            {
                //Assign wave spectra.
                myJON = static_cast<osea::SpecJONSWAP *>(ptSystem->listWaveSpecPt(i));

                //Check that the wave spectrum includes a significant wave height.
                //-------------------------------------------
                if (myJON->getSigWaveHeight() == 0)
                {
                    Stop = false;
                    std::string msg = "Significant wave height not set on wave spectrum.";
                    msg = msg + "  Spectrum name:  " + mySpec->getName();
                    throw std::runtime_error(msg);
                }

                //Check that the wave spectrum includes a wave period defined.
                //-------------------------------------------
                if (myJON->getTMean() == 0)
                {
                    Stop = false;
                    std::string msg = "Wave period not set on wave spectrum.";
                    msg = msg + "  Spectrum name:  " + mySpec->getName();
                    throw std::runtime_error(msg);
                }
            }

            //Check on items for Pierson-Moskowitz spectra.
            if (typeid(*(ptSystem->listWaveSpecPt(i))) == typeid(*myPM))
            {
                //Assign wave spectra
                myPM = static_cast<osea::SpecPM *>(ptSystem->listWaveSpecPt(i));

                //Check that it has at least a wind speed or wave height defined.
                //-------------------------------------------
                if ((myPM->getWindSpeed() == 0)
                        && (myPM->getSigWaveHeight() == 0))
                {
                    Stop = false;
                    std::string msg = "No significant wave height or wind speed defined for Pierson-Moskowitz spectra.";
                    msg = msg + "  Spectrum name:  " + mySpec->getName();
                    throw std::runtime_error(msg);
                }
            }
        }

        //Check that all sea models have at least one wave definition defined.
        //-------------------------------------------
        for (unsigned int i = 0; i < ptSystem->listSeaModel().size(); i++)
        {
            if (ptSystem->listSeaModel(i).listWaveSpec().size() == 0)
            {
                Stop = true;
                std::string msg = "Sea model defined without any wave spectra included.";
                msg = msg + "  Sea model name:  " + ptSystem->listSeaModel(i).getName();
                throw std::runtime_error(msg);
            }
        }
    }
    catch(const std::exception &err)
    {
        std::string file = SEAENV + " input file >> ";
        if (Stop)
        {
            //Critical error encountered.  Must stop program.
            logStd.Notify();
            logErr.Write(file + string(err.what()));
        }
        else
        {
            /* Do not stop program execution.
             * These sensibility checks are only considered warnings.
             * They are not guaranteed to produce errors, but deviate from typical program
             * behavior.
             * If the user knows what they are doing, this may be fine.
             */
            logStd.Notify(std::string("Warnings encountered.  Check the error log for more details."));
            logErr.Write(file + string(err.what()), 1);
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readData()
{
    //Read data input file
    logStd.Write("Reading data input file . . .",3);

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + DATA;

    //clear the list of objects.
    plistObjects.clear();

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done.",3);

    //Write output
    return out;

    bool Stop = false;       //Boolean to record if warning should require stopping program execution.

    //Perform some sensibility checks to ensure program is sensible.
    try
    {
        //Check if there were no hydrodynamic input files specified.
        //----------------------------------------------------------
        if (this->listDataFiles().size() == 0)
        {
            Stop = false;
            throw std::runtime_error("No hydrodynamic files listed.");
        }

    }
    catch(const std::exception &err)
    {
        std::string file = DATA + " input file >> ";
        if (Stop)
        {
            //Critical error encountered.  Must stop program.
            logStd.Notify();
            logErr.Write(file + string(err.what()));
        }
        else
        {
            /* Do not stop program execution.
             * These sensibility checks are only considered warnings.
             * They are not guaranteed to produce errors, but deviate from typical program
             * behavior.
             * If the user knows what they are doing, this may be fine.
             */
            logStd.Notify(std::string("Warnings encountered.  Check the error log for more details."));
            logErr.Write(file + string(err.what()), 1);
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readOutputs()
{
    //Read data input file
    logStd.Write("Reading outputs input file . . .",3);

    //Set filename
    string filename;
    filename = pPath + SLASH + SYS + SLASH + OUTPUTS;

    //clear the list of objects.
    plistObjects.clear();

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done.",3);

    //Write output
    return out;

    bool Stop = false;       //Boolean to record if warning should require stopping program execution.

    //Perform some sensibility checks to ensure program is sensible.
    for (unsigned int i = 0; i < ptSystem->refReportManager().listReport().size(); i++)
    {
        try
        {
            //Get a pointer to the specified report, to simplify code.
            ofreq::Report *ptRep = ptSystem->refReportManager().listReportPt(i);

            //Check that report has the ReportManager assigned.
            ptRep->setManager(&(ptSystem->refReportManager()));

            //Check that every output has a name assigned to it.
            if (ptRep->getName() == "")
                throw std::runtime_error("No name defined for one of the given reports.");

            //Check that every output has a body assigned to it.
            if (ptRep->getBody() == NULL)
                throw std::runtime_error(string("No body assigned to report:  ") + ptRep->getName());
        }
        catch(const std::exception &err)
        {
            std::string file = OUTPUTS + " input file >> ";
            if (Stop)
            {
                //Critical error encountered.  Must stop program.
                logStd.Notify();
                logErr.Write(file + string(err.what()));
            }
            else
            {
                /* Do not stop program execution.
                 * These sensibility checks are only considered warnings.
                 * They are not guaranteed to produce errors, but deviate from typical program
                 * behavior.
                 * If the user knows what they are doing, this may be fine.
                 */
                logStd.Notify(std::string("Warnings encountered.  Check the error log for more details."));
                logErr.Write(file + string(err.what()), 1);
            }
        }
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void FileReader::setDictionary(osea::Dictionary &dictIn)
{
    ptDict = &dictIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
vector<string> &FileReader::listDataFiles()
{
    return plistDataFiles;
}

//------------------------------------------Function Separator --------------------------------------------------------
string &FileReader::listDataFiles(int index)
{
    return plistDataFiles.at(index);
}


//==========================================Section Separator =========================================================
//Signals

//==========================================Section Separator =========================================================
//Public Slots

//------------------------------------------Function Separator --------------------------------------------------------
void FileReader::setSystem(ofreq::System* ptInput)
{
    ptSystem = ptInput;
}

//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
void FileReader::sendOutput(int index)
{
    ptDict->setObject(plistObjects.at(index));
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readFile(string path)
{
    //Read input file

    //Create input file
    ifstream InputFile(path.c_str());

    try
    {
        //Test if file exists
        if(!InputFile)
            throw std::ios_base::failure("Could not open file:  " + path);

        //Parse file
        Parser myParse;
        myParse.Parse(InputFile);

        //variables to record seafile parameters.
        string version;     //version of the sea file.
        string format;      //format of the sea file.
        string object;      //Name of the sea file object.
        int sea_index = 0;  //Index of where the sea file object is located.

        //Get results
        for (unsigned int i = sea_index; i < myParse.listObject().size(); i++)
        {
            //Check if the first object is an opensea file definition.
            if (myParse.listObject(i).getClassName().find(OBJ_SEAFILE) != std::string::npos)
            {
                //True.  Process as a seafile object.
                for (unsigned int j = 0; j < myParse.listObject(sea_index).listKey().size(); j++)
                {
                    if (myParse.listObject(sea_index).getKey(j) == KEY_VERSION)
                    {
                        //Response if the key is the version number.
                        version = myParse.listObject(sea_index).getVal(j).at(0);
                    }
                    else if(myParse.listObject(sea_index).getKey(j) == KEY_FORMAT)
                    {
                        //Response if the key is the format designator.
                        format = myParse.listObject(sea_index).getVal(j).at(0);
                    }
                    else if (myParse.listObject(sea_index).getKey(j) == KEY_OBJECT)
                    {
                        //Response if the key is the object designator.
                        object = myParse.listObject(sea_index).getVal(j).at(0);
                    }
                }
            }
            else
            {
                //False.  Process as normal file object.
                //Add objects to list.
                plistObjects.push_back(myParse.getObject(i));

                //Add version and format to object.
                plistObjects.at(i - 1).setVersion(version);     //Assumes the sea_index is 0
                plistObjects.at(i - 1).setFormat(format);       //Assumes the sea_index is 0
            }
        }

        //Check for any key sets contained in the parser that are not part of an object.
        if (myParse.listKey().size() != 0)
        {
            /*If any keysets were found, create a dummy object and add them all under one dummy object.
             *The dummy object will have the same name as the object name declared in the seafile object.
             */

            //Create a new object to add to the list.
            plistObjects.push_back(ObjectGroup());
            plistObjects.back().setClassName(object);   //Set object name
            plistObjects.back().setVersion(version);    //Set version
            plistObjects.back().setFormat(format);      //Set the format

            //Add the key sets.
            for (unsigned int j = 0; j < myParse.listKey().size(); j++)
            {
                plistObjects.back().addKeySet(myParse.listKey(j), myParse.listVal(j));
            }
        }
    }
    catch (const std::exception &err)
    {
        logErr.Write(ID + std::string(err.what()));
        logStd.Notify();
        return 1;

        //Stop program execution
        exit(1);
    }

    //Close file
    InputFile.close();

    //Emit signal of objects that were read
    for (unsigned int i = 0; i < plistObjects.size(); i++)
    {
        //emit outputSeaEnvFile(plistObjects.at(i));
        sendOutput(i);
    }

    //Report success of file reading.
    return 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
void FileReader::initClass()
{
}

//==========================================Section Separator =========================================================
//Private Functions
