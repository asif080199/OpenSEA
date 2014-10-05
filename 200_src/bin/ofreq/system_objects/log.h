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
 *May 09 2014       Nicholas Barczak    Initially created
 *Aug 02 2014       Nicholas Barczak    Debugged and updated to Rev 1.0
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
#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <typeinfo>
#include <vector>
#include <QObject>
#include <QString>
#include <QFile>
#include <exception>



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
/**
 * @brief The Log class is used to create output logs.
 *
 * The Log class creates output logs.  You can use this class to write outputs to a log file, to the screen, or
 * to both.  You also have the options to set output format.  You can write output with an actual date and time
 * assigned, or with just a direct message.
 */
class Log
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Sets the default value for the output file.
     */
    Log();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.  Closes the output log file.
     */
    ~Log();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes output in general.
     *
     * Writes general output.  The options of output destination are specified by parameters.  Default values
     * are also included.
     * @param msg The message that you want to write as output.  A carriage return is automatically appended to the
     * end of the message.  Standard string variable.  Variable passed by value.
     * @param timestamp Defines whether a time stamp should be included at the beginning of the message.  Default
     * value is to include a timestamp.  Integer variable.  Default value is to include a timestamp.  Default value
     * can be changed.
     *
     * Options for input are:
     * 1:   Use timestamp (true).
     * -1:  Do not use timestamp (false).
     * @param output String variable.  Sets the destination for the output of the message.  Default value is to write to
     * both the log file and the screen.
     *
     * Options for output types:
     * Log, File, log, file =       Write only to the output file.
     * Screen, Message, screen, message =       Write only to the screen.
     * Both, Dual, All, both, dual, all =       Write the message to both the screen and the log file.
     * @sa Log::setDefaultOutput()
     * @sa Log::setDefaultTimeStamp()
     */
    void Write(std::string msg, int timestamp = 0, std::string output = "Default");

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes output only to the log file.
     *
     * Writes output only to the log file.
     * @param msg The message that you want to write as output.  A carriage return is automatically appended to the
     * end of the message.  Standard string variable.  Variable passed by value.
     * @param timestamp Defines whether a time stamp should be included at the beginning of the message.  Default
     * value is to include a timestamp.  Integer variable.  Default value is to include a timestamp.  Default value
     * can be changed.
     *
     * Options for input are:
     * 1:   Use timestamp (true).
     * -1:  Do not use timestamp (false).
     */
    void WriteLog(std::string msg, int timestamp = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Writes output to only the screen.
     * @param msg The message that you want to write as output.  A carriage return is automatically appended to the
     * end of the message.  Standard string variable.  Variable passed by value.
     * @param timestamp Defines whether a time stamp should be included at the beginning of the message.  Default
     * value is to include a timestamp.  Integer variable.  Default value is to include a timestamp.  Default value
     * can be changed.
     *
     * Options for input are:
     * 1:   Use timestamp (true).
     * -1:  Do not use timestamp (false).
     */
    void WriteScreen(std::string msg, int timestamp = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the location of the log file.
     *
     * Sets the location of the log file.  Provide the full path of the log file, ending with the file name.
     * The function will find the file name at the end.  It will then create the the file and open it for writing.
     * Or if the file already exists, it will open the file and start appending to it.
     * @param pathIn String variable.  Variable passed by value.  Provides the full path of the log file, ending
     * with the file name.
     * The function will find the file name at the end.  It will then create the the file and open it for writing.
     * Or if the file already exists, it will open the file and start appending to it.
     * Directory path is specified as an absolute path, or specified relative to the current program executable
     * file.
     */
    void setLogFile(std::string pathIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Deletes the log file.
     * @param pathIn String variable.  Variable passed by value.  Provides the full path of the log file, ending
     * with the file name.
     * The function will find the file name at the end.  It will then create the the file and open it for writing.
     * Or if the file already exists, it will open the file and start appending to it.
     * Directory path is specified as an absolute path, or specified relative to the current program executable
     * file.
     *
     * Default input uses the current log file path, assuming the log file was defined.
     */
    void deleteLog(std::string pathIn = "");

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Prints out many carriage returns to make it appear as if the screen were cleared.
     *
     * There are some system functions that allow you to clear the screen.  However, these were not implemented
     * because the system commands change for each OS and some forum postings from other programmers indicates that
     * the system() object, which is used to clear the screen, poses a security vulnerability.  It's possible that
     * different system admins will lock down this function in some way and cause problems with oFreq functionality.
     *
     * Instead, stick with what is guaranteed to work.
     */
    void cls();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the default output destination for the log file.
     *
     * Options for output types:
     * Log, File, log, file =       Write only to the output file.
     * Screen, Message, screen, message =       Write only to the screen.
     * Both, Dual, All, both, dual, all =       Write the message to both the screen and the log file.
     * @param output Standard string variable.  Variable passed by value.  Specifies the default output destination.
     * Must be one of the key words listed above.
     */
    void setDefaultOutput(std::string output);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Determines the default value of the timestamp to use.
     *
     * a timestamp can be added to every log entry.  This sets the default value to use for the log timestamp.
     *
     * Options:
     * 1:   Use timestamp (true).
     * -1:  Do not use timestamp (false).
     * @param timein Boolean.  Variable passed by value.  Determines if the default value should be to use the
     * timestamp or not.
     */
    void setDefaultTimeStamp(bool timein);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The actual output file that the log writes to.  Simple ASCII text file.
     */
    std::ofstream outFile;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Notifies that errors were found in the run.
     *
     * Function meant mostly for the standard log.  Simply writes a default message to notify the user that
     * errors were found in the run, and the user should check the error log for more details.  You can also override
     * the action message with your own input.
     * @param input String, passed by value.  An override for the standard notification message.
     * Default Behavior:  "Errors found.  Please check the error log for details."
     * Override Behavior:  input.
     */
    void Notify(std::string input = "");

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Prints one of several pre-programmed standard notification messages.
     *
     * This expands on the Notify function which was meant mostly for the standard log.  This function allows you to
     * select from several pre-selected notification messages.  Intended for scenarios when an unknown error occurred
     * and you can not use the exception classes for the notification message.  Using a single notification message
     * helps ensure consistency of program appearance and improves user confidence in the program.
     * @param msgId Integer, variable passed by value.  The following messages are pre-programmed.
     * 0:       "Errors found.  Please check the error log for details."
     * 1:       "Unknown error occurred."
     *
     */
//    void Notify(int msgId);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the ID properties for the class and function.
     *
     * Returns a string which identified the class name and function name of the calling function.  This can be
     * added onto error statements to properly identify the origin of the error.
     * @return Returns string, variable passed by value.  Returned string identifies class name and object name of the
     * function which produced the error.
     */
//    std::string funcID();

//==========================================Section Separator =========================================================
protected:



//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The directory path of the log file location.  Directory is specified relative to the program executable
     * directory.
     */
    std::string fileDir;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The filename of the log file.
     */
    std::string fileName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Parses the path to enter the file directory and file name as separate entries.
     * @param pathIn String variable.  Variable passed by value.  Provides the full path of the log file, ending
     * with the file name.
     * The function will find the file name at the end.  It will then create the the file and open it for writing.
     * Or if the file already exists, it will open the file and start appending to it.
     * Directory path is specified as an absolute path, or specified relative to the current program executable
     * file.
     */
    void parsePath(std::string pathIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    static std::string SLASH; /**< The directory level specifier.  Changes under windows or linux.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default location for output file.
     *
     * Options for output types:
     * Log, File, log, file =       Write only to the output file.
     * Screen, Message, screen, message =       Write only to the screen.
     * Both, Dual, All, both, dual, all =       Write the message to both the screen and the log file.
     */
    std::string defaultOutput;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The default value for the timestamp.  Default value is 1:  use timestamp.  Options are:
     * 1:   Use timestamp.
     * -1:  Do not use timestamp.
     */
    int defaultTimestamp;

};

}   //Namespace ofreq
}   //Namespace osea

#endif // LOG_H
