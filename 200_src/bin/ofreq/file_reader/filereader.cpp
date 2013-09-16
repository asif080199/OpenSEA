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
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
FileReader::FileReader()
{
}

//------------------------------------------Function Separator --------------------------------------------------------
FileReader(string Path)
{
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

    //Read file
    return readFile(filename);

    //Emit signal of objects that were read
    for (int i = 0; i < plistObjects.size(); i++)
    {
        emit outputControlFile(plistObjects[i]);
    }
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

    //Read file
    return readFile(filename);

    //Emit signal of objects that were read
    for (int i = 0; i < plistObjects.size(); i++)
    {
        emit outputBodiesFile(plistObjects[i]);
    }
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

    //Read file
    return readFile(filename);

    //Emit signal of objects that were read
    for (int i = 0; i < plistObjects.size(); i++)
    {
        emit outputForcesFile(plistObjects[i]);
    }
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

    //Read file
    return readFile(filename);

    //Emit signal of objects that were read
    for (int i = 0; i < plistObjects.size(); i++)
    {
        emit outputSeaEnvFile(plistObjects[i]);
    }
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

    //Read file
    return readFile(filename);

    //Emit signal of objects that were read
    for (int i = 0; i < plistObjects.size(); i++)
    {
        emit outputDataFile(plistObjects[i]);
    }
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
    for (unsigned int i = sea_index; i < myParse.refObject().size(); i++)
    {
        //Check if the first object is an opensea file definition.
        if (myParse.refObject()[sea_index].getClassName() == OBJ_SEAFILE)
        {
            //True.  Process as a seafile object.
            for (int j = 0; j < myParse.refObject().at(sea_index).refListKey().size(); j++)
            {
                switch (myParse.refObject().at(sea_index).getKey(j))
                {
                case KEY_VERSION:
                    //Response if the key is the version number.
                    version = myParse.refObject().at(sea_index).getVal(j).at(0);
                    break;
                case KEY_FORMAT:
                    //Response if the key is the format designator.
                    format = myParse.refObject().at(sea_index).getVal(j).at(0);
                    break;
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

