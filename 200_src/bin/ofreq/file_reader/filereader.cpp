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
#ifdef Q_OS_WIN
    string FileReader::SLASH = "\\";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    string FileReader::SLASH = "/";   /**< Directory separator in a string path., linux version**/
#endif

//------------------------------------------Function Separator ----------------------------------------------------
// Input File Names
string FileReader::CONTROL = "control.in"; /**< The filename for the control file. */
string FileReader::BODIES = "bodies.in"; /**< The filename for the bodies control file. */
string FileReader::DATA = "data.in"; /**< The filename for the data control file. */
string FileReader::FORCES = "forces.in"; /**< The filename for the forces control file. */
string FileReader::SEAENV = "seaenv.in"; /**< The filename for the sea environment control file. */

//------------------------------------------Function Separator ----------------------------------------------------
// Class Name Designators
string FileReader::OBJ_SEAFILE = "seafile"; /**< The string designation for a sea file object. */
string FileReader::OBJ_SYSTEM = "system"; /**< The string designation for a system object. */
string FileReader::OBJ_HYDROFILE = "hydrofiles"; /**< The string designation for a hydrofile object. */
string FileReader::OBJ_FORCE_ACTIVE = "force_active"; /**< The string designation for an active force object. */
string FileReader::OBJ_FORCE_REACT = "force_reactive"; /**< The string designation for a reactive force object. */
string FileReader::OBJ_FORCE_CROSS = "force_crossbody"; /**< The string designation for a cross-body force object.*/

// ---------------------------------
// Key Value Pair Designators
string FileReader::KEY_FORMAT = "format";  /**< The key designator for a format value in the seafile object. */
string FileReader::KEY_VERSION = "version"; /**< The key designator for a version value in the seafile object. */
string FileReader::KEY_OBJECT = "object";  /**< The key designator for a object value in the seafile object. */

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
        logStd.Notify();
        logErr.Write(ID + err.what());

        //Stop program execution
        exit(1);
    }

    pPath = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readControl()
{
    //Read control input file
    logStd.Write("Reading control input file . . .");

    //Set filename
    string filename;
    filename = pPath + SLASH + SYS + SLASH + CONTROL;

    //clear the list of objects.
    plistObjects.clear();

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done.");

    //Write output
    return out;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readBodies()
{
    //Read Bodies input file
    logStd.Write("Reading bodies input file . . .");

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

    logStd.Write(". . . done.");

    //write output
    return out;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readForces()
{
    //Read forces input file
    logStd.Write("Reading forces input file . . .");

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + FORCES;

    //clear the list of objects.
    plistObjects.clear();

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done.");

    //Write output
    return out;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readSeaEnv()
{
    //Read sea environment input file
    logStd.Write("Reading seaenv input file . . .");

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + SEAENV;

    //clear the list of objects.
    plistObjects.clear();   

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done");

    //Write output
    return out;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readData()
{
    //Read data input file
    logStd.Write("Reading data input file . . .");

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + DATA;

    //clear the list of objects.
    plistObjects.clear();

    //Read file
    int out = readFile(filename);

    logStd.Write(". . . done.");

    //Write output
    return out;
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
