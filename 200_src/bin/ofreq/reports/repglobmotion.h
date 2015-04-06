/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date          Author                  Description
 *---------------------------------------------------------------------------------------------------------------------
 *Dec 13, 2014	Nicholas Barczak		Initially Created
 *Dec 20, 2014  Nicholas Barczak        Debugged and updated to Rev 1.0
 *Mar 29, 2015  Nicholas Barczak        Adjusted to note that this is now global coordinate system.
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
//Include statements and headers
#ifndef REPGLOBMOTION_H
#define REPGLOBMOTION_H
#include "repglobsolution.h"

#ifdef Q_OS_WIN
    //Any windows specific inclusions go in here.
#elif defined Q_OS_LINUX
    //Any linux specific inclusions go in here.
#endif

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
 * @brief Report for global motions of the body motions (0th order derivative.)
 *
 * Provides the solution to the Body motions, in each degree of freedom, for a 0th order of derivative.  Solution
 * provided in global coordinate system.  The input must specify the name of the body.  The following keywords are
 * declared:
 *
 * KEY_BODY:        String.  Name of the body to associate with.
 */
class repGlobMotion: public repGlobSolution
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    repGlobMotion();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Sets a pointer to the system object.
     * @param ptIn Pointer to ReportManager object that owns this Report object, pointer passed by value.
     */
    repGlobMotion(ofreq::ReportManager *ptIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the name of the class associated with this Report object.
     *
     * The Report class is a pure virtual class.  There are multiple different types of reports, each derived from
     * this abstract class.  oFreq needs to know the class of the report, to know which file to write the report out
     * to.  This is done with the getClass() function.  This uses macros in the code.
     * @return Returns the name of the Report class.  Returned variable is a string, variable passed by value.
     */
    virtual std::string getClass();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides the filename associated with this Report object.
     * @return Returns a string object, passed by value.  String is the filename associated with this Report.
     */
    virtual std::string getFileName();

//==========================================Section Separator =========================================================
protected:


//==========================================Section Separator =========================================================
private:

};

}   //Namespace ofreq

}   //Namespace osea

#endif // REPGLOBMOTION_H
