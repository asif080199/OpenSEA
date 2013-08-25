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
#ifndef FILEPARSER_H
#define FILEPARSER_H
#include <QObject>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
#include "objectgroup.h"

using namespace std;

//######################################### Class Separator ###########################################################
//Constant variables

// Directory Names
// ---------------------------------
const string SYS = "system"; /**< The system directory name */
const string CONST = "constant"; /**< The constant directory name */
#ifdef Q_OS_WIN
    const string SLASH = "\"";  /**< Directory separator in a string path., windows version**/
#elif defined Q_OS_LINUX
    const string SLASH = "/";   /**< Directory separator in a string path., linux version**/
#endif

// Input File Names
// ---------------------------------
const string CONTROL = "control.in"; /**< The filename for the control file. */
const string BODIES = "bodies.in"; /**< The filename for the bodies control file. */
const string DATA = "data.in"; /**< The filename for the data control file. */
const string FORCES = "forces.in"; /**< The filename for the forces control file. */
const string SEAENV = "seaenv.in"; /**< The filename for the sea environment control file. */

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

class FileReader : public QObject
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
     * @param input The full path to the working directory.  Variable passed by value.
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
signals:

//==========================================Section Separator =========================================================
public slots:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads hydrodynamic input files.
     * @return Returns integer to report success or failure of file parsing.  Returns 0 for success.
     * Returns 1 for file does not exist.
     */
    int readHydroFile();

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
};

#endif // FILEPARSER_H
