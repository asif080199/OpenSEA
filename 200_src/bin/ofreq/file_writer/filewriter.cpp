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

using namespace std;
using namespace boost::filesystem; //for using boost file system
using namespace osea::ofreq;

//==========================================Section Separator =========================================================
//Static Initialization

//------------------------------------------Function Separator ----------------------------------------------------
//Filename Specification
string FileWriter::HEADER_FILENAME = "openseaheader.txt";

//------------------------------------------Function Separator ----------------------------------------------------
//Directory Specifications
string FileWriter::NAME_DIR = "d"; /**< The starting character for the wave direction directory.**/
string FileWriter::NAME_RES = "r"; /**< The starting character for the resonant frequency directory.**/
#ifdef Q_OS_WIN
    string FileWriter::SLASH = "\\";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    string FileWriter::SLASH = "/";   /**< Directory separator in a string path., linux version**/
#endif

//------------------------------------------Function Separator ----------------------------------------------------
//General File Content Marks
string FileWriter::LIST_BEGIN2 = "(";
string FileWriter::LIST_END2 = ")";
string FileWriter::OBJECT_BEGIN2 = "{";
string FileWriter::OBJECT_END2 = "}";
string FileWriter::END = ";";
string FileWriter::TAB_REF = "  ";
string FileWriter::SPACE = " ";
string FileWriter::QUOTE = "\"";
string FileWriter::KEY_NAME = "name";
string FileWriter::KEY_DATA = "data";
string FileWriter::KEY_VALUE = "value";
string FileWriter::KEY_FREQUENCY = "frequency";
string FileWriter::KEY_DIRECTION = "direction";
string FileWriter::KEY_VERSION = "version";
string FileWriter::KEY_FORMAT = "format";
string FileWriter::KEY_OBJECT = "object";
string FileWriter::KEY_BODY = "body";
string FileWriter::VAL_VERSION = "1.0";
string FileWriter::VAL_FORMAT = "ascii";
string FileWriter::VAL_SEAFILE = "seafile";
string FileWriter::BREAK_TOP = "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //\n";
string FileWriter::BREAK_BOTTOM = "// ************************************************************************* //";
int FileWriter::DIGIT = 6; /** The number of digits to use in precision of floating point numbers.*/

//==========================================Section Separator =========================================================
//Filename Markers
string FileWriter::FILE_DIRECTIONS = "directions.out";
string FileWriter::FILE_FREQUENCIES = "frequencies.out";
string FileWriter::FILE_GLOBAL_ACCELERATION = "accglobal.out";
string FileWriter::FILE_GLOBAL_MOTION = "motglobal.out";
string FileWriter::FILE_GLOBAL_VELOCITY = "velglobal.out";
string FileWriter::FILE_GLOBAL_SOLUTION = "solglobal.out";

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
    setOutputsBody(&BodyIn);
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
        dirIn.resize(dirIn.length() - 1);
    }

    //Set the value
    projectDirectory = dirIn;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::clearFiles()
{
    string numToDelete = "0";
    string curDirectoryPath = NAME_DIR + numToDelete; //start at directory "d0"

    //Remove the direcions & frequencies file outputs if they exist
    if(exists(projectDirectory + SLASH + FILE_DIRECTIONS))
        remove(projectDirectory + SLASH + FILE_DIRECTIONS);

    if(exists(projectDirectory + SLASH + FILE_FREQUENCIES))
        remove(projectDirectory + SLASH + FILE_FREQUENCIES);

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
            curDirectoryPath = NAME_DIR + numToDelete;
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
void FileWriter::setOutputsBody(OutputsBody *input)
{
    pOutput = input;
}

//------------------------------------------Function Separator --------------------------------------------------------
OutputsBody *FileWriter::getOutputsBody()
{
    return pOutput;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::fileExists(string filename)
{
    //Checks if file exists.
    string wavePath;    //Directory of current wave path.
    ostringstream convert;      //Converts number to string.

    convert << getCurWaveInd();

    wavePath = NAME_DIR + convert.str();

    wavePath = wavePath + SLASH + filename;

    //Test if exists
    if (exists(wavePath))
        return true;
    else
        return false;
}

//------------------------------------------Function Separator --------------------------------------------------------
void FileWriter::setHeader(string filePathIn)
{
    std::string fileIn;     //Full path of input file that contains the header to write.

    //Combine file path and filename.
    fileIn = filePathIn;

    if (fileIn.substr(fileIn.size(),1) == SLASH)
    {
        //Just add filename
        fileIn.append(HEADER_FILENAME);
    }
    else
    {
        //Add filename plus slash.
        fileIn.append(SLASH);
        fileIn.append(HEADER_FILENAME);
    }

    ifstream header_fileInput(fileIn.c_str(), std::ios::in);

    if (!header_fileInput)
    {
        cerr << fileIn + ":  file does not exist." << endl;
    }
    else
    {
        header_fileInput.seekg(0, std::ios::end);
        header.resize(header_fileInput.tellg());
        header_fileInput.seekg(0, std::ios::beg);
        header_fileInput.read(&header[0], header.size());
        //Close file
        header_fileInput.close();
    }
}

//--------------------------------------------Function Separator ------------------------------------------------------
//--------------------------------------------Function Separator ------------------------------------------------------
//Outputs writing methods below here
//--------------------------------------------Function Separator ------------------------------------------------------
//--------------------------------------------Function Separator ------------------------------------------------------

//------------------------------------------Function Separator --------------------------------------------------------
int FileWriter::writeWaveDirection()
{
    ofstream output;
    string writeFilename;
    int errVal = 0;             //Output error code.  Default value set to no error.

    //Get filename
    writeFilename = projectDirectory + SLASH + FILE_DIRECTIONS;

    try
    {
        if (!exists(writeFilename))
        {
            //File does not exist.  Create file.
            //Open file
            output.open(writeFilename.c_str());

            //Add top header
            output << header;

            //Add info block
            output << getInfoBlock(KEY_DIRECTION);
            output << BREAK_TOP;

            //Add beginning of data
            output << KEY_DIRECTION + SPACE + LIST_BEGIN2 << endl;

            //Write outputs
            for(unsigned int i = 0; i < pOutput->listWaveDir().size(); i ++)
            {
                output << TAB() << pOutput->listWaveDir(i) << endl;
            }

            //Close the list and finish
            output << LIST_END2 << END << "\n\n" << BREAK_BOTTOM;

            //Close file
            output.close();
        }
    }
    catch (int err)
    {
        //Add error handler later
        errVal = err;
    }

    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileWriter::writeFrequency()
{
    ofstream output;
    string writeFilename;
    int errVal = 0;             //Output error code.  Default value set to no error.

    //Get filename
    writeFilename = projectDirectory + SLASH + FILE_FREQUENCIES;

    try
    {
        if (!exists(writeFilename))
        {
            //File does not exist.  Create.
            //Open file
            output.open(writeFilename.c_str());

            //Add top header
            output << header;

            //Add info block
            output << getInfoBlock(KEY_FREQUENCY);
            output << BREAK_TOP;

            //Add beginning of data
            output << KEY_FREQUENCY + SPACE + LIST_BEGIN2 << endl;

            //Write outputs
            for(unsigned int i = 0; i < pOutput->listFreq().size(); i ++)
            {
                output << TAB() << pOutput->listFreq(i) << endl;
            }

            //Close the list and finish
            output << LIST_END2 << END << "\n\n" << BREAK_BOTTOM;

            //Close file
            output.close();
        }
    }
    catch (int err)
    {
        //Add error handler later
        errVal = err;
    }

    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileWriter::writeGlobalMotion()
{
    string writeFilename;           //The name of the file to write out.
    string classname;               //The name of the class type that contains the file elements.
    int errVal = 0;                 //Returned value for error codes from calculations.  Default set to no error.
    bool outputAvail = false;       //Boolean to track if any output objects are even available.

    //Check if output objects even exist.
    if (pOutput->listGlobalMotion().size() == 0)
        outputAvail = false;
    else
        outputAvail = true;

    //create directory.  Function will do nothing if directory already exists.
    createDir(getCurWaveDir());

    //create filename and file output stream.
    writeFilename = projectDirectory + SLASH + getCurWaveDir() + FILE_GLOBAL_MOTION;
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

        if (outputAvail)
        {
            //Get the name for the class written in output.)
            classname = pOutput->listGlobalMotion(0).getClassName();

            //Write the file info block.
            output << getInfoBlock(classname);
            output << BREAK_TOP;
        }
    }

    if (outputAvail)
    {
        //Write output for beginning of body
        output << KEY_BODY << SPACE << OBJECT_BEGIN2 << endl;
        output << TAB() << KEY_NAME << SPACE << QUOTE << pOutput->refCurBody().refBodyName() << QUOTE << END << endl;

        //Repeat process for each item in object list
        for (unsigned int i = 0; i < pOutput->listGlobalMotion().size(); i++)
        {
            try
            {
                //Calculate the results for the given body and specified output object.
                errVal = pOutput->calcGlobalMotion(i);

                //Check for errors
                if (errVal != 0)
                    throw errVal;

                //Start the output object.
                output << TAB() << classname << SPACE << OBJECT_BEGIN2 << endl;
                output << TAB() << KEY_NAME << SPACE << QUOTE << pOutput->listGlobalMotion(i).getName() << QUOTE;
                output << END << endl;

                //Iterate for each frequency in the list
                for (unsigned int j = 0; j < pOutput->listFreq().size(); j++)
                {
                    //Create data signifier
                    output << TAB(2) << KEY_DATA << SPACE << OBJECT_BEGIN2 << endl;
                    //Add frequency designator
                    output << TAB(3) << KEY_FREQUENCY << SPACE << (j+1) << END << endl;
                    //Add value indicator
                    output << TAB(3) << KEY_VALUE << SPACE << LIST_BEGIN2 << endl;

                    //Write out data for given frequency.
                    for (unsigned int k = 0; k < pOutput->listResult(j).n_rows; k++)
                    {
                        //Set precision
                        output.precision(DIGIT);
                        //Write output values for each solution object.
                        //Write the real part
                        output << TAB(4) << std::setprecision(DIGIT) << pOutput->listResult(j)(k,0).real();

                        //Write the imaginary part
                        //Set precision
                        if (pOutput->listResult(j)(k,0).imag() < 0.00)
                            output << std::setprecision(DIGIT) << pOutput->listResult(j)(k,0).imag() << "i" << endl;
                        else
                        {
                            output << "+";
                            output << std::setprecision(DIGIT) << pOutput->listResult(j)(k,0).imag();
                            output << "i" << endl;
                        }
                    }
                    //Close list
                    output << TAB(3) << LIST_END2 << END << endl;
                    //Close data object
                    output << TAB(2) << OBJECT_END2 << endl;
                }
            }
            catch(int err)
            {
                //Error handler
                return errVal;
            }

            //End the output object
            output << TAB() << OBJECT_END2 << "\n";
        }

        //Write output for ending of body
        output << OBJECT_END2 << "\n" << BREAK_BOTTOM << "\n";
    }

    //Close file
    output.close();

    //Report status of efforts
    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileWriter::writeGlobalVelocity()
{
    string writeFilename;           //The name of the file to write out.
    string classname;               //The name of the class type that contains the file elements.
    int errVal = 0;                 //Returned value for error codes from calculations.  Default set to no error.
    bool outputAvail = false;       //Boolean to track if any output objects are even available.

    //Check if output objects even exist.
    if (pOutput->listGlobalVelocity().size() == 0)
        outputAvail = false;
    else
        outputAvail = true;

    //create directory.  Function will do nothing if directory already exists.
    createDir(getCurWaveDir());

    //create filename and file output stream.
    writeFilename = projectDirectory + SLASH + getCurWaveDir() + FILE_GLOBAL_VELOCITY;
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

        if (outputAvail)
        {
            //Get the name for the class written in output.)
            classname = pOutput->listGlobalVelocity(0).getClassName();

            //Write the file info block.
            output << getInfoBlock(classname);
            output << BREAK_TOP;
        }
    }

    if (outputAvail)
    {
        //Write output for beginning of body
        output << KEY_BODY << SPACE << OBJECT_BEGIN2 << endl;
        output << TAB() << KEY_NAME << SPACE << QUOTE << pOutput->refCurBody().refBodyName() << QUOTE << END << endl;

        //Repeat process for each item in object list
        for (unsigned int i = 0; i < pOutput->listGlobalVelocity().size(); i++)
        {
            try
            {
                //Calculate the results for the given body and specified output object.
                errVal = pOutput->calcGlobalVelocity(i);

                //Check for errors
                if (errVal != 0)
                    throw errVal;

                //Start the output object.
                output << TAB() << classname << SPACE << OBJECT_BEGIN2 << endl;
                output << TAB() << KEY_NAME << SPACE << QUOTE << pOutput->listGlobalVelocity(i).getName() << QUOTE;
                output << END << endl;

                //Iterate for each frequency in the list
                for (unsigned int j = 0; j < pOutput->listFreq().size(); j++)
                {
                    //Create data signifier
                    output << TAB(2) << KEY_DATA << SPACE << OBJECT_BEGIN2 << endl;
                    //Add frequency designator
                    output << TAB(3) << KEY_FREQUENCY << SPACE << (j+1) << END << endl;
                    //Add value indicator
                    output << TAB(3) << KEY_VALUE << SPACE << LIST_BEGIN2 << endl;

                    //Write out data for given frequency.
                    for (unsigned int k = 0; k < pOutput->listResult(j).n_rows; k++)
                    {
                        //Set precision
                        output.precision(DIGIT);
                        //Write output values for each solution object.
                        //Write the real part
                        output << TAB(4) << pOutput->listResult(j)(k,0).real();

                        //Write the imaginary part
                        //Set precision
                        output.precision(DIGIT);
                        if (pOutput->listResult(j)(k,0).imag() < 0.00)
                            output << pOutput->listResult(j)(k,0).imag() << "i" << endl;
                        else
                            output << "+" << pOutput->listResult(j)(k,0).imag() << "i" << endl;
                    }
                    //Close list
                    output << TAB(3) << LIST_END2 << END << endl;
                    //Close data object
                    output << TAB(2) << OBJECT_END2 << endl;
                }
            }
            catch(int err)
            {
                //Error handler
                return errVal;
            }

            //End the output object
            output << TAB() << OBJECT_END2 << "\n";
        }

        //Write output for ending of body
        output << OBJECT_END2 << "\n" << BREAK_BOTTOM << "\n";
    }

    //Close file
    output.close();

    //Report status of efforts
    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileWriter::writeGlobalAcceleration()
{
    string writeFilename;           //The name of the file to write out.
    string classname;               //The name of the class type that contains the file elements.
    int errVal = 0;                 //Returned value for error codes from calculations.  Default set to no error.
    bool outputAvail = false;       //Boolean to track if any output objects are even available.

    //Check if output objects even exist.
    if (pOutput->listGlobalAcceleration().size() == 0)
        outputAvail = false;
    else
        outputAvail = true;

    //create directory.  Function will do nothing if directory already exists.
    createDir(getCurWaveDir());

    //create filename and file output stream.
    writeFilename = projectDirectory + SLASH + getCurWaveDir() + FILE_GLOBAL_ACCELERATION;
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

        if (outputAvail)
        {
            //Get the name for the class written in output.)
            classname = pOutput->listGlobalAcceleration(0).getClassName();

            //Write the file info block.
            output << getInfoBlock(classname);
            output << BREAK_TOP;
        }
    }

    if (outputAvail)
    {
        //Write output for beginning of body
        output << KEY_BODY << SPACE << OBJECT_BEGIN2 << endl;
        output << TAB() << KEY_NAME << SPACE << QUOTE << pOutput->refCurBody().refBodyName() << QUOTE << END << endl;

        //Repeat process for each item in object list
        for (unsigned int i = 0; i < pOutput->listGlobalAcceleration().size(); i++)
        {
            try
            {
                //Calculate the results for the given body and specified output object.
                errVal = pOutput->calcGlobalAcceleration(i);

                //Check for errors
                if (errVal != 0)
                    throw errVal;

                //Start the output object.
                output << TAB() << classname << SPACE << OBJECT_BEGIN2 << endl;
                output << TAB() << KEY_NAME << SPACE << QUOTE << pOutput->listGlobalAcceleration(i).getName() << QUOTE;
                output << END << endl;

                //Iterate for each frequency in the list
                for (unsigned int j = 0; j < pOutput->listFreq().size(); j++)
                {
                    //Create data signifier
                    output << TAB(2) << KEY_DATA << SPACE << OBJECT_BEGIN2 << endl;
                    //Add frequency designator
                    output << TAB(3) << KEY_FREQUENCY << SPACE << (j+1) << END << endl;
                    //Add value indicator
                    output << TAB(3) << KEY_VALUE << SPACE << LIST_BEGIN2 << endl;

                    //Write out data for given frequency.
                    for (unsigned int k = 0; k < pOutput->listResult(j).n_rows; k++)
                    {
                        //Set precision
                        output.precision(DIGIT);
                        //Write output values for each solution object.
                        //Write the real part
                        output << TAB(4) << pOutput->listResult(j)(k,0).real();

                        //Write the imaginary part
                        //Set precision
                        output.precision(DIGIT);
                        if (pOutput->listResult(j)(k,0).imag() < 0.00)
                            output << pOutput->listResult(j)(k,0).imag() << "i" << endl;
                        else
                            output << "+" << pOutput->listResult(j)(k,0).imag() << "i" << endl;
                    }
                    //Close list
                    output << TAB(3) << LIST_END2 << END << endl;
                    //Close data object
                    output << TAB(2) << OBJECT_END2 << endl;
                }
            }
            catch(int err)
            {
                //Error handler
                return errVal;
            }

            //End the output object
            output << TAB() << OBJECT_END2 << "\n";
        }

        //Write output for ending of body
        output << OBJECT_END2 << "\n" << BREAK_BOTTOM << "\n";
    }

    //Close file
    output.close();

    //Report status of efforts
    return errVal;
}

//------------------------------------------Function Separator --------------------------------------------------------
int FileWriter::writeGlobalSolution()
{
    string writeFilename;           //The name of the file to write out.
    string classname;               //The name of the class type that contains the file elements.
    int errVal = 0;                 //Returned value for error codes from calculations.  Default set to no error.
    bool outputAvail = false;       //Boolean to track if any output objects are even available.

    //Check if output objects even exist.
    if (pOutput->listGlobalSolution().size() == 0)
        outputAvail = false;
    else
        outputAvail = true;

    //create directory.  Function will do nothing if directory already exists.
    createDir(getCurWaveDir());

    //create filename and file output stream.
    writeFilename = projectDirectory + SLASH + getCurWaveDir() + FILE_GLOBAL_SOLUTION;
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

        if (outputAvail)
        {
            //Get the name for the class written in output.)
            classname = pOutput->listGlobalSolution(0).getClassName();

            //Write the file info block.
            output << getInfoBlock(classname);
            output << BREAK_TOP;
        }
    }

    if (outputAvail)
    {
        //Write output for beginning of body
        output << KEY_BODY << SPACE << OBJECT_BEGIN2 << endl;
        output << TAB() << KEY_NAME << SPACE << QUOTE << pOutput->refCurBody().refBodyName() << QUOTE << END << endl;

        //Repeat process for each item in object list
        for (unsigned int i = 0; i < pOutput->listGlobalSolution().size(); i++)
        {
            try
            {
                //Calculate the results for the given body and specified output object.
                errVal = pOutput->calcGlobalSolution(i);

                //Check for errors
                if (errVal != 0)
                    throw errVal;

                //Start the output object.
                output << TAB() << classname << SPACE << OBJECT_BEGIN2 << endl;
                output << TAB() << KEY_NAME << SPACE << QUOTE << pOutput->listGlobalSolution(i).getName() << QUOTE;
                output << END << endl;

                //Iterate for each frequency in the list
                for (unsigned int j = 0; j < pOutput->listFreq().size(); j++)
                {
                    //Create data signifier
                    output << TAB(2) << KEY_DATA << SPACE << OBJECT_BEGIN2 << endl;
                    //Add frequency designator
                    output << TAB(3) << KEY_FREQUENCY << SPACE << (j+1) << END << endl;
                    //Add value indicator
                    output << TAB(3) << KEY_VALUE << SPACE << LIST_BEGIN2 << endl;

                    //Write out data for given frequency.
                    for (unsigned int k = 0; k < pOutput->listResult(j).n_rows; k++)
                    {
                        //Set precision
                        output.precision(DIGIT);
                        //Write output values for each solution object.
                        //Write the real part
                        output << TAB(4) << pOutput->listResult(j)(k,0).real();

                        //Write the imaginary part
                        //Set precision
                        output.precision(DIGIT);
                        if (pOutput->listResult(j)(k,0).imag() < 0.00)
                            output << pOutput->listResult(j)(k,0).imag() << "i" << endl;
                        else
                            output << "+" << pOutput->listResult(j)(k,0).imag() << "i" << endl;
                    }
                    //Close list
                    output << TAB(3) << LIST_END2 << END << endl;
                    //Close data object
                    output << TAB(2) << OBJECT_END2 << endl;
                }
            }
            catch(int err)
            {
                //Error handler
                return errVal;
            }

            //End the output object
            output << TAB() << OBJECT_END2 << "\n";
        }

        //Write output for ending of body
        output << OBJECT_END2 << "\n" << BREAK_BOTTOM << "\n";
    }

    //Close file
    output.close();

    //Report status of efforts
    return errVal;
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

    return NAME_DIR + convert.str() + SLASH;
}

//------------------------------------------Function Separator --------------------------------------------------------
bool FileWriter::createDir(string path)
{
    //Create Qt directory object.
    QString Qpath;
    Qpath.append(projectDirectory.c_str());
    Qpath.append(SLASH.c_str());
    Qpath.append(path.c_str());

    QDir dir(Qpath);    //Qt objec that is the output directory.

    //Check if directory exists and create if not.
    return dir.mkpath(".");
}

//------------------------------------------Function Separator --------------------------------------------------------
string FileWriter::getInfoBlock(string nameIn)
{
    return VAL_SEAFILE + "\n" + OBJECT_BEGIN2 + "\n" + TAB() + KEY_VERSION + TAB() + VAL_VERSION + END + "\n" + TAB()
            + KEY_FORMAT + TAB()
            + VAL_FORMAT + END + "\n" + TAB() + KEY_OBJECT + TAB() + nameIn + END + "\n" + OBJECT_END2 + "\n\n";
}

//==========================================Section Separator =========================================================
//Private Functions

//------------------------------------------Function Separator --------------------------------------------------------
std::string FileWriter::TAB(int num)
{
    string output;  //Output string
    for (int i = 0; i < num; i++)
        output.append(TAB_REF);

    //Write output
    return output;
}
