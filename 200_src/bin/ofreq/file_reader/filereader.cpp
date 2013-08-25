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
FileReader::FileParser()
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

    //Read file
    return readFile(filename);
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileReader::readForces()
{
    //Read forces input file

    //Set filename
    string filename;
    filename = pPath + SLASH + CONST + SLASH + FORCES;

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

    //Read file
    return readFile(filename);
}

//==========================================Section Separator =========================================================
//Signals

//==========================================Section Separator =========================================================
//Public Slots

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

    //Get results
    for (unsigned int i = 0; i < myParse.refObject().size(); i++)
    {
        plistObjects.push_back(myParse.getObject(i));
    }

    //Report success of file reading.
    return 0;
}


