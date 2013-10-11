/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date		Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *Mar 09 2013	Nicholas Barczak		Initially Created
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
//Put header file includes and constant variables in here.
#ifndef OFREQCORE_H
#define OFREQCORE_H

#ifdef Q_OS_WIN
    //Any windows specific inclusions go in here.
#elif defined Q_OS_LINUX
    //Any linux specific inclusions go in here.
#endif
#include <QDebug>   //Include for access to debugging objects.
#include <fstream>
#include <string>
#include <time.h>
#include <typeinfo>

using namespace std;

//######################################### Class Separator ###########################################################
/**
 * @brief The core oFreq class.  All oFreq classes inherit from this class.
 *
 * Core oFreq class.  All oFreq classes inherit from this class.  Includes definition for anything fundamental
 * and common to the entire program.  Major items are any code used for application debugging.  Also includes
 * some objects to give everything access to log and error files for the program.
 */
class oFreqCore
{

//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Default constructor.  Nothing happens here.
     */
    oFreqCore();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Default destructor.  Nothing happens here.
     */
    virtual ~oFreqCore();

//==========================================Section Separator =========================================================
signals:


//==========================================Section Separator =========================================================
public slots:

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The log file for an oFreq run.  Records normal actions for the program.  Informs user of regular program
     * developments.  Through inheritence of the oFreqCore class, this object is available to every object in the
     * oFreq application.
     */
    static std::ofstream OutLog;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The error log file for an oFreq run.  Records any errors or warnings for the program.  Informs user of
     * errors and warnings and where they occurred.  Through inheritence of the oFreqCore class, this object is
     * available for every object in the oFreq application.
     */
    static std::ofstream ErrLog;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Opens the output log file and prepares it for writing.
     * @param dirIn String parameter.  Designates the directory path to use for writing the log file.
     */
    void setOutLog(string dirIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Opens the error log file and prepares it for writing.
     * @param dirIn String parameter.  Designates the directory path to use for writing the error file.
     */
    void setErrLog(string dirIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes output of a log message to the log file.  Adds in a date and time stamp to the log.
     * @param mesIn String variable.  The message to write to the log file.
     */
    void writeLog(string mesIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes output of an error message to the error file.  Adds in a date, time, and class name to the log.
     * @param mesIn String variable.  The message to write to the log file.
     */
    void writeError(string mesIn);

//==========================================Section Separator =========================================================
private:
    static std::string FILE_OUTPUT; /**< The filename for the output file.*/
    static std::string FILE_ERROR; /**< The filename for the error file.*/
    static std::string SLASH; /**< Directory separator in a string path. */
    static std::string DIR; /**< The full path for the directory of the oFreq run.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Generates a string that is the current date and time, as recorded from the system clock.  Format is:
     * yyyy-mm-dd.hh:mm::ss
     * @return Generates a string that is the current date and time, as recorded from the system clock.  Variable
     * returned by value.
     */
    const std::string Time();
};

#endif // OFREQCORE_H
