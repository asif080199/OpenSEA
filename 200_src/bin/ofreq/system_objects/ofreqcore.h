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
//Put header file includes and constant variables in here.
#ifndef OFREQCORE_H
#define OFREQCORE_H

#ifdef Q_OS_WIN
    //Any windows specific inclusions go in here.
#elif defined Q_OS_LINUX
    //Any linux specific inclusions go in here.
#endif
#include <QDebug>   //Include for access to debugging objects.
#include <string>
#include <typeinfo>
#include <exception>
#include <stdexcept>
#include "log.h"

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
 * @brief The core oFreq class.  All oFreq classes inherit from this class.
 *
 * Core oFreq class.  All oFreq classes inherit from this class.  Includes definition for anything fundamental
 * and common to the entire program.  Major items are any code used for application debugging.  Also includes
 * some objects to give everything access to log and error files for the program.
 *
 * The core class also includes access to the Qt debugging options.  In addition to the normal log files, this allows
 * you to write debugging messages that will only appear in the application output window of the developer environment.
 * Helpful to developer specific notes.  In general, we want to write things out to the user error log.  This
 * enables the user to also troubleshoot the problem.  But if you ever want to write specific debug messages for
 * developers only, here are four functions enabled that write to the developer console only.
 * - qDebug() is used for writing custom debug output.
 * - qWarning() is used to report warnings and recoverable errors in your application.
 * - qCritical() is used for writing critical error mesages and reporting system errors.
 * - qFatal() is used for writing fatal error messages shortly before exiting.
 *
 * These functions work as stream operators.  You can read more about them on the website:
 *
 * http://qt-project.org/doc/qt-4.8/debug.html#warning-and-debugging-messages
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

    //------------------------------------------Function Separator ----------------------------------------------------
    /** @brief The output log for all standard messages.
     *
     * Writes to both a log file and to the screen.  All messages are time stamped.
     */
    static ofreq::Log logStd;

    //------------------------------------------Function Separator ----------------------------------------------------
    /** @brief The output log for all error messages.
     *
     * Writes only to a log file.  All messages are time stamped.
     */
    static ofreq::Log logErr;

    //------------------------------------------Function Separator ----------------------------------------------------
    /** @brief The output log for monitor variables.
     *
     * This log file is for any output that another program might use to monitor the program progress in real time.
     */
    static ofreq::Log logMon;

//==========================================Section Separator =========================================================
signals:


//==========================================Section Separator =========================================================
public slots:

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    static std::string SLASH; /**< The directory level specifier.  Changes under windows or linux.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    static double GRAVITY; /**< Acceleration due to gravity.  Normal earth gravity.*/

//==========================================Section Separator =========================================================
private:


};

}   //Namespace ofreq
}   //Namespace osea

#endif // OFREQCORE_H
