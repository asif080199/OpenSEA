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
 *Dec 27, 2014	Nicholas Barczak		Initially Created
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
#ifndef REPBODPOWER_H
#define REPBODPOWER_H
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
 * @brief The Report to calculate average power extracted by each force on the Body object.
 *
 * Calculates actual magnitude of power extracted, resulting from Body motions.  Based on input forces.  Report
 * can provide total power, or any subcategory from the forces already defined.  You can even go down to the level
 * of requesting a single individual force.  One catch is that any filtering by order of derivative must use odd
 * powers of derivative.  Otherwise the math works out so that no power gets extracted.  The Report includes checks
 * to ensure this is true, and provides a warning message to user as a reminder if they do ask for even powers of
 * derivative.  The following keywords are defined for the class.
 *
 * KEY_BODY:        String.  Name of the body to associate with.
 * KEY_CATEGORY:    String.  The category of forces to use for power calculation.  Several options are provided.
 *                  "react":        Total of reactive forces
 *                  "react_user":   Total of user reactive forces
 *                  "react_hydro":  Total of hydrodynamic reactive forces
 *                  "cross":        Total of crossbody forces.
 *                  "cross_user":   Total user crossbody forces.
 *                  "cross_hydro":  Total of hydrodynamic crossbody forces.
 *                  "total":        Total of all forces on body.
 *                  "total_user":   Total of all user forces on body.
 *                  "total_hydro":  Total of all hydrodynamic forces on body.
 * KEY_ORDER:       String.  The order of the specific forces that you want to retrieve.  Order of derivative must
 *                  be an odd number.
 * KEY_FORCENAME:   String.  Name of a specific force object that you want retrieved from the list of user forces.
 *                  For crossbody forces, this force name specifies both the name of the force object, and the
 *                  name of the linked body, seperated by a symbol.
 */
class repBodPower : public Report
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     */
    repBodPower();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.  Sets a pointer to the system object.
     * @param ptIn Pointer to ReportManager object that owns this Report object, pointer passed by value.
     */
    repBodPower(ofreq::ReportManager *ptIn);

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
    /**
     * @brief Pointer to the motion model associated with body in this report.
     */
    osea::ofreq::MotionModel *ptMotion = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the linked body used by the crossbody forces.
     */
    ofreq::Body *ptLinkBody = NULL;

    //------------------------------------------Function Separator ----------------------------------------------------
    std::string pCategory = "";  /**< Name of the category of force type to use. */

    //------------------------------------------Function Separator ----------------------------------------------------
    std::string pLinkBody = "";  /**< Name of the linked body to use for crossbody forces. */

    //------------------------------------------Function Separator ----------------------------------------------------
    std::string pForceName = "";    /**< Name of the force object requested. */

    //------------------------------------------Function Separator ----------------------------------------------------
    //Static variables
    static std::string KEY_BODY;    /**< Name of the body to associate with.*/
    static std::string KEY_CATEGORY;    /**< Name of the category of force type. */
    static std::string KEY_ORDER;   /**< Name of the order of derivative for each force type that you want. */
    static std::string KEY_FORCENAME; /**< Name of the specific user forcename you want. */
    static std::string SEPARATOR;   /**< Symbol to separate force name from linked body name.  Only used when
                                       specifying crossbody force outputs. */

    //------------------------------------------Function Separator ----------------------------------------------------
    static std::string CAT_REACT;   /**< KEY for total of reactive forces. */
    static std::string CAT_REACT_USER; /**< Key for user reactive forces. */
    static std::string CAT_REACT_HYDRO; /**< Key for hydro reactive forces. */
    static std::string CAT_CROSS;   /**< Key for total of crossbody forces. */
    static std::string CAT_CROSS_USER; /**< Key for user crossbody forces. */
    static std::string CAT_CROSS_HYDRO; /**< Key for hydro crossbody forces. */
    static std::string CAT_TOTAL;   /**< Key for total forces. */
    static std::string CAT_TOTAL_USER; /**< Key for all user forces. */
    static std::string CAT_TOTAL_HYDRO; /**< Key for all hydro forces. */

    //------------------------------------------Function Separator ----------------------------------------------------
    int pOrd = -1;   /**< The order of derivative to use for processing solution output. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the power extracted by the user reactive forces.
     * @param forceName String, variable passed by value.  Specifies exactly which user reactive force should be used.
     * The only applies if there is more than one user reactive force defined for the body.  If this input is left
     * blank, the function automatically uses all user reactive forces.
     * @param ord Integer, passed by value.  Specifies exactly the order of derivative that should be used in the
     * user reactive force.  If this input is left blank, the function automatically uses all orders of derivative.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the value of real
     * average power extracted over one wave period, for the given conditions.  Each entry represents a single
     * equation of motion.  Entries are ordered in the sequence that the equations of motion appear in the
     * motion model.  Not the data index.
     */
    std::vector<double> PowReact_user(std::string forceName = "", int ord = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the power extracted by the hydro reactive forces.
     * @param forceName String, variable passed by value.  Specifies exactly which hydro reactive force should be used.
     * The only applies if there is more than one hydro reactive force defined for the body.  If this input is left
     * blank, the function automatically uses all hydro reactive forces.
     * @param ord Integer, passed by value.  Specifies exactly the order of derivative that should be used in the
     * hydro reactive force.  If this input is left blank, the function automatically uses all orders of derivative.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the value of real
     * average power extracted over one wave period, for the given conditions.  Each entry represents a single
     * equation of motion.  Entries are ordered in the sequence that the equations of motion appear in the
     * motion model.  Not the data index.
     */
    std::vector<double> PowReact_hydro(std::string forceName = "", int ord = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the power extracted by the user crossbody forces.
     * @param forceName String, variable passed by value.  Specifies exactly which user crossbody force should be used.
     * The only applies if there is more than one user crossbody force defined for the body.  If this input is left
     * blank, the function automatically uses all user crossbody forces.
     * @param linkName String, variable passed by value.  Specifies exactly which linked body the crossbody force
     * should use.  If the input is left blank, the function automatically uses all linked bodies.
     * @param ord Integer, passed by value.  Specifies exactly the order of derivative that should be used in the
     * user crossbody force.  If this input is left blank, the function automatically uses all orders of derivative.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the value of real
     * average power extracted over one wave period, for the given conditions.  Each entry represents a single
     * equation of motion.  Entries are ordered in the sequence that the equations of motion appear in the
     * motion model.  Not the data index.
     */
    std::vector<double> PowCross_user(std::string forceName = "", std::string linkName = "", int ord = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the power extracted by the hydro crossbody forces.
     * @param forceName String, variable passed by value.  Specifies exactly which hydro crossbody force should be used.
     * The only applies if there is more than one hydro crossbody force defined for the body.  If this input is left
     * blank, the function automatically uses all hydro crossbody forces.
     * @param linkName String, variable passed by value.  Specifies exactly which linked body the crossbody force
     * should use.  If the input is left blank, the function automatically uses all linked bodies.
     * @param ord Integer, passed by value.  Specifies exactly the order of derivative that should be used in the
     * hydro crossbody force.  If this input is left blank, the function automatically uses all orders of derivative.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the value of real
     * average power extracted over one wave period, for the given conditions.  Each entry represents a single
     * equation of motion.  Entries are ordered in the sequence that the equations of motion appear in the
     * motion model.  Not the data index.
     */
    std::vector<double> PowCross_hydro(std::string forceName = "", std::string linkName = "", int ord = -1);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the power extracted by the user reactive force.
     *
     * This function gets used by the PowReact_user function.  This function does the actual math calculation, and
     * the PowReact_user function sorts the inputs to create the proper summary.
     * @param forceInd Integer, passed by value.  The index of a specific reactive force.
     * @param ord Integer, variable passed by value.  The order of derivative.  Includes safety checks to ensure
     * that only odd numbered orders of derivative are used.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the value of real
     * average power extracted over one wave period, for the given conditions.  Each entry represents a single
     * equation of motion.  Entries are ordered in the sequence that the equations of motion appear in the
     * motion model.  Not the data index.
     */
    std::vector<double> calcPowReact_user(int forceInd, int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the power extracted by the hydro reactive force.
     *
     * This function gets used by the PowReact_hydro function.  This function does the actual math calculation, and
     * the PowReact_hydro function sorts the inputs to create the proper summary.
     * @param forceInd Integer, passed by value.  The index of a specific reactive force.
     * @param ord Integer, variable passed by value.  The order of derivative.  Includes safety checks to ensure
     * that only odd numbered orders of derivative are used.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the value of real
     * average power extracted over one wave period, for the given conditions.  Each entry represents a single
     * equation of motion.  Entries are ordered in the sequence that the equations of motion appear in the
     * motion model.  Not the data index.
     */
    std::vector<double> calcPowReact_hydro(int forceInd, int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the power extracted by the user crossbody force.
     *
     * This function gets used by the PowReact_user function.  This function does the actual math calculation, and
     * the PowReact_user function sorts the inputs to create the proper summary.
     * @param forceInd Integer, passed by value.  The index of a specific crossbody force.
     * @param ord Integer, variable passed by value.  The order of derivative.  Includes safety checks to ensure
     * that only odd numbered orders of derivative are used.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the value of real
     * average power extracted over one wave period, for the given conditions.  Each entry represents a single
     * equation of motion.  Entries are ordered in the sequence that the equations of motion appear in the
     * motion model.  Not the data index.
     */
    std::vector<double> calcPowCross_user(int forceInd, int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Calculates the power extracted by the hydro crossbody force.
     *
     * This function gets used by the PowReact_hydro function.  This function does the actual math calculation, and
     * the PowReact_hydro function sorts the inputs to create the proper summary.
     * @param forceInd Integer, passed by value.  The index of a specific crossbody force.
     * @param ord Integer, variable passed by value.  The order of derivative.  Includes safety checks to ensure
     * that only odd numbered orders of derivative are used.
     * @return Returns a vector of doubles, variable passed by value.  Each entry in the vector is the value of real
     * average power extracted over one wave period, for the given conditions.  Each entry represents a single
     * equation of motion.  Entries are ordered in the sequence that the equations of motion appear in the
     * motion model.  Not the data index.
     */
    std::vector<double> calcPowCross_hydro(int forceInd, int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Searches through the list of crossbody objects to find the matching force object.
     * @param typeForce String, passed by value.  Specifies if this is a hydro or user force to match.
     *  Expected inputs:
     *  "hydro" or
     *  "user"
     * @param match Integer, passed by value.  Determines which characteristics to match.
     * 0 = match both force name and linked body name.
     * 1 = match force name only.
     * 2 = match body name only.
     * @param forceName String, passed by value.  The name of the force object to match.
     * @param bodName String, passed by value.  The name of the body object to match.

     * @return Vector of integers, variable passed by value.  The list of indices which matches the forceName
     * and body name criteria supplied.
     */
    std::vector<int> findLinkIndex(std::string typeForce, int match = 0, std::string forceName = "",
                                   std::string bodName = "");


};

}   //Namespace ofreq

}   //Namespace osea

#endif // REPBODPOWER_H
