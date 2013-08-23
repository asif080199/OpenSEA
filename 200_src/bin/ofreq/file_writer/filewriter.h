/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date              Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *May 15 2013       Shane Honanie       Initially created
 *
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



//######################################### Class Separator ###########################################################
#ifndef FILEWRITER_H
#define FILEWRITER_H
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>  
#include <typeinfo>
#include <QtGlobal>
#include "../derived_outputs/outputsbody.h"
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::filesystem; //for using boost file system

//##################################### VARIABLE DECLARATIONS #########################################################

//==========================================Section Separator =========================================================
//Reference File Declarations
const string HEADER_FILENAME = "../../var/openseaheader.txt";

//==========================================Section Separator =========================================================
//Directory Specifications
const string DIR_NAME = "d"; /**< The starting character for the wave direction directory.**/
const string RES_NAME = "r"; /**< The starting character for the resonant frequency directory.**/
#ifdef Q_OS_WIN
    const string SLASH = "\"";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    const string SLASH = "/";   /**< Directory separator in a string path., linux version**/
#endif

//==========================================Section Separator =========================================================
//General File Content Marks
const string LIST_BEGIN2 = "(";
const string LIST_END2 = ");";
const string OBJECT_BEGIN2 = "{";
const string OBJECT_END2 = "}";
const string END = ";";
const string TAB = "  ";
const string SPACE = " ";
const string NAME = "name";
const string DATA = "data";
const string VALUE = "value";
const string FREQUENCY = "frequency";
const string DIRECTION = "direction";
const string VERSION = "version";
const string FORMAT = "format";
const string OBJECT = "object";
const string BODY = "body";
const string VERSION_INFO = "1.0";
const string FORMAT_INFO = "ascii";
const string SEAFILE2 = "seafile";
const string BREAK_TOP = "// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //\n";
const string BREAK_BOTTOM = "// ************************************************************************* //";
const int DIGIT = 15; /** The number of digits to use in precision of floating point numbers.*/

//==========================================Section Separator =========================================================
//Filename Markers
const string DIRECTIONS_FILENAME = "directions.out";
const string FREQUENCIES_FILENAME = "frequencies.out";
const string GLOBAL_ACCELERATION_FILENAME = "accglobal.out";
const string GLOBAL_MOTION_FILENAME = "motglobal.out";
const string GLOBAL_VELOCITY_FILENAME = "velglobal.out";
const string GLOBAL_SOLUTION_FILENAME = "solglobal.out";

//######################################### Class Separator ###########################################################
//Prototype class declarations
class OutputsBody;

//######################################### Class Separator ###########################################################
/**
 * This class write all outputs to files.  All output data is based on the attached OutputsBody object.  To use the
 * filewriter object, follow this sequence of steps:
 * 1.)  Create object.
 * 2.)  Set OutputsBody object associated with the file.
 * 3.)  Set the filesystem path to the root directory for the current run of ofreq.
 * 4.)  Run the clearFiles() function, which will clear out any pre-existing files.
 * 5.)  Run the writeFile function for the specified file that you want to write out.
 *
 * Note that the OutputsBody object also provides the information on the current wave direction.  And the FileWriter
 * changes its directory to write to depending on the current wave direction.  So the OutputsBody object must be
 * updated before writing a new wave direction.
 */

class FileWriter
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The default constructor.
     */
    FileWriter();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor that includes the two important properties that must be declared for FileWriter to work
     * correctly.  The root path for the project must be declared.  And the pointer to the OutputsBody object must
     * be declared.
     * @param rootPath The full fule system path to the root directory of the currently running oFreq project.  Not
     * the path to the oFreq executable files.  This is the path to the directory containing input and output files.
     * @param BodyIn The OutputsBody object that will be used to write out data for the FileWriter.  The OutputsBody
     * object supplies the data, and the FileWriter writes that data to the ASCII text file.  The OutputsBody object
     * also provides the information on the current wave direction.  So the OutputsBody object must be updated before
     * writing a new wave direction.
     * @sa OutputsBody
     */
    FileWriter(string rootPath, OutputsBody &BodyIn);

    //------------------------------------------Function Separator ----------------------------------------------------
	~FileWriter(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the path to the project directory.  Assumes the string specifying the path does not end with a slash
     * mark.  The class will automatically add the slash mark.  If a slash mark is present, the function will
     * automatically remove it.
     * @param dirIn String specifying the path to the project directory.  Variable passed by value.
     */
    void setProjectDir(string dirIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Remove all old directiories & files written by oFreq previous run.
     * @return Return true if all files & directories were successfully deleted.
     */
    bool clearFiles();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the OutputsBody object.  The OutputsBody object must be set for the fileWriter
     * to work.  All file data comes from the FileWriter object.
     * @return Returns pointer to the OutputsBody object.  Variable passed by reference.
     */
    OutputsBody &refOutputsBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Test is a file exists.  Function automatically assumes that the file is located in the directory
     * associated with the getCurWaveInd() function.  Returns true if the file exists and is valid.  Returns false
     * if the file does not exist or the directory does not exist.
     * @param filename String.  Specifies the filename to search for.  Only needs to specify local filename.  Directory
     * information is already inferred from previous settings with the OutputsBody object.
     * @return Returns boolean variable.  True if the file exists.  False if the file or any required directories
     * do not exist.
     * @sa FileWriter::getCurWaveInd()
     */
    bool fileExists(string filename);

    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------
    //Outputs writing methods below here
    //------------------------------------------Function Separator ----------------------------------------------------
    //------------------------------------------Function Separator ----------------------------------------------------

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
     * @brief Writes the directions list to file.
	 * @return true if write successful.
	 */
    bool writeWaveDirection();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Writes the frequencies list to file.
	 * @return true if write successful.
	 */
    bool writeFrequency();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes the output file of global motions.  If file exists, appends to the file, assuming the appended
     * file is a new body object.
     * @return Returns true if write successful.
     */
    bool writeGlobalMotion();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes the output file of global velocities.  If file exists, appends to the file, assuming the appended
     * file is a new body object.
     * @return Returns true if write successful.
     */
    bool writeGlobalVelocity();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes the output file of global accelerations.  If file exists, appends to the file, assuming the appended
     * file is a new body object.
     * @return Returns true if write successful.
     */
    bool writeGlobalAcceleration();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes the output file of global solutions.  If file exists, appends to the file, assuming the appended
     * file is a new body object.
     * @return Returns true if write successful.
     */
    bool writeGlobalSolution();


//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the index of the current wave direction.  The index is used to specify which directory to write the
     * file into.
     * @return Returns integer which specifies the index of the current wave direction.  Index specifies the wave
     * direction in the list of wave directions.  Valid values are any integer from 0 or greater.
     */
    int getCurWaveInd();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the string containing the folder path for the current wave direction.  Path name includes the
     * slash mark.  For example, if using wave index 0, the string output would be:  "d0/"
     * @return String output.  Has the path name for the current wave directory.
     */
    string getCurWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Creates the directory specified by the string path.  Assumes any specified directory is under the root
     * project directory.
     * @param path String.  The path of the directory to create.
     * @return Returns true if creation sucessful.
     */
    bool createDir(string path);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads in from input file the header to be used in all files.  This is a basic header text that should
     * be at the top of all OpenSEA output files.  Simple identification of the program.  Nothing specific for output.
     */
    void setHeader();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Set information about the file to be written after header and above data, included in the seafile block.
     * @param nameIn The name of the object.
     * @return Returns string.  String contains the file info for the output file.  Everything written into the
     * seafile block.  Variable passed by value.
     */
    string getInfoBlock(string nameIn);

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The path to the root directory for the current project run of oFreq.
     */
    string projectDirectory;

    //------------------------------------------Function Separator ----------------------------------------------------
	string header; /**< The header to be included in all files */

    //------------------------------------------Function Separator ----------------------------------------------------
    vector<double>* thefrequenciesList; /**< List of all wave frequencies. */

    //------------------------------------------Function Separator ----------------------------------------------------
    vector<double>* theDirectionsList; /**< List of of wave directions. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the OutputsBody object that contains all derived outputs to calculate.  All information for
     * file writting comes from the OutputsBody object.  And the wave frequencies and wave directions also come
     * from the OutputsBody object.
     */
    OutputsBody* pOutput;
};
#endif
