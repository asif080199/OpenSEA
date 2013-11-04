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
#include "../global_objects/ofreqcore.h"

//######################################### Class Separator ###########################################################
//Namespace declarations

//==========================================Section Separator =========================================================
/**
 * The namespace for all code created under the OpenSEA project.  There are also several sub-namespaces, one
 * associated with each primary program under osea.
 * 1.)  ohydro:  Code associated with the program ohydro.
 * 2.)  ofreq:   Code associated with the program ofreq.
 * 3.)  otime:   Code associated with the program otime.
 * 4.)  ofourier:  Code associated with the program ofourier.
 * 5.)  obatch:    Code associated with the program obatch.
 * 6.)  guisea:    Code assocaited with the GUI that interacts with all OpenSEA programs.
 * Any code that may have common utility amongst all programs, such as file reading objects, goes under the generic
 * osea namespace.  Any code that is only useful within the specific program it serves, goes under the specific
 * namespace.  When in doubt, default to just the osea namespace.
 *
 * The namespaces are not intended to create an organizational structure.  They are only intended to prevent
 * name conflicts.
 */
namespace osea
{

//==========================================Section Separator =========================================================
/**
 * The namespace of all code specifically associated with ofreq.
 */
namespace ofreq
{

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

class FileWriter : public osea::ofreq::oFreqCore
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
    FileWriter(std::string rootPath, OutputsBody &BodyIn);

    //------------------------------------------Function Separator ----------------------------------------------------
	~FileWriter(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the path to the project directory.  Assumes the std::string specifying the path does not end with a slash
     * mark.  The class will automatically add the slash mark.  If a slash mark is present, the function will
     * automatically remove it.
     * @param dirIn std::string specifying the path to the project directory.  Variable passed by value.
     */
    void setProjectDir(std::string dirIn);

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
     * @param filename std::string.  Specifies the filename to search for.  Only needs to specify local filename.  Directory
     * information is already inferred from previous settings with the OutputsBody object.
     * @return Returns boolean variable.  True if the file exists.  False if the file or any required directories
     * do not exist.
     * @sa FileWriter::getCurWaveInd()
     */
    bool fileExists(std::string filename);

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
     * @brief Returns the std::string containing the folder path for the current wave direction.  Path name includes the
     * slash mark.  For example, if using wave index 0, the std::string output would be:  "d0/"
     * @return std::string output.  Has the path name for the current wave directory.
     */
    std::string getCurWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Creates the directory specified by the std::string path.  Assumes any specified directory is under the root
     * project directory.
     * @param path std::string.  The path of the directory to create.
     * @return Returns true if creation sucessful.
     */
    bool createDir(std::string path);

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
     * @return Returns std::string.  std::string contains the file info for the output file.  Everything written into the
     * seafile block.  Variable passed by value.
     */
    std::string getInfoBlock(std::string nameIn);

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The path to the root directory for the current project run of oFreq.
     */
    std::string projectDirectory;

    //------------------------------------------Function Separator ----------------------------------------------------
	std::string header; /**< The header to be included in all files */

    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<double>* thefrequenciesList; /**< List of all wave frequencies. */

    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<double>* theDirectionsList; /**< List of of wave directions. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the OutputsBody object that contains all derived outputs to calculate.  All information for
     * file writting comes from the OutputsBody object.  And the wave frequencies and wave directions also come
     * from the OutputsBody object.
     */
    OutputsBody* pOutput;

    //=================================== Variable Declarations =======================================================

    //------------------------------------------Function Separator ----------------------------------------------------
    //Reference File Declarations
    static std::string HEADER_FILENAME;

    //------------------------------------------Function Separator ----------------------------------------------------
    //Directory Specifications
    static std::string NAME_DIR; /**< The starting character for the wave direction directory.**/
    static std::string NAME_RES; /**< The starting character for the resonant frequency directory.**/
    static std::string SLASH; /**< The directory separator in a std::string path.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    //General File Content Marks
    static std::string LIST_BEGIN2;
    static std::string LIST_END2;
    static std::string OBJECT_BEGIN2;
    static std::string OBJECT_END2;
    static std::string END;
    static std::string TAB;
    static std::string SPACE;
    static std::string BREAK_TOP;
    static std::string BREAK_BOTTOM;
    static int DIGIT; /** The number of digits to use in precision of floating point numbers.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    //Key-Value Markers
    static std::string KEY_NAME; /**< Key to mark name of object being described. */
    static std::string KEY_DATA; /**< Key to mark data for an object */
    static std::string KEY_VALUE; /**< Key to mark the values for an object */
    static std::string KEY_FREQUENCY; /**< Key to mark a frequency value */
    static std::string KEY_DIRECTION; /**< Key to mark a direction value */
    static std::string KEY_VERSION; /**< Key to mark the version of the program */
    static std::string KEY_FORMAT; /**< Key to mark the format of the output file */
    static std::string KEY_OBJECT; /**< Key to mark the object of the output file */
    static std::string KEY_BODY; /**< Key to mark a body object */
    static std::string VAL_VERSION; /**< Value to denote current program version */
    static std::string VAL_FORMAT; /**< Value to denote format of output */
    static std::string VAL_SEAFILE; /**< Value to denote a seafile for output file */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Filename Markers
    static std::string FILE_DIRECTIONS; /**< Filename for directions output file */
    static std::string FILE_FREQUENCIES; /**< Filename for frequencies output file */
    static std::string FILE_GLOBAL_ACCELERATION; /**< Filename for global acceleration output file */
    static std::string FILE_GLOBAL_MOTION; /**< Filename for global motion output file */
    static std::string FILE_GLOBAL_VELOCITY; /**< Filename for global velocity output file */
    static std::string FILE_GLOBAL_SOLUTION; /**< Filename for general global solution output file */
};

}   //namespace ofreq
}   //namespace osea

#endif
