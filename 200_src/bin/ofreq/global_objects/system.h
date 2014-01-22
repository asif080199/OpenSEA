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
#include "../global_objects/body.h"
#include "../derived_outputs/outputsbody.h"
#include "./ofreqcore.h"

//Motion Models to include
#include "./motion_model/motionmodel.h"
#include "./motion_model/model6dof.h"

//The following definitions are included indirectly through inclusion of body.h
//#include "./forceactive.h"
//#include ".forcecross.h"
//#include ".forcereact.h"

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
 * This class holds data for the system object.  The system object controls the overall behavior of the program.
 * It also decides which analysis type to run:  motion or resonant frequency.  The system object controls the current
 * wave environment settings.
 */
class System : public QObject, public osea::ofreq::oFreqCore
{
    Q_OBJECT

//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
	System(); /**< The default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
    virtual ~System(); /**< The default destructor, clears any dynamic memory. */

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the wave frequencies.
	 * @param vecIn The list of wave frequencies.
	 */
	void setWaveFrequencies(std::vector<double>);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the wave directions.
	 * @param vecIn The list of wave directions.
	 */
	void setWaveDirections(std::vector<double>);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Sets the spread model.
	 * @param spreadIn The spread model.
	 */
	void setSpreadModel(std::string);

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the list of wave frequencies.
	 * @return The list of wave frequencies.
	 */
	std::vector<double> getWaveFrequencies();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of wave frequencies.
     * @return Pointer to the list of wave frequencies.  Variable passed by reference.
     */
    std::vector<double> &listWaveFrequencies();

    //------------------------------------------Function Separator ----------------------------------------------------
	/**
	 * Retrieve the list of wave directions.
	 * @return The list of wave directions.
	 */
	std::vector<double> getWaveDirections();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of wave directions.
     * @return Pointer to the list of wave directions  Variable passed by reference.
     */
    std::vector<double> &listWaveDirections();

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
     * @brief Returns direct access to the list of Body objects.  Includes all the properties included by a std::vector<>
     * class.
     * @return Returns a vector of Body objects.  Returned variable passed by reference.
     * @sa Body
     */
    std::vector<Body> &listBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns direct access to a single Body object.
     * @param input Specifies index of which Body object to access in the list of Body objects.
     * @return Returns a Body object. Returned variable is passed by reference.
     */
    Body &listBody(int input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns direct access to the list of OutputsBody objects.  Includes all the properties included by a
     * std::vector<> class.
     * @return Returns a vector of OutputsBody objects.  Returned variable passed by reference.
     * @sa OutputsBody
     */
    std::vector<OutputsBody> &listOutput();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns direct access to a single OutputsBody object.
     * @param input Specifies the index of which OutputsBody object to access in the list of OutputsBody objects.
     * @return Returns an OutputsBody object.  Returned variable is passed by reference.
     */
    OutputsBody &listOutput(int input);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Clears the vector of force objects for the specified force type.
     * This is useful to free system memory.  Force type is specified by a std::string
     * input.  If no std::string input is supplied, all force objects are cleared from the system object.  All force objects
     * should already be copied to their respective Body objects before issuing this function.
     * @param forceClass std::string input designating which force object type to clear.  Valid values are:
     * ForceActive:  Clears the ForceActive class of objects.
     * ForceReact:   Clears the ForceReact class of objects.
     * ForceCross:   Clears the ForceCross class of objects.
     * "":           Clears all three object classes of objects.
     * other:        If an unknown input is encountered, no objects are cleared.
     */
    void clearForce(std::string forceClass = "");

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceActive_user object referenced by the name specified in the input.  The Bodies input file
     * will define the forceActive_user object by a name.  This uses that name to retrieve the forceActive_user object.
     * @param forceName std::string input.  Variable passed by value.  The name of the forceActive_user object.  Must match
     * exactly what is defined in the Forces.in input file which defines the forceActive_user object.
     * @return Returns a forceActive_user object as requested.  Variable passed by value.
     */
    ForceActive getForceActive_user(std::string forceName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the vector of forceActive_user objects.  Provides direct access to the vector.
     * @return Returns a reference to the vector of forceActive_user objects.  Variable passed by reference.
     */
    std::vector<ForceActive> &listForceActive_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns reference to the forceActive_user object referenced by the index specified in the input.
     * @param forceIndex Integer variable which defines the index of which forceActive_user object to retrieve.
     * Variable passed by reference.
     * @return Returns a reference to the ForceActive_user object.  Variable passed by reference
     */
    ForceActive &listForceActive_user(unsigned int forceIndex);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceReact_user object referenced by the name specified in the input.  The Bodies input file
     * will define the forceReact_user object by a name.  This uses that name to retrieve the forceReact_user object.
     * @param forceName std::string input.  Variable passed by value.  The name of the forceReact_user object.  Must match
     * exactly what is defined in the Forces.in input file which defines the forceReact_user object.
     * @return Returns a forceReact_user object as requested.  Variable passed by value.
     */
    ForceReact getforceReact_user(std::string forceName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the vector of forceReact_user objects.  Provides direct access to the vector.
     * @return Returns a reference to the vector of forceReact_user objects.  Variable passed by reference.
     */
    std::vector<ForceReact> &listForceReact_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns reference to the forceReact_user object referenced by the index specified in the input.
     * @param forceIndex Integer variable which defines the index of which forceReact_user object to retrieve.
     * Variable passed by value.
     * @return Returns a reference to the ForceReact_user object.  Variable passed by reference
     */
    ForceReact &listForceReact_user(unsigned int forceIndex);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the forceCross_user object referenced by the name specified in the input.  The Bodies input file
     * will define the forceCross_user object by a name.  This uses that name to retrieve the forceCross_user object.
     * @param forceName std::string input.  Variable passed by value.  The name of the forceCross_user object.  Must match
     * exactly what is defined in the Forces.in input file which defines the forceCross_user object.
     * @return Returns a forceCross_user object as requested.  Variable passed by value.
     */
    ForceCross getforceCross_user(std::string forceName);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Exposes the vector of forceCross_user objects.  Provides direct access to the vector.
     * @return Returns a reference to the vector of forceCross_user objects.  Variable passed by reference.
     */
    std::vector<ForceCross> &listForceCross_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns reference to the forceCross_user object referenced by the index specified in the input.
     * @param forceIndex Integer variable which defines the index of which forceCross_user object to retrieve.
     * Variable passed by value.
     * @return Returns a reference to the ForceCross_user object.  Variable passed by reference
     */
    ForceCross &listForceCross_user(unsigned int forceIndex);

//==========================================Section Separator =========================================================
public slots:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Sets the analysis ype.
     * @param analysisTypeIn The analysis type.
     */
    void setAnalysisType(std::string);

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
     * @brief Adds another ForceActive object to the list of forceActive_user objects.  Sets the object equal to
     * the input.
     * @param input ForceActive object to add to the list of forceActive_user objects.  Variable is passed by value
     * and stored independant inside the System class.
     * @sa ForceActive
     */
    void addForceActive_user(ForceActive input);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another ForceActive Object to the list of forceActive_user objects.  Uses a blank new ForceActive
     * object.
     * @sa ForceActive
     */
    void addForceActive_user();

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another ForceReact object to the list of forceReact_user objects.  Sets the object equal to
     * the input.
     * @param input ForceReact object to add to the list of forceReact_user objects.  Variable is passed by value
     * and stored independant inside the System class.
     * @sa ForceReact
     */
    void addForceReact_user(ForceReact input);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another ForceReact Object to the list of forceReact_user objects.  Uses a blank new ForceReact
     * object.
     * @sa ForceReact
     */
    void addForceReact_user();

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another ForceCross object to the list of forceCross_user objects.  Sets the object equal to
     * the input.
     * @param input ForceCross object to add to the list of forceCross_user objects.  Variable is passed by value
     * and stored independant inside the System class.
     * @sa ForceCross
     */
    void addForceCross_user(ForceCross input);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds another ForceCross Object to the list of forceCross_user objects.  Uses a blank new ForceCross
     * object.
     * @sa ForceCross
     */
    void addForceCross_user();

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief This converts the cross-body force links from simple identification by name into actual pointers to
     * each body.  The linkBodies command must exist so that all bodies can be read into the program before linking
     * takes place.  This command searches through the list of bodies to get a matching body name and creates a pointer
     * link to that body.  At the end, it clears the list of names for linked bodies, to reduce memory requirements.
     * @param bodID An integer variable that describes the base body which the function should process all links for.
     * parameter passed by value.
     * @sa Body::listNamedLink_user()
     * @sa Body::listNamedLink_hydro();
     *
     */
    void linkBodies(int bodID);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides access to the full list of motion models.
     *
     * Returns a vector to the full list of motion models.  The list of motion models are all the various model
     * classes available to the system at run time.  Each object in the vector is a different class, but all
     * classes in the vector are derived from the MotionModel class.
     * @sa MotionModel
     * @return Returns a vector of objects of different types.  Each object is derived from the MotionModel class.
     * Returned variable is passed by reference.
     */
    std::vector<MotionModel *> &listModel();

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides access to one item in the list of motion models.
     *
     * Returns a single MotionModel based object.  Each model will be a different class, but all classes are derived
     * from the MotionModel object.  Each object in the vector is a different class of motion model.
     * @param index Integer.  The index of which item in the vector you want.
     * @return Returns an object of a class derived from the MotionModel class.  Returned variable is passed by
     * reference.
     */
    ofreq::MotionModel &listModel(unsigned int index);

    //-----------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides access to one item in the list of motion models.
     *
     * Returns a single MotionModel based object.  Each model will be a different class, but all classes are derived
     * from the MotionModel object.  Each object in the vector is a different class of motion model.
     * @param modelName String input.  The name of the motion model, as specified by the user.  Must match the
     * predefined name of the model exactly.
     * @return Returns an object of a class derived from the MotionModel class.  Returned variable is passed by
     * reference.
     */
    ofreq::MotionModel &listModel(std::string modelName);

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
	std::string analysisType; /**< The analysis type. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave directions.  Directions measured in radians.  True North is zero, with positive going
     * counter clockwise.
     */
    std::vector<double> pWaveDirections;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of wave frequencies.  Each frequency measured in radians per second.
     */
    std::vector<double> pWaveFrequencies;

    //------------------------------------------Function Separator ----------------------------------------------------
    int pCurWaveDir; /**< The index of the current wave direction. */

    //------------------------------------------Function Separator ----------------------------------------------------
    int pCurWaveFreq; /**< The index of the current wave frequency. */

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of Body objects.  Inputs set by file reader go to this list of Body objects.
     * @sa Body
     */
    std::vector<Body> plistBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of OutputsBody objects.  The inputs set by the file reader on which Output options to calculate
     * go to this list of OutputsBody objects.
     * @sa OutputsBody
     * @sa OutputDerived
     */
    std::vector<OutputsBody> plistOutputs;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector of active force objects defined for the system to use, under the scope of user
     * defined objects.  These forces are defined by the Forces input file.  These objects are normally only
     * retained for the intial phase of the program.  After input reading is complete, these force objects will be
     * copied to their respective Body objects.  The list of force objects can then be safely deleted.
     * @sa Body
     */
    std::vector<ForceActive> plistForceActive_user;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector of reactive force objects defined for the system to use, under the scope of user
     * defined objects.  These forces are defined by the Forces input file.  These objects are normally only
     * retained for the intial phase of the program.  After input reading is complete, these force objects will be
     * copied to their respective Body objects.  The list of force objects can then be safely deleted.
     * @sa Body
     */
    std::vector<ForceReact> plistForceReact_user;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The vector of cross-body force objects defined for the system to use, under the scope of user
     * defined objects.  These forces are defined by the Forces input file.  These objects are normally only
     * retained for the intial phase of the program.  After input reading is complete, these force objects will be
     * copied to their respective Body objects.  The list of force objects can then be safely deleted.
     * @sa Body
     */
    std::vector<ForceCross> plistForceCross_user;

    //Constant Variables declaration
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Designator for the vector of ForceActive class of objects, under the scope of the user.  These are force
     * objects defined by the Forces input file.
     */
    static std::string valForceActive_user;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Designator for the vector of ForceReact class of objects, under the scope of the user.  These are force
     * objects defined by the Forces input file.
     */
    static std::string valForceReact_user;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Designator for the vector of ForceCross class of objects, under the scope of the user.  These are force
     * objects defined by the Forces input file.
     */
    static std::string valForceCross_user;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of motion models available to the system.  Each object in this list is from a different class.
     * All classes are derived from the MotionModel class.
     * Vector must use a list of pointer objects to allow correct polymorphism to overload any functions of derived
     * classes.
     */
    std::vector<ofreq::MotionModel *> plistModels;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Defines all motion models.
     *
     * Adds all defined motion models to the list of available motion models.  Each
     * class of motion model has only one object defined in the list.
     */
    void DefineModels();
};

}   //Namespace ofreq
}   //Namespace osea

#endif
