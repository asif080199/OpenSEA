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
 *Dec 30, 2014	Nicholas Barczak		Initially Created
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
#ifndef REPABSEFF_H
#define REPABSEFF_H
#include "report.h"
#include "repbodpower.h"

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
 * @brief The Report for calculation of body absolute efficiency.
 *
 * Absolute efficiency is based on calculation of another body power object.  The absolute efficiency is a measurement of
 * efficiency, based on extracted power.  Absolute efficiency is a single number.  The Report requires reference to
 * another Report for power calculation.  The absolute efficiency changes, depending on what terms are included in the
 * Power calculation.  Thus, the absolute efficiency needs to reference another Power object to get the appropriate
 * calculation.  The absolute efficiency provides a total efficiency, summing extracted power for all motion variables.
 * Or, the user can specify a list of exactly which variables to include in the summation.  The following keywords are
 * declared:
 *
 * KEY_BODY:        String.  Name of the body to associate with.
 * KEY_POWERREP:    String.  Name of the power report to reference.
 * KEY_VARS:        Integer.  List of variable indices used to sum for efficiency calculation.
 */
class repAbsEff : public ofreq::Report
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor object.
     */
    repAbsEff();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Sets a pointer to the system object.
     * @param ptIn Pointer to ReportManager object that owns this Report object, pointer passed by value.
     */
    repAbsEff(ofreq::ReportManager *ptIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Generates the Report Data.
     *
     * Writes the results of the calculation to the vector of Data values.  Calling the calcReport function only
     * generates the values.  They must be retrieved from the object after calculation, using the listData() function.
     *
     * The calcReport() function will overwrite any previously calculated data.  The function includes a user input
     * that allows you to specify a specific wave frequency for calculation.  Default behavior is to calculate data
     * for all wave frequencies.  If only a single wave frequency is specified, the function will fill all other data
     * entries up to that frequency with zero data objects.  Only the specified frequency will contain the data.  It's
     * place in the vector of Data objects specifies its index.
     * @param freqInd Integer, variable passed by value.  The wave frequency index to use for calculating the Report
     * object's data.  Specifies the index of the wave frequency to retrieve from the list of wave frequencies.
     * The frequency index has no meaning for this report and can be ignored.  It is included only because the
     * function gets inheritted from another class, and the form of the function must be maintained.
     */
    virtual void calcReport(int freqInd = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pure virtual member.  Calculates the RAO report data.
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


//==========================================Section Separator =========================================================
private:

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reads through each of the constants assigned to this report object and processes them as needed.
     * @param constIndex Integer, passed by value.  The index of the Report constant value to evaluate.
     */
    void defineConst(int constIndex);

    //------------------------------------------Function Separator ----------------------------------------------------
    ofreq::Report *ptPower = NULL;    /**< Pointer to the Body power report. */

    //------------------------------------------Function Separator ----------------------------------------------------
    std::vector<int> plistVars; /**< The list of variables to sum the power for. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the maximum wave power that can be extracted.
     *
     * This is the denominator term used to calculate the absolute efficiency.
     * @param freqInd Integer, variable passed by value.  The index of the body encounter frequency to use.
     * @return Returns double, variable passed by value.  The value of the average power calculated.
     */
    double calcWavePow(int freqInd);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the reactive force coefficient, hydro.  Used to calculate maximum power.
     *
     * Uses the motion model to calculate results, based on evaluations from the motion model.  This method includes
     * any constants or additional coefficients the user may have defined directly in the motion model.
     *
     * This method specifically just calculates the reactive coefficients for a derivative of order = 1.  This is
     * part of the formula for calculating maximum power.
     * @param forceInd Integer, passed by value.  The index of a specific reactive force.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the the force
     * coefficient returned from the motion model.  This is the direct coefficient.  The imaginary numbers and
     * wave frequencies were removed from the calculation.  Entries are ordered in the sequence that the equations
     * of motion appear in the motion model.  Not the data index.
     */
    std::vector<double> calcForceReact_hydro(int forceInd);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds the list of variable names to the output.
     */
    void addVarNames();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Boolean to track if the list of variables was already added.
     */
    bool plistAdded = false;

    //------------------------------------------Function Separator ----------------------------------------------------
    //Static variables
    static std::string KEY_BODY;    /**< Name of the body to associate with.*/
    static std::string KEY_POWERREP;    /**< Name of the power report to reference. */
    static std::string KEY_VARS;        /**< List of variable indices used to sum for efficiency calculation. */

};

}   //Namespace ofreq

}   //Namespace osea

#endif // REPABSEFF_H
