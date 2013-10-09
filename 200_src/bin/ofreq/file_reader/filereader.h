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
#include "../global_objects/system.h"
#include "../global_objects/ofreqcore.h"

using namespace std;

//######################################### Class Separator ###########################################################
//Prototype class declarations
//class System;       /** < Prototype class declaration of System object.  Allows reference to system object.*/

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

class FileReader : public QObject, public oFreqCore
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
    FileReader(string Path);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Default destructor.  Nothing happens here.
     */
    ~FileReader();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the path to the working directory that all control files are located in.
     * @param input The full path to the working directory.  Do not include directory separator (SLASH, "/") at end
     * of string.  Variable passed by value.
     */
    void setPath(string input);

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

//==========================================Section Separator =========================================================
public slots:
        //------------------------------------------Function Separator ----------------------------------------------------
        /**
         * @brief Reads hydrodynamic input files.
         * @param path The full path to the hydrodynamic input file to read.
         * @return Returns integer to report success or failure of file parsing.  Returns 0 for success.
         * Returns 1 for file does not exist.
         */
        int readHydroFile(string path);

        //------------------------------------------Function Separator ----------------------------------------------------
        /**
         * @brief Sets the system object for the dictionary to reference.
         * @param ptSystem Pointer to the System object.  Variable passed by value.
         */
        void setSystem(System* ptInput);

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

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The filepath to the directory that contains the input files.  Always the path to the root directory
     * of the project.  This will be the folder which then contains the other "system" and "constant" folders.
     */
    string pPath;   

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of ObjectGroup objects retrieved from parsing the file.  This variable gets reused each time
     * a new file is parsed.
     */
    vector<ObjectGroup> plistObjects;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief General function for reading and parsing an input file.  Opens the file with the specified path,
     * parses that file, and stores the parsed file in plistObjects.
     * @param path The path to the file that should be read.  Must be specified as an absolute path, or relative
     * to the directory that ofreq is executed from.
     * @return Returns integer to report success or failure of file reading.  Returns 0 for success.
     * Returns 1 for file does not exist.
     */
    int readFile(string path);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the System object.  Used to reference any important variables in the System object.
     */
    System* ptSystem;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief private function to initialize variables and perform other functions common to all constructors.
     */
    void initClass();

    //------------------------------------------Function Separator ----------------------------------------------------
    // Directory Names
    static string SYS; /**< The system directory name */
    static string CONST; /**< The const directory name */
    static string SLASH; /**< Directory separator in a string path. */

    //------------------------------------------Function Separator ----------------------------------------------------
    // Input File Names
    static string CONTROL; /**< The filename for the control file. */
    static string BODIES; /**< The filename for the bodies control file. */
    static string DATA; /**< The filename for the data control file. */
    static string FORCES; /**< The filename for the forces control file. */
    static string SEAENV; /**< The filename for the sea environment control file. */

    //------------------------------------------Function Separator ----------------------------------------------------
    // Class Name Designators
    static string OBJ_SEAFILE; /**< The string designation for a sea file object. */
    static string OBJ_SYSTEM; /**< The string designation for a system object. */
    static string OBJ_HYDROFILE; /**< The string designation for a hydrofile object. */
    static string OBJ_FORCE_ACTIVE; /**< The string designation for an active force object. */
    static string OBJ_FORCE_REACT; /**< The string designation for a reactive force object. */
    static string OBJ_FORCE_CROSS; /**< The string designation for a cross-body force object.*/

    // Key Value Pair Designators
    // ---------------------------------
    static string KEY_FORMAT;  /**< The key designator for a format value in the seafile object. */
    static string KEY_VERSION; /**< The key designator for a version value in the seafile object. */
};

#endif // FILEPARSER_H
