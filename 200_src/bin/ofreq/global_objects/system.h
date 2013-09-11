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
 *Aug 22 2013       Nicholas Barczak    Revised to include main objects in the system object.
 *Aug 24 2013       Nicholas Barczak    Changed the definition of wave directions and wave frequencies from their own
 *                                      objects to simple vector lists.
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
#ifndef SYSTEM_H
#define SYSTEM_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <QObject>
#include "../motion_solver/body.h"
#include "../motion_solver/forceactive.h"
#include "../motion_solver/forcecross.h"
#include "../motion_solver/forcereact.h"
#include "../derived_outputs/outputsbody.h"
using namespace std;

//######################################### Class Separator ###########################################################
//Constant Variables declaration
//------------------------------------------Function Separator --------------------------------------------------------
/**
 *@brief Designator for the vector of forceActive class of objects, under the scope of the user.  These are force objects
 *defined by the Forces input file.
 */
const string valForceActive_user = "forceActive_user";

//------------------------------------------Function Separator --------------------------------------------------------
/**
 *@brief Designator for the vector of forceReact class of objects, under the scope of the user.  These are force objects
 *defined by the Forces input file.
 */
const string valForceReact_user = "forceReact_user";

//------------------------------------------Function Separator --------------------------------------------------------
/**
 *@brief Designator for the vector of forceCross class of objects, under the scope of the user.  These are force objects
 *defined by the Forces input file.
 */
const string valForceCross_user = "forceCross_user";

//######################################### Class Separator ###########################################################
/**
 * This class holds data for the system object.  The system object controls the overall behavior of the program.
 * It also decides which analysis type to run:  motion or resonant frequency.  The system object controls the current
 * wave environment settings.
 */

class System : public QObject
{
    Q_OBJECT

//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	System(); /**< The default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
	~System(); /**< The default destructor, nothing happens here. */

    //------------------------------------------Function Separator ----------------------------------------------------
	void testPrint(); /**< Test print to console the values of all data members. */

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the wave frequencies.
	 * @param vecIn The list of wave frequencies.
	 */
	void setWaveFrequencies(vector<double>);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the wave directions.
	 * @param vecIn The list of wave directions.
	 */
	void setWaveDirections(vector<double>);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the spread model.
	 * @param spreadIn The spread model.
	 */
	void setSpreadModel(string);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the list of wave frequencies.
	 * @return The list of wave frequencies.
	 */
	vector<double> getWaveFrequencies();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of wave frequencies.
     * @return Pointer to the list of wave frequencies.  Variable passed by reference.
     */
    vector<double> &refWaveFrequencies();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the list of wave directions.
	 * @return The list of wave directions.
	 */
	vector<double> getWaveDirections();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of wave directions.
     * @return Pointer to the list of wave directions  Variable passed by reference.
     */
    vector<double> &refWaveDirections();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the current wave index.
     * @param input Integer specifying the index of the current wave direction in the list of wave directions.
     */
    void setCurWaveDirInd(int input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the current frequency index.
     * @param input Integer specifying the index of the current wave frequency in the list of wave frequencies.
     */
    void setCurFreqInd(int input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the current wave direction index.
     * @return Integer.  Returns the current wave direction index.  Variable passed by value.
     */
    int getCurWaveDirInd();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the current wave frequency index.
     * @return Integer.  Returns the current wave frequency index.  Variable passed by value.
     */
    int getCurFreqInd();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the current wave direction.  Actual value of wave direction angle.  Angle specified as radians
     * with zero as True North, positive counter-clockwise.
     * @return Double.  Returns the current wave direction.  Actual value of wave direction angle.  Angle specified as
     * radians with zero as True North, positive counter-clockwise.  Variable passed by value.
     */
    double getCurWaveDir();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the current wave frequency.  Actual value of the wave frequency.  Value specified with units of
     * radians per second.
     * @return Double.  Returns the current wave frequency.  Actual value of the wave frequency.  Value specified with
     * units of radians per second.
     */
    double getCurFreq();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns direct access to the list of Body objects.  Includes all the properties included by a vector<>
     * class.
     * @return Returns a vector of Body objects.  Returned variable passed by reference.
     * @sa Body
     */
    vector<Body>& reflistBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns direct access to the list of OutputsBody objects.  Includes all the properties included by a
     * vector<> class.
     * @return Returns a vector of OutputsBody objects.  Returned variable passed by reference.
     * @sa OutputsBody
     */
    vector<OutputsBody>& reflistOutputs();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns direct access to a single Body object.
     * @param input Specifies index of which Body object to access in the list of Body objects.
     * @return Returns a Body object. Returned variable is passed by reference.
     */
    Body& refBody(int input = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns direct access to a single OutputsBody object.
     * @param input Specifies the index of which OutputsBody object to access in the list of OutputsBody objects.
     * @return Returns an OutputsBody object.  Returned variable is passed by reference.
     */
    OutputsBody& refOutput(int input = 0);  

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Clears the vector of force objects for the specified force type.
     * This is useful to free system memory.  Force type is specified by a string
     * input.  If no string input is supplied, all force objects are cleared from the system object.  All force objects
     * should already be copied to their respective Body objects before issuing this function.
     * @param forceClass String input designating which force object type to clear.  Valid values are:
     * forceActive:  Clears the forceActive class of objects.
     * forceReact:   Clears the forceReact class of objects.
     * forceCross:   Clears the forceCross class of objects.
     * "":           Clears all three object classes of objects.
     * other:        If an unknown input is encountered, no objects are cleared.
     */
    void clearForce(string forceClass = "");

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceActive_user object referenced by the name specified in the input.  The Bodies input file
     * will define the forceActive_user object by a name.  This uses that name to retrieve the forceActive_user object.
     * @param forceName String input.  Variable passed by value.  The name of the forceActive_user object.  Must match
     * exactly what is defined in the Forces.in input file which defines the forceActive_user object.
     * @return Returns a forceActive_user object as requested.  Variable passed by value.
     */
    forceActive getforceActive_user(string forceName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the vector of forceActive_user objects.  Provides direct access to the vector.
     * @return Returns a reference to the vector of forceActive_user objects.  Variable passed by reference.
     */
    vector<forceActive> &listforceActive_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceReact_user object referenced by the name specified in the input.  The Bodies input file
     * will define the forceReact_user object by a name.  This uses that name to retrieve the forceReact_user object.
     * @param forceName String input.  Variable passed by value.  The name of the forceReact_user object.  Must match
     * exactly what is defined in the Forces.in input file which defines the forceReact_user object.
     * @return Returns a forceReact_user object as requested.  Variable passed by value.
     */
    forceReact getforceReact_user(string forceName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the vector of forceReact_user objects.  Provides direct access to the vector.
     * @return Returns a reference to the vector of forceReact_user objects.  Variable passed by reference.
     */
    vector<forceReact> &listforceReact_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceCross_user object referenced by the name specified in the input.  The Bodies input file
     * will define the forceCross_user object by a name.  This uses that name to retrieve the forceCross_user object.
     * @param forceName String input.  Variable passed by value.  The name of the forceCross_user object.  Must match
     * exactly what is defined in the Forces.in input file which defines the forceCross_user object.
     * @return Returns a forceCross_user object as requested.  Variable passed by value.
     */
    forceActive getforceCross_user(string forceName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the vector of forceCross_user objects.  Provides direct access to the vector.
     * @return Returns a reference to the vector of forceCross_user objects.  Variable passed by reference.
     */
    vector<forceActive> &listforceCross_user();

//==========================================Section Separator =========================================================
public slots:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Sets the analysis ype.
     * @param analysisTypeIn The analysis type.
     */
    void setAnalysisType(string);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another Body object to the list of Body objects.  Sets the new Body object equal to the input.
     * @param input Body object to add into the list of stored Body objects.  Variable is passed by value and stored
     * independant inside the System class.
     * @sa Body
     */
    void addBody(Body input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another Body object to the list of Body objects.  Uses a blank new Body object.
     * @sa Body
     */
    void addBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another OutputsBody object to the list of OutputsBody objects.  Sets the object equal to the input.
     * @param input OutputsBody object to add into the list of stored OutputsBody objects.  Variable is passed by
     * value and sotred independant inside the System class.
     * @sa OutputsBody
     */
    void addOutput(OutputsBody input);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another OutputsBody object to the list of OutputsBody objects.  Uses a blank new OutputsBody object.
     * @sa OutputsBody
     */
    void addOutput();

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another forceActive object to the list of forceActive_user objects.  Sets the object equal to
     * the input.
     * @param input forceActive object to add to the list of forceActive_user objects.  Variable is passed by value
     * and stored independant inside the System class.
     * @sa forceActive
     */
    void addforceActive_user(forceActive input);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another forceActive Object to the list of forceActive_user objects.  Uses a blank new forceActive
     * object.
     * @sa forceActive
     */
    void addforceActive_user();

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another forceReact object to the list of forceReact_user objects.  Sets the object equal to
     * the input.
     * @param input forceReact object to add to the list of forceReact_user objects.  Variable is passed by value
     * and stored independant inside the System class.
     * @sa forceReact
     */
    void addforceReact_user(forceReact input);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another forceReact Object to the list of forceReact_user objects.  Uses a blank new forceReact
     * object.
     * @sa forceReact
     */
    void addforceReact_user();

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another forceCross object to the list of forceCross_user objects.  Sets the object equal to
     * the input.
     * @param input forceCross object to add to the list of forceCross_user objects.  Variable is passed by value
     * and stored independant inside the System class.
     * @sa forceCross
     */
    void addforceCross_user(forceCross input);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another forceCross Object to the list of forceCross_user objects.  Uses a blank new forceCross
     * object.
     * @sa forceCross
     */
    void addforceCross_user();

//==========================================Section Separator =========================================================
signals:
    /**
     * @brief Returns the System object for information.  Used mainly to access the list of forced defined for the
     * system object.
     * @param Returns a pointer to the System object.  Variable passed by reference.
     */
    void ReferenceSystem(System *mySystem);

//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
	string analysisType; /**< The analysis type. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave directions.  Directions measured in radians.  True North is zero, with positive going
     * counter clockwise.
     */
    vector<double> pWaveDirections;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave frequencies.  Each frequency measured in radians per second.
     */
    vector<double> pWaveFrequencies;

    //------------------------------------------Function Separator ----------------------------------------------------
    int pCurWaveDir; /**< The index of the current wave direction. */

    //------------------------------------------Function Separator ----------------------------------------------------
    int pCurWaveFreq; /**< The index of the current wave frequency. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of Body objects.  Inputs set by file reader go to this list of Body objects.
     * @sa Body
     */
    vector<Body> plistBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of OutputsBody objects.  The inputs set by the file reader on which Output options to calculate
     * go to this list of OutputsBody objects.
     * @sa OutputsBody
     * @sa OutputDerived
     */
    vector<OutputsBody> plistOutputs;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector of active force objects defined for the system to use, under the scope of user
     * defined objects.  These forces are defined by the Forces input file.  These objects are normally only
     * retained for the intial phase of the program.  After input reading is complete, these force objects will be
     * copied to their respective Body objects.  The list of force objects can then be safely deleted.
     * @sa Body
     */
    vector<forceActive> plistForceActive_user;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector of reactive force objects defined for the system to use, under the scope of user
     * defined objects.  These forces are defined by the Forces input file.  These objects are normally only
     * retained for the intial phase of the program.  After input reading is complete, these force objects will be
     * copied to their respective Body objects.  The list of force objects can then be safely deleted.
     * @sa Body
     */
    vector<forceReact> plistForceReact_user;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector of cross-body force objects defined for the system to use, under the scope of user
     * defined objects.  These forces are defined by the Forces input file.  These objects are normally only
     * retained for the intial phase of the program.  After input reading is complete, these force objects will be
     * copied to their respective Body objects.  The list of force objects can then be safely deleted.
     * @sa Body
     */
    vector<forceCross> plistForceCross_user;
};
#endif

