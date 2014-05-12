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
 *Aug 24 2013       Nicholas Barczak    Revised to change the paradigm.  Now uses Qt slots and signals.  FileParser
 *                                      is the next generation of superseded class:  ReadInputFile.  FileParser
 *                                      simply reads the text file and parses it into keword value pairs.  These
 *                                      pairs are then sent to Dictionary classes which interpret the keyword value
 *                                      pairs.
 *May 7, 2014       Nicholas Barczak    Debugged and updated to Rev 1.0
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
//Put header files
#ifndef FILEREADER_H
#define FILEREADER_H
#include <QObject>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
#include "objectgroup.h"
#include "../system_objects/system.h"
#include "../system_objects/ofreqcore.h"
#include "dictionary.h"

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

//######################################### Class Separator ###########################################################
//Prototype class declarations
class Dictionary;

//######################################### Class Separator ###########################################################
/**
 * FileReader is the next generation of superseded class:  ReadInputFile.  FileReader simply reads the text file and
 * parses it into keword value pairs.  FileReader reads in the input files.  It then passes those input files to the
 * Parser object.  Parser then segments the file in a series of object groupings with a vector list of keyword
 * value pairs.
 * The file reader interprets a limited number of those keywords to recognize new object declarations.  It creates
 * the new objects.  It then parses the information in that object into a series
 * of keyword-value pairs.  Each pair is sent to a Dictionary object that interprets the information.  Information is
 * sent using Qt Slots and Signals.
 *
 * To use this class, the following sequence must be followed.
 * 1.)  Create FileReader object.
 * 2.)  Create Dictionary objects for each file type you will read.
 * 3.)  Satisfy any follow on dependencies for Dictionary objects.
 * 4.)  Qt Connect FileReader to Dictionary objects.
 * 5.)  Qt Connect FileReader to System object.
 * 6.)  Feed in the target path to the FileReader object.
 * 7.)  Read each file type in turn.  The FileReader object will automatically find the file with the correct
 *      filename, read it, parse it, and send the resulting keyword-value pairs to the appropriate Dictionary object.
 */

class FileReader : public QObject, public osea::ofreq::oFreqCore
{
    Q_OBJECT 
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    FileReader();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor with input for file path that holds input files.
     * @param Path The full path to the working directory.  Variable passed by value.
     */
    FileReader(std::string Path);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Default destructor.  Nothing happens here.
     */
    ~FileReader();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the path to the working directory that all control files are located in.
     * @param input The full path to the working directory.  Do not include directory separator (SLASH, "/") at end
     * of std::string.  Variable passed by value.
     */
    void setPath(std::string input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads in the control file and parses its inputs.
     * @return Returns integer to report success or failure of file parsing.  Returns 0 for success.
     * Returns 1 for file does not exist.
     */
    int readControl();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads in the Bodies file and parses its inputs.
     * @return Returns integer to report success or failure of file parsing.  Returns 0 for success.
     * Returns 1 for file does not exist.
     */
    int readBodies();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads in the Forces file and parses its inputs.
     * @return Returns integer to report success or failure of file parsing.  Returns 0 for success.
     * Returns 1 for file does not exist.
     */
    int readForces();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads in the Sea Environment input file and parses its inputs.
     * @return Returns integer to report success or failure of file parsing.  Returns 0 for success.
     * Returns 1 for file does not exist.
     */
    int readSeaEnv();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads in the Data input file and parses its inputs.
     * @return Returns integer to report success or failure of file parsing.  Returns 0 for success.
     * Returns 1 for file does not exist.
     */
    int readData();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the dictionary object to be used for processing any data read from the input files.
     * @param dictIn The dictionary object that you want to use for processing the file.  This can change between
     * reading individual files.  Variable is passed by reference and stored as a pointer in the object.
     */
    void setDictionary(osea::Dictionary &dictIn);

//==========================================Section Separator =========================================================
public slots:
        //------------------------------------------Function Separator ----------------------------------------------------
        /**
         * @brief Reads hydrodynamic input files.
         * @param path The full path to the hydrodynamic input file to read.
         * @return Returns integer to report success or failure of file parsing.  Returns 0 for success.
         * Returns 1 for file does not exist.
         */
        int readHydroFile(std::string path);

        //------------------------------------------Function Separator ----------------------------------------------------
        /**
         * @brief Sets the system object for the dictionary to reference.
         * @param ptSystem Pointer to the System object.  Variable passed by value.
         */
        void setSystem(ofreq::System* ptInput);

//==========================================Section Separator =========================================================
signals:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sends output of objects discovered when reading the Control file.  Top level objects can include:
     * 1.  System object.
     * @sa System
     * @param output The ObjectGroup object parsed out of the file.  Variable passed by value.
     */
    void outputControlFile(ObjectGroup output);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sends output of objects discovered when reading the Bodies file.  Top level objects can include:
     * 1.  Body object.
     * @sa Body
     * @param output The ObjectGroup object parsed out of the file.  Variable passed by value.
     */
    void outputBodiesFile(ObjectGroup output);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sends output of objects discovered when reading the Data file.  Top level objects can include:
     * 1.  hydrofiles
     * @param output The ObjectGroup object parsed out of the file.  Variable passed by value.
     */
    void outputDataFile(ObjectGroup output);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sends output of objects discovered when reading the Forces file.  Top level objects can include:
     * 1.  ForceActive
     * 2.  ForceReactive
     * 3.  ForceCrossBody
     * @sa ForceActive
     * @sa ForceCross
     * @sa ForceReact
     * @param output The ObjectGroup object parsed out of the file.  Variable passed by value.
     */
    void outputForcesFile(ObjectGroup output);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sends output of objects discovered when reading the SeaEnv file.  Top level objects can include:
     * 1.  Wave_Custom
     * 2.  Sea_Custom
     * @param output The ObjectGroup object parsed out of the file.  Variable passed by value.
     */
    void outputSeaEnvFile(ObjectGroup output);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sends the results of parsing the input file onto the dictionary object.  Use for processing the input
     * file.
     * @param index Integer.  The index which specifies which object in the list of recognized objects to use.
     * Variable passed by value.
     */
    void sendOutput(int index);

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The filepath to the directory that contains the input files.  Always the path to the root directory
     * of the project.  This will be the folder which then contains the other "system" and "constant" folders.
     */
    std::string pPath;   

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of ObjectGroup objects retrieved from parsing the file.  This variable gets reused each time
     * a new file is parsed.
     */
    std::vector<ObjectGroup> plistObjects;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief General function for reading and parsing an input file.  Opens the file with the specified path,
     * parses that file, and stores the parsed file in plistObjects.
     * @param path The path to the file that should be read.  Must be specified as an absolute path, or relative
     * to the directory that ofreq is executed from.
     * @return Returns integer to report success or failure of file reading.  Returns 0 for success.
     * Returns 1 for file does not exist.
     */
    int readFile(std::string path);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to a dictionary object used to process the ObjectGroup's recorded.  Used to send the Object
     * group objects to the Dictionary object for reading.
     * @sa ObjectGroup
     * @sa Dictionary
     */
    osea::Dictionary* ptDict;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the System object.  Used to reference any important variables in the System object.
     */
    ofreq::System* ptSystem;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief private function to initialize variables and perform other functions common to all constructors.
     */
    void initClass();

    //------------------------------------------Function Separator ----------------------------------------------------
    // Directory Names
    static std::string SYS; /**< The system directory name */
    static std::string CONST; /**< The const directory name */
    static std::string SLASH; /**< Directory separator in a std::string path. */

    //------------------------------------------Function Separator ----------------------------------------------------
    // Input File Names
    static std::string CONTROL; /**< The filename for the control file. */
    static std::string BODIES; /**< The filename for the bodies control file. */
    static std::string DATA; /**< The filename for the data control file. */
    static std::string FORCES; /**< The filename for the forces control file. */
    static std::string SEAENV; /**< The filename for the sea environment control file. */

    //------------------------------------------Function Separator ----------------------------------------------------
    // Class Name Designators
    static std::string OBJ_SEAFILE; /**< The std::string designation for a sea file object. */
    static std::string OBJ_SYSTEM; /**< The std::string designation for a system object. */
    static std::string OBJ_HYDROFILE; /**< The std::string designation for a hydrofile object. */
    static std::string OBJ_FORCE_ACTIVE; /**< The std::string designation for an active force object. */
    static std::string OBJ_FORCE_REACT; /**< The std::string designation for a reactive force object. */
    static std::string OBJ_FORCE_CROSS; /**< The std::string designation for a cross-body force object.*/

    // Key Value Pair Designators
    // ---------------------------------
    static std::string KEY_FORMAT;  /**< The key designator for a format value in the seafile object. */
    static std::string KEY_VERSION; /**< The key designator for a version value in the seafile object. */
};

}   // End of namespace osea

#endif // FILEPARSER_H
