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

#include "filewriter.h"

//==========================================Section Separator =========================================================
//Public Functions

//------------------------------------------Function Separator --------------------------------------------------------
FileWriter::FileWriter()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
FileWriter::FileWriter(string rootPath, OutputsBody &BodyIn)
{
    //Set the root path
    this->setProjectDir(rootPath);

    //Set the OutputsBody
    this->refOutputsBody() = BodyIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
FileWriter::~FileWriter()
{

}

//------------------------------------------Function Separator --------------------------------------------------------
void FileWriter::setProjectDir(string dirIn)
{
    char lastChar = dirIn.at(dirIn.length() - 1);
    if (lastChar == SLASH.at(0))
    {
        //Delete the slash
        dirIn.resize(dirIn.length() - 2);
    }

    //Set the value
    projectDirectory = dirIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::clearFiles()
{
    string numToDelete = "0";
    string curDirectoryPath = DIR_NAME + numToDelete; //start at directory "d0"

    //Remove the direcions & frequencies file outputs if they exist
    if(exists(projectDirectory + SLASH + DIRECTIONS_FILENAME))
        remove(projectDirectory + SLASH + DIRECTIONS_FILENAME);

    if(exists(projectDirectory + SLASH + FREQUENCIES_FILENAME))
        remove(projectDirectory + SLASH + FREQUENCIES_FILENAME);

    while(exists(projectDirectory + SLASH + curDirectoryPath)) //check if current directory exists
    {
        if(!remove_all(projectDirectory + SLASH + curDirectoryPath))
        {
            cerr << "Failed to delete " + projectDirectory + SLASH + curDirectoryPath << endl;
            return false;
        }
        else //increment to next directory
        {
            int numToDeleteHelper = boost::lexical_cast<int>(numToDelete);
            ++numToDeleteHelper;
            numToDelete = boost::lexical_cast<string>(numToDeleteHelper);
            curDirectoryPath = DIR_NAME + numToDelete;
        }
    }
    return true; //all directories deleted successfully
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody &FileWriter::refOutputsBody()
{
    return *pOutput;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::fileExists(string filename)
{
    //Checks if file exists.
    string wavePath;    //Directory of current wave path.
    ostringstream convert;      //Converts number to string.

    convert << getCurWaveInd();

    wavePath = DIR_NAME + convert.str();

    wavePath = wavePath + SLASH + filename;

    //Test if exists
    if (exists(wavePath))
        return true;
    else
        return false;
}

//--------------------------------------------Function Separator ------------------------------------------------------
//--------------------------------------------Function Separator ------------------------------------------------------
//Outputs writing methods below here
//--------------------------------------------Function Separator ------------------------------------------------------
//--------------------------------------------Function Separator ------------------------------------------------------

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::writeWaveDirection()
{
    ofstream output;
    string writeFilename;

    //Get filename
    writeFilename = projectDirectory + SLASH + DIRECTIONS_FILENAME;

    //Open file
    output.open(writeFilename.c_str());

    //Add top header
    output << header;

    //Add info block
    output << getInfoBlock(DIRECTION);
    output << BREAK_TOP;

    //Add beginning of data
    output << DIRECTION + SPACE + LIST_BEGIN2 << endl;

    //Write outputs
    for(unsigned int i = 0; i < pOutput->reflistWaveDir().size(); i ++)
    {
        output << TAB << pOutput->reflistWaveDir()[i] << endl;
    }

    //Close the list and finish
    output << END << LIST_END2  << "\n\n" << BREAK_BOTTOM;

    //Close file
    output.close();

    return true;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::writeFrequency()
{
    ofstream output;
    string writeFilename;

    //Get filename
    writeFilename = projectDirectory + SLASH + FREQUENCIES_FILENAME;

    //Open file
    output.open(writeFilename.c_str());

    //Add top header
    output << header;

    //Add info block
    output << getInfoBlock(FREQUENCY);
    output << BREAK_TOP;

    //Add beginning of data
    output << FREQUENCY + SPACE + LIST_BEGIN2 << endl;

    //Write outputs
    for(unsigned int i = 0; i < pOutput->reflistFreq().size(); i ++)
    {
        output << TAB << pOutput->reflistFreq().at(i) << endl;
    }

    //Close the list and finish
    output << END << LIST_END2  << "\n\n" << BREAK_BOTTOM;

    //Close file
    output.close();

    return true;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::writeGlobalMotion()
{
    string writeFilename;           //The name of the file to write out.
    string classname;               //The name of the class type that contains the file elements.

    //create directory.  Function will do nothing if directory already exists.
    createDir(getCurWaveDir());

    //create filename and open file
    writeFilename = projectDirectory + SLASH + getCurWaveDir() + GLOBAL_MOTION_FILENAME;
    ofstream output;

    //Check if file already exists.
    if (exists(writeFilename))
    {
        //Already exists.  Just open the file.
        //Open file
        output.open(writeFilename.c_str());
    }
    else
    {
        //Didn't already exist.  Create a new file and add the header information.
        //create file
        output.open(writeFilename.c_str());

        //Add top header
        output << header;

        //Get the name for the class written in output.
        classname.assign(typeid(pOutput->refGlobalMotion()).name());

        //Write the file info block.
        output << getInfoBlock(classname);
        output << BREAK_TOP;
    }

    //Write output for beginning of body
    output << BODY << SPACE << OBJECT_BEGIN2 << endl;
    output << NAME;
    output << SPACE;
    output << pOutput->refCurBody().BodyName();
    output << END << endl;

    //Repeat process for each item in object list
    for (int i = 0; i < pOutput->sizeGlobalMotion(); i++)
    {
        //Start the output object.
        output << TAB << classname << OBJECT_BEGIN2 << endl;
        output << TAB;
        output << NAME;
        output << SPACE;
        output << pOutput->refGlobalMotion(i).getName();
        output << END << endl;

        //Iterate for each frequency in the list
        for (unsigned int j = 0; j < pOutput->reflistFreq().size(); j++)
        {
            //Create data signifier
            output << TAB << TAB << DATA << SPACE << OBJECT_BEGIN2 << endl;
            //Add frequency designator
            output << TAB << TAB << FREQUENCY << SPACE << (j+1) << END << endl;
            //Add value indicator
            output << TAB << TAB << VALUE << SPACE << LIST_BEGIN2 << endl;
            for (unsigned int k = 0; j < pOutput->getGlobalMotion(i).size(); k++)
            {
                //Set precision
                output.precision(DIGIT);
                //Write output values for each solution object.
                //Write the real part
                output << TAB << TAB << TAB << pOutput->getGlobalMotion(i)[j][k].real();
                //Write the imaginary part
                if (pOutput->getGlobalMotion(i)[j][k].imag() < 0.0)
                    output << pOutput->getGlobalMotion(i)[j][k].imag() << "i" << endl;
                else
                    output << "+" << pOutput->getGlobalMotion(i)[j][k].imag() << "i" << endl;
            }
            //Close list
            output << TAB << TAB << LIST_END2 << endl;
            //Close data object
            output << TAB << TAB << OBJECT_END2 << endl;
        }
        //End the output object
        output << TAB << OBJECT_END2 << "\n";
    }

    //Write output for ending of body
    output << OBJECT_END2 << "\n" << BREAK_BOTTOM << "\n";

    //Close file
    output.close();

    //Report success
    return true;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::writeGlobalVelocity()
{
    string writeFilename;           //The name of the file to write out.
    string classname;               //The name of the class type that contains the file elements.

    //create directory.  Function will do nothing if directory already exists.
    createDir(getCurWaveDir());

    //create filename and open file
    writeFilename = projectDirectory + SLASH + getCurWaveDir() + GLOBAL_VELOCITY_FILENAME;
    ofstream output;

    //Check if file already exists.
    if (exists(writeFilename))
    {
        //Already exists.  Just open the file.
        //Open file
        output.open(writeFilename.c_str());
    }
    else
    {
        //Didn't already exist.  Create a new file and add the header information.
        //create file
        output.open(writeFilename.c_str());

        //Add top header
        output << header;

        //Get the name for the class written in output.
        classname.assign(typeid(pOutput->refGlobalVelocity()).name());

        //Write the file info block.
        output << getInfoBlock(classname);
        output << BREAK_TOP;
    }

    //Write output for beginning of body
    output << BODY << SPACE << OBJECT_BEGIN2 << endl;
    output << NAME;
    output << SPACE;
    output << pOutput->refCurBody().BodyName();
    output << END << endl;

    //Repeat process for each item in object list
    for (int i = 0; i < pOutput->sizeGlobalVelocity(); i++)
    {
        //Start the output object.
        output << TAB << classname << OBJECT_BEGIN2 << endl;
        output << TAB;
        output << NAME;
        output << SPACE;
        output << pOutput->refGlobalVelocity(i).getName();
        output << END << endl;

        //Iterate for each frequency in the list
        for (unsigned int j = 0; j < pOutput->reflistFreq().size(); j++)
        {
            //Create data signifier
            output << TAB << TAB << DATA << SPACE << OBJECT_BEGIN2 << endl;
            //Add frequency designator
            output << TAB << TAB << FREQUENCY << SPACE << (j+1) << END << endl;
            //Add value indicator
            output << TAB << TAB << VALUE << SPACE << LIST_BEGIN2 << endl;
            for (unsigned int k = 0; j < pOutput->getGlobalVelocity(i).size(); k++)
            {
                //Set precision
                output.precision(DIGIT);
                //Write output values for each solution object.
                //Write the real part
                output << TAB << TAB << TAB << pOutput->getGlobalVelocity(i)[j][k].real();
                //Write the imaginary part
                if (pOutput->getGlobalVelocity(i)[j][k].imag() < 0.0)
                    output << pOutput->getGlobalVelocity(i)[j][k].imag() << "i" << endl;
                else
                    output << "+" << pOutput->getGlobalVelocity(i)[j][k].imag() << "i" << endl;
            }
            //Close list
            output << TAB << TAB << LIST_END2 << endl;
            //Close data object
            output << TAB << TAB << OBJECT_END2 << endl;
        }
        //End the output object
        output << TAB << OBJECT_END2 << "\n";
    }

    //Write output for ending of body
    output << OBJECT_END2 << "\n" << BREAK_BOTTOM << "\n";

    //Close file
    output.close();

    //Report success
    return true;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::writeGlobalAcceleration()
{
    string writeFilename;           //The name of the file to write out.
    string classname;               //The name of the class type that contains the file elements.

    //create directory.  Function will do nothing if directory already exists.
    createDir(getCurWaveDir());

    //create filename and open file
    writeFilename = projectDirectory + SLASH + getCurWaveDir() + GLOBAL_ACCELERATION_FILENAME;
    ofstream output;

    //Check if file already exists.
    if (exists(writeFilename))
    {
        //Already exists.  Just open the file.
        //Open file
        output.open(writeFilename.c_str());
    }
    else
    {
        //Didn't already exist.  Create a new file and add the header information.
        //create file
        output.open(writeFilename.c_str());

        //Add top header
        output << header;

        //Get the name for the class written in output.
        classname.assign(typeid(pOutput->refGlobalAcceleration()).name());

        //Write the file info block.
        output << getInfoBlock(classname);
        output << BREAK_TOP;
    }

    //Write output for beginning of body
    output << BODY << SPACE << OBJECT_BEGIN2 << endl;
    output << NAME;
    output << SPACE;
    output << pOutput->refCurBody().BodyName();
    output << END << endl;

    //Repeat process for each item in object list
    for (int i = 0; i < pOutput->sizeGlobalAcceleration(); i++)
    {
        //Start the output object.
        output << TAB << classname << OBJECT_BEGIN2 << endl;
        output << TAB;
        output << NAME;
        output << SPACE;
        output << pOutput->refGlobalAcceleration(i).getName();
        output << END << endl;

        //Iterate for each frequency in the list
        for (unsigned int j = 0; j < pOutput->reflistFreq().size(); j++)
        {
            //Create data signifier
            output << TAB << TAB << DATA << SPACE << OBJECT_BEGIN2 << endl;
            //Add frequency designator
            output << TAB << TAB << FREQUENCY << SPACE << (j+1) << END << endl;
            //Add value indicator
            output << TAB << TAB << VALUE << SPACE << LIST_BEGIN2 << endl;
            for (unsigned int k = 0; j < pOutput->getGlobalAcceleration(i).size(); k++)
            {
                //Set precision
                output.precision(DIGIT);
                //Write output values for each solution object.
                //Write the real part
                output << TAB << TAB << TAB << pOutput->getGlobalAcceleration(i)[j][k].real();
                //Write the imaginary part
                if (pOutput->getGlobalAcceleration(i)[j][k].imag() < 0.0)
                    output << pOutput->getGlobalAcceleration(i)[j][k].imag() << "i" << endl;
                else
                    output << "+" << pOutput->getGlobalAcceleration(i)[j][k].imag() << "i" << endl;
            }
            //Close list
            output << TAB << TAB << LIST_END2 << endl;
            //Close data object
            output << TAB << TAB << OBJECT_END2 << endl;
        }
        //End the output object
        output << TAB << OBJECT_END2 << "\n";
    }

    //Write output for ending of body
    output << OBJECT_END2 << "\n" << BREAK_BOTTOM << "\n";

    //Close file
    output.close();

    //Report success
    return true;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::writeGlobalSolution()
{
    string writeFilename;           //The name of the file to write out.
    string classname;               //The name of the class type that contains the file elements.

    //create directory.  Function will do nothing if directory already exists.
    createDir(getCurWaveDir());

    //create filename and open file
    writeFilename = projectDirectory + SLASH + getCurWaveDir() + GLOBAL_SOLUTION_FILENAME;
    ofstream output;

    //Check if file already exists.
    if (exists(writeFilename))
    {
        //Already exists.  Just open the file.
        //Open file
        output.open(writeFilename.c_str());
    }
    else
    {
        //Didn't already exist.  Create a new file and add the header information.
        //create file
        output.open(writeFilename.c_str());

        //Add top header
        output << header;

        //Get the name for the class written in output.
        classname.assign(typeid(pOutput->refGlobalSolution()).name());

        //Write the file info block.
        output << getInfoBlock(classname);
        output << BREAK_TOP;
    }

    //Write output for beginning of body
    output << BODY << SPACE << OBJECT_BEGIN2 << endl;
    output << NAME;
    output << SPACE;
    output << pOutput->refCurBody().BodyName();
    output << END << endl;

    //Repeat process for each item in object list
    for (int i = 0; i < pOutput->sizeGlobalSolution(); i++)
    {
        //Start the output object.
        output << TAB << classname << OBJECT_BEGIN2 << endl;
        output << TAB;
        output << NAME;
        output << SPACE;
        output << pOutput->refGlobalSolution(i).getName();
        output << END << endl;

        //Iterate for each frequency in the list
        for (unsigned int j = 0; j < pOutput->reflistFreq().size(); j++)
        {
            //Create data signifier
            output << TAB << TAB << DATA << SPACE << OBJECT_BEGIN2 << endl;
            //Add frequency designator
            output << TAB << TAB << FREQUENCY << SPACE << (j+1) << END << endl;
            //Add value indicator
            output << TAB << TAB << VALUE << SPACE << LIST_BEGIN2 << endl;
            for (unsigned int k = 0; j < pOutput->getGlobalSolution(i).size(); k++)
            {
                //Set precision
                output.precision(DIGIT);
                //Write output values for each solution object.
                //Write the real part
                output << TAB << TAB << TAB << pOutput->getGlobalSolution(i)[j][k].real();
                //Write the imaginary part
                if (pOutput->getGlobalSolution(i)[j][k].imag() < 0.0)
                    output << pOutput->getGlobalSolution(i)[j][k].imag() << "i" << endl;
                else
                    output << "+" << pOutput->getGlobalSolution(i)[j][k].imag() << "i" << endl;
            }
            //Close list
            output << TAB << TAB << LIST_END2 << endl;
            //Close data object
            output << TAB << TAB << OBJECT_END2 << endl;
        }
        //End the output object
        output << TAB << OBJECT_END2 << "\n";
    }

    //Write output for ending of body
    output << OBJECT_END2 << "\n" << BREAK_BOTTOM << "\n";

    //Close file
    output.close();

    //Report success
    return true;
}

//==========================================Section Separator =========================================================
//Protected Functions

//------------------------------------------Function Separator --------------------------------------------------------
int FileWriter::getCurWaveInd()
{
    return pOutput->getCurWaveDir();
}

//------------------------------------------Function Separator --------------------------------------------------------
string FileWriter::getCurWaveDir()
{
    ostringstream convert;      //Converts number to string.

    convert << getCurWaveInd();

    return DIR_NAME + convert.str() + SLASH;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::createDir(string path)
{
    //Create the current directory
    if (!exists(projectDirectory + SLASH + path))
    {
        if (!create_directory(projectDirectory + SLASH + path))
        {
            cerr << "Failed to create " + projectDirectory + SLASH + path << endl; //This needs to be handled
            return false;
        }
        else
            return true;
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
void FileWriter::setHeader()
{
    ifstream header_fileInput(HEADER_FILENAME.c_str());

    if (!header_fileInput)
    {
        cerr << HEADER_FILENAME + " file does not exist." << endl;
    }
    else
    {
        header.assign((std::istreambuf_iterator<char>(header_fileInput)),
            (std::istreambuf_iterator<char>()));
    }
}

//------------------------------------------Function Separator --------------------------------------------------------
string FileWriter::getInfoBlock(string nameIn)
{
    return SEAFILE2 + "\n" + OBJECT_BEGIN2 + "\n    " + VERSION + "   " + VERSION_INFO + END + "\n    " + FORMAT + "    "
        + FORMAT_INFO + END + "\n    " + OBJECT + "    " + nameIn + END + "\n" + OBJECT_END2 + "\n\n";
}

//==========================================Section Separator =========================================================
//Private Functions
