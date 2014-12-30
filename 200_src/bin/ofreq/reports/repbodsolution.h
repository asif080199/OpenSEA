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
#ifndef REPGLOBSOLUTION_H
#define REPGLOBSOLUTION_H
#include "report.h"

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
 * @brief Report for global solution to Body motions.
 *
 * Provides the solution of the Body motions, in each degree of freedom, for the specified body and order of
 * derivative.  The input must specify the order of derivative and the body.  The following keywords are declared:
 *
 * KEY_BODY:        String.  Name of the body to associate with.
 * KEY_ORDER:       String.  The order of the derivative.
 */
class repBodSolution: public Report
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor object.
     */
    repBodSolution();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Sets a pointer to the system object.
     * @param ptIn Pointer to ReportManager object that owns this Report object, pointer passed by value.
     */
    repBodSolution(ofreq::ReportManager *ptIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Generates the Report Data.
     *
     * Writes the results of the calculation to the vector of Data values.  Calling the calcReport function only
     * generates the values.  They must be retrieved from the object after calculation, using the listData() function.
     *
     * @param freqInd Integer, variable passed by value.  The wave frequency index to use for calculating the Report
     * object's data.  Specifies the index of the wave frequency to retrieve from the list of wave frequencies.
     * The frequency index has no meaning for this report and can be ignored.  It is included only because the
     * function gets inheritted from another class, and the form of the function must be maintained.
     */
    virtual void calcReport(int freqInd = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the RAO report data.
     *
     * Write the results of the calculation of the vector of RAO Data values.  Calling calcRAO function only
     * generates the value.  THey must be retrieved from the object after calculations, using the listRAO() function.
     *
     * @param freqInd Integer, variable passed by value.  The wave frequency index to use for calculating the Report
     * object's data.  Specifies the index of the wave frequency to retrieve from the list of wave frequencies.
     * Most Report values will depend on the wave frequency specified.
     */
    virtual void calcRAO(int freqInd = -1);

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

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads through each of the constants assigned to this report object and processes them as needed.
     * @param constIndex Integer, passed by value.  The index of the Report constant value to evaluate.
     */
    void defineConst(int constIndex);

    //------------------------------------------Function Separator ----------------------------------------------------
    //Static variables
    static std::string KEY_BODY;    /**< Name of the body to associate with.*/
    static std::string KEY_ORDER;   /**< Order of derivative to use for processing the output.*/

    //------------------------------------------Function Separator ----------------------------------------------------
    int pOrd;   /**< The order of derivative to use for processing solution output. */


//==========================================Section Separator =========================================================
private:

};


}   //Namespace ofreq

}   //Namespece osea

#endif // REPGLOBSOLUTION_H
