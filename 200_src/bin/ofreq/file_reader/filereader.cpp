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
    //check if input has a slash at the end.
    //All functions assume no slash at the end.
    if (input[input.length() - 1] == SLASH[0])
    {
        //End slash found.  Remove it.
        input.erase(input.length() - 1, 1);
    }

    pPath = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readControl()
{
    //Read control input file

    //Set filename
    string filename;
    filename = pPath + SLASH + SYS + SLASH + CONTROL;

    //clear the list of objects.
    plistObjects.clear();

    //Emit signal of objects that were read
    for (unsigned int i = 0; i < plistObjects.size(); i++)
    {
        emit outputControlFile(plistObjects[i]);
    }

    //Read file
    return readFile(filename);
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readBodies()
{
    //Read Bodies input file

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + BODIES;

    //clear the list of objects.
    plistObjects.clear();

    //Emit signal of objects that were read
    for (unsigned int i = 0; i < plistObjects.size(); i++)
    {
        emit outputBodiesFile(plistObjects[i]);
    }

    //Read file
    int out = readFile(filename);

    for (unsigned int i; i < ptSystem->listBody().size(); i++)
    {
        //Link bodies for cross-body forces
        ptSystem->linkBodies(i);
    }

    //write output
    return out;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readForces()
{
    //Read forces input file

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + FORCES;

    //clear the list of objects.
    plistObjects.clear();

    //Emit signal of objects that were read
    for (unsigned int i = 0; i < plistObjects.size(); i++)
    {
        emit outputForcesFile(plistObjects[i]);
    }

    //Read file
    return readFile(filename);
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readSeaEnv()
{
    //Read sea environment input file

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + SEAENV;

    //clear the list of objects.
    plistObjects.clear();

    //Emit signal of objects that were read
    for (unsigned int i = 0; i < plistObjects.size(); i++)
    {
        emit outputSeaEnvFile(plistObjects[i]);
    }

    //Read file
    return readFile(filename);
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readData()
{
    //Read data input file

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + DATA;

    //clear the list of objects.
    plistObjects.clear();

    //Emit signal of objects that were read
    for (unsigned int i = 0; i < plistObjects.size(); i++)
    {
        emit outputDataFile(plistObjects[i]);
    }

    //Read file
    return readFile(filename);
}

//==========================================Section Separator =========================================================
//Signals

//==========================================Section Separator =========================================================
//Public Slots

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readHydroFile(string path)
{
    //Read hydrodynamic input file
    return readFile(path);
}

//------------------------------------------Function Separator --------------------------------------------------------
void FileReader::setSystem(System* ptInput)
{
    ptSystem = ptInput;
}

//==========================================Section Separator =========================================================
//Protected Functions

//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readFile(string path)
{
    //Read input file

    //Create input file
    ifstream InputFile(path.c_str());

    //Test if file exists
    if(!InputFile)
    {
        cerr << "control.in file does not exist." << endl;
        return 1;
        exit(1);
    }

    //Parse file
    Parser myParse;
    myParse.Parse(InputFile);

    //variables to record seafile parameters.
    string version;     //version of the sea file.
    string format;      //format of the sea file.
    int sea_index = 0;  //Index of where the sea file object is located.

    //Get results
    for (unsigned int i = sea_index; i < myParse.listObject().size(); i++)
    {
        //Check if the first object is an opensea file definition.
        if (myParse.listObject(sea_index).getClassName() == OBJ_SEAFILE)
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
            }
        }
        else
        {
            //False.  Process as normal file object.
            //Add objects to list.
            plistObjects.push_back(myParse.getObject(i));

            //Add version and format to object.
            plistObjects[i].setVersion(version);
            plistObjects[i].setFormat(format);
        }
    }

    //Report success of file reading.
    return 0;
}

//------------------------------------------Function Separator --------------------------------------------------------
void FileReader::initClass()
{
}
