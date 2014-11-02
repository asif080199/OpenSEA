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

#include "hydroreader.h"

using namespace std;
using namespace osea;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator --------------------------------------------------------
// Directory Names
std::string HydroReader::DIR = "d";  /**< The letter prefix for directory names of wave direction directories. */

//------------------------------------------Function Separator --------------------------------------------------------
// Input File Names
std::string HydroReader::DIRECTIONS = "directions.out"; /**< The filename for the wave directions file. */
std::string HydroReader::ENVIRONMENT = "environment.out"; /**< The filename for the wave environmental conditions file. */
std::string HydroReader::FREQUENCIES = "frequencies.out"; /**< The filename for the wave frequencies file. */
std::string HydroReader::FORCEEXCITE = "forceexcite.out"; /**< THe filename for the wave excitation forces file. */
std::string HydroReader::HYDROMASS = "hydromass.out"; /**< The filename for the added mass forces file. */
std::string HydroReader::HYDRODAMP = "hydrodamp.out"; /**< The filename for the added damping forces file. */
std::string HydroReader::HYDROSTIFF = "hydrostiffness.out"; /**< The filename for the added stiffness forces file. */
std::string HydroReader::CROSSMASS = "crossmass.out"; /**< The filename for the crossbody mass forces file. */
std::string HydroReader::CROSSDAMP = "crossdamp.out"; /**< The filename for the crossbody damping forces file. */
std::string HydroReader::CROSSSTIFFNESS = "crossstiffness.out"; /**< The filename for the crossbody stiffness forces file. */

//------------------------------------------Function Separator --------------------------------------------------------
// Class Name Designators
std::string HydroReader::OBJ_DIRECTIONS = "direction"; /**< The designation for the wave directions object. */
std::string HydroReader::OBJ_ENVIRONMENT = "hydroenv"; /**< The designation for the wave environmental conditions object. */
std::string HydroReader::OBJ_FREQUENCIES = "frequency"; /**< The designation for the wave frequencies object. */
std::string HydroReader::OBJ_FORCEEXCITE = "forcexcite"; /**< THe designation for the wave excitation forces object. */
std::string HydroReader::OBJ_HYDROMASS = "hydromass"; /**< The designation for the added mass forces object. */
std::string HydroReader::OBJ_HYDRODAMP = "hydrodamp"; /**< The designation for the added damping forces object. */
std::string HydroReader::OBJ_HYDROSTIFF = "hydrostiff"; /**< The designation for the added stiffness forces object. */
std::string HydroReader::OBJ_CROSSMASS = "crossmass"; /**< The designation for the crossbody mass forces object. */
std::string HydroReader::OBJ_CROSSDAMP = "crossdamp"; /**< The designation for the crossbody damping forces object. */
std::string HydroReader::OBJ_CROSSSTIFFNESS = "crossstiff"; /**< The designation for the crossbody stiffness forces object. */

//------------------------------------------Function Separator --------------------------------------------------------
// Key Value Pair Designators


//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
HydroReader::HydroReader()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readHydroSys()
{
    //Start by clearing out all previous files.
    plistObjects.clear();
    plistTempHydro.clear();
    plistWaveDir.clear();
    plistWaveFreq.clear();

    try {
        //Write output to user.
        logStd.Write(string("    ") + pPath + string(". . . "),3);

        //First read the top level files
        //Read environment file
        readEnvironment();

        //Read directions file
        readDirections();

        //Read frequencies file
        readFrequencies();

        //Next Iterate through each of the wave directions and read those files.
        for (WaveInd = 0; WaveInd < plistWaveDir.size(); WaveInd++)
        {
            //Read the hydro mass file
            readHydroMass(WaveInd);

            //Read the hydro damping file
            readHydroDamp(WaveInd);

            //Read the hydro stiffness file
            readHydroStiff(WaveInd);

            //Read the cross mass file
            readCrossMass(WaveInd);

            //Read the cross damping file
            readCrossDamp(WaveInd);

            //Read the cross stiffness file
            readCrossStiff(WaveInd);

            //Read the active forces file
            readForceExcite(WaveInd);

            //Done reading in data.  Transfer the data from temporary holding to final values.
            //Each hydrodata object is its own hydrobody.
            for (unsigned int j = 0; j < plistTempHydro.size(); j++)
            {
                //Find index of the hydrobody
                int bod = findHydroManager(plistTempHydro.at(j).getHydroBodyName());

                if (bod < 0)
                {
                    //No hydrobody found.  Create a new one.
                    ptSystem->addHydroManager();
                    bod = ptSystem->listHydroManager().size() - 1;

                    //Set the body name.
                    ptSystem->listHydroManager(bod).setHydroBodyName(
                                plistTempHydro.at(j).getHydroBodyName());

                    //Set gravity for the body.
                    ptSystem->listHydroManager(bod).setGravity(pGravity);
                }

                //Find the index of the hydrodata set that matches the wave amplitude.
                int amp = findHydroDataAmp(bod,
                                           plistTempHydro.at(j).getWaveAmp());

                if (amp < 0)
                {
                    //No hydrodata set with that wave amplitude was found.  Add the data set as a new one.
                    ptSystem->listHydroManager(bod).addHydroData(
                                plistTempHydro.at(j));
                }
                else
                {
                    //Hydrodata set was found.  Add a new direction to that part of the list.
                    ptSystem->listHydroManager(bod).addHydroData(
                                amp,
                                plistTempHydro.at(j));
                }
            }

            //Clear the list of hydrodata
            plistTempHydro.clear();
        }

        //Write output to user.
        logStd.Write("    . . . done.",3);
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));   
    }
}


//==========================================Section Separator =========================================================
//Public Slots


//==========================================Section Separator =========================================================
//Signals



//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readEnvironment()
{
    try
    {
        //Create dictionary object
        osea::dictHydroEnv dictEnv(this);

        //Create pointer to system object.
        dictEnv.setSystem(this->ptSystem);

        //Set the dictionary for the filereader.
        this->setDictionary(dictEnv);

        //Set the filename
        std::string filename;
        filename = pPath + SLASH + ENVIRONMENT;

        //Clear the list of objects.
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));   
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readDirections()
{
    try
    {
        //Create dictinary object
        osea::dictHydroDirection dictDir(this);

        //Create pointer to the system object.
        dictDir.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictDir);

        //Set the filename
        std::string filename;
        filename = pPath + SLASH + DIRECTIONS;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));   
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readFrequencies()
{
    try
    {
        //Create dictionary object
        osea::dictHydroFrequency dictFreq(this);

        //Create pointer to the system object.
        dictFreq.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictFreq);

        //Set the filename
        std::string filename;
        filename = pPath + SLASH + FREQUENCIES;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));   
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readForceExcite(int index)
{
    try
    {
        //Create dictionary object
        osea::dictForceExcite dictForce(this);

        //Create pointer to the system object.
        dictForce.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictForce);

        //Set the filename
        std::string filename;
        ostringstream convert;
        convert << index + 1;   //Convert to switch to human counting system (start 1).
        filename = pPath + SLASH + DIR + convert.str() + SLASH + FORCEEXCITE;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));   
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readHydroMass(int index)
{
    try
    {
        //Create dictionary object
        osea::dictHydroMass dictMass(this);

        //Create pointer to the system object.
        dictMass.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictMass);

        //Set the filename
        std::string filename;
        ostringstream convert;
        convert << index + 1;   //Convert to switch to human counting system (start 1).
        filename = pPath + SLASH + DIR + convert.str() + SLASH + HYDROMASS;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));   
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readHydroDamp(int index)
{
    try
    {
        //Create dictionary object
        osea::dictHydroDamp dictDamp(this);

        //Create pointer to the system object.
        dictDamp.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictDamp);

        //Set the filename
        std::string filename;
        ostringstream convert;
        convert << index + 1;   //Convert to switch to human counting system (start 1).
        filename = pPath + SLASH + DIR + convert.str() + SLASH + HYDRODAMP;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readHydroStiff(int index)
{
    try
    {
        //Create dictionary object
        osea::dictHydroStiff dictStiff(this);

        //Create pointer to the system object.
        dictStiff.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictStiff);

        //Set the filename
        std::string filename;
        ostringstream convert;
        convert << index + 1;   //Convert to switch to human counting system (start 1).
        filename = pPath + SLASH + DIR + convert.str() + SLASH + HYDROSTIFF;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));    
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readCrossMass(int index)
{
    try
    {
        //Create dictionary object
        osea::dictCrossMass dictCMass(this);

        //Create pointer to the system object.
        dictCMass.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictCMass);

        //Set the filename
        std::string filename;
        ostringstream convert;
        convert << index + 1;   //Convert to switch to human counting system (start 1).
        filename = pPath + SLASH + DIR + convert.str() + SLASH + CROSSMASS;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));   
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readCrossDamp(int index)
{
    try
    {
        //Create dictionary object
        osea::dictCrossDamp dictCDamp(this);

        //Create pointer to the system object.
        dictCDamp.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictCDamp);

        //Set the filename
        std::string filename;
        ostringstream convert;
        convert << index + 1;   //Convert to switch to human counting system (start 1).
        filename = pPath + SLASH + DIR + convert.str() + SLASH + CROSSDAMP;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));   
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::readCrossStiff(int index)
{
    try
    {
        //Create dictionary object
        osea::dictCrossStiff dictCStiff(this);

        //Create pointer to the system object.
        dictCStiff.setSystem(this->ptSystem);

        //Set the dictionary for the filereader
        this->setDictionary(dictCStiff);

        //Set the filename
        std::string filename;
        ostringstream convert;
        convert << index + 1;   //Convert to switch to human counting system (start 1).
        filename = pPath + SLASH + DIR + convert.str() + SLASH + CROSSSTIFFNESS;

        //Clear the list of objects
        plistObjects.clear();

        //Read the file
        int out = readFile(filename);

        //clear the dictionary setting
        this->ptDict = NULL;

        //Write output
        return out;
    }
    catch(const std::exception &err)
    {
        logStd.Notify();
        logErr.Write(ID + std::string(err.what()));     
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::findHydroManager(std::string BodyName)
{
    bool found = false;
    unsigned int i;

    //Search through the list of HydroManagers defined in the system.
    for (i = 0; i < ptSystem->listHydroManager().size(); i++)
    {
        if (ptSystem->listHydroManager(i).getHydroBodyName().compare(BodyName) == 0)
        {
            found = true;
            break;
        }
    }

    if (found)
        return i;
    else
        return -1;
}

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::findHydroDataAmp(int BodyInd, double waveAmpIn)
{
    bool found = false;
    unsigned int i;

    //Search through the list of HydroData objects.
    if (ptSystem->listHydroManager(BodyInd).listHydroData().size() == 0)
    {
        //No hydro data set yet.
        return -1;
    }
    else
    {
        for (i = 0; i< ptSystem->listHydroManager(BodyInd).listHydroData().size(); i++)
        {
            if (ptSystem->listHydroManager(BodyInd).listHydroData(i,0).getWaveAmp() == waveAmpIn)
            {
                found = true;
                break;
            }
        }
    }

    if (found)
        return i;
    else
        return -1;
}


//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
int HydroReader::findHydroDataTemp(std::string BodyName)
{
    bool found = false;
    unsigned int i;

    //Search through the list of hydrodata objects.
    for (i = 0; i < plistTempHydro.size(); i++)
    {
        if (plistTempHydro.at(i).getHydroBodyName().compare(BodyName) == 0)
        {
            found = true;
            break;
        }
    }

    if (found)
        return i;
    else
    {
        //Create a new hydrodata object and populate it.
        plistTempHydro.push_back(ofreq::hydroData());

        i = plistTempHydro.size() - 1;

        //set wave amplitude.
        plistTempHydro.at(i).setWaveAmp(pWaveAmp);

        //Set wave direction
        plistTempHydro.at(i).setWaveDir(plistWaveDir.at(WaveInd));

        //Set the name of the hydrobody
        plistTempHydro.at(i).setHydroBodyName(BodyName);

        //Set the water depth
        plistTempHydro.at(i).setDepth(pDepth);

        //Set the water density
        plistTempHydro.at(i).setDensity(pDensity);

        //Set the list of wave frequencies.
        plistTempHydro.at(i).addWaveFreq(plistWaveFreq);

        //Return index
        return i;
    }
}








