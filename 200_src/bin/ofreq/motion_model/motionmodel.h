/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date              Author                  Description
 *---------------------------------------------------------------------------------------------------------------------
 *Aug 03 2013       Nicholas Barczak        Initially created
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
#ifndef MOTIONMODEL_H
#define MOTIONMODEL_H
#include <vector>
#include <complex>
#include "../global_objects/body.h"
#include "equationofmotion.h"
#ifdef Q_OS_WIN
    #include "armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "../global_objects/body.h"
#include "../global_objects/ofreqcore.h"

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
 * This class provides the functionality to translate between input coefficients in the body class and the force
 * coefficients in the matBody class.  Most important, it acts as an interface for advanced users to enter their own
 * equations of motion.  This was devised to create a very generic interface that could allow any sort of definition
 * for equations.  The use of functions for the class should use the following sequence.
 * 1.)  Create class:  constructor
 * 2.)  Set body data (if not already done in constructor):  setListBodies
 * 3.)  Set the current body working with:  setBody
 * 4.)  Set the current wave frequency working with:  setFreq
 * 5.)  Set whether calculating coefficients or values (default:  Values):  calcCoefficient
 * 6.)  Reset the forces you wish to use.
 * 7.)  Set the new list of forces you wish to use.:
 *      useForceAct_usr
 *      useForceAct_hydro
 *      useForceReact_usr
 *      useForceReact_hydro
 *      useForceCross_usr
 *      useForceCross_hydro
 *      useForceMass
 * 7.)  Evaluate the motion model to produce a single complex value result.
 */

class MotionModel : oFreqCore
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default constructor.
     *
     * Default constructor.
     */
    MotionModel();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor.  This is the preferred constructor as it supplies the body data.
     * @param listBodIn The vector of the body objects to input.
     */
    MotionModel(std::vector<Body> &listBodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Default destructor.
     */
    virtual ~MotionModel();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Inputs the list of body data.
     * @param listBodIn The vector of body objects to input.
     */
    void setlistBody(std::vector<Body> &listBodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the index for the body that all calculations are based on.
     * @param Integer input specifying the number of the body to use.  Integer corresponds to the sequence of bodies
     * in the vector supplied with the body.
     */
    void setBody(int bod);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the index for the body that all calculations are based on.
     * @return Returns integer specifying the number of the body currently in use.  Integer corresponds to the
     * sequence of bodies in the vector supplied with the body.  If no Body is currently set, the function returns
     * -1.
     */
    int getBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Determines whether the class should calculate force coefficients or actual force values.
     * True = Calculate force coefficients only.
     * False = Calculate force values.
     * Default = (False) Calculate force values.
     * @return Boolean to determine whether should calculate coefficients or values.
     */
    bool &CoefficientOnly();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Boolean to track whether only the active forces are requested.
     *
     * Boolean to track whether only the active forces are requested.  The active forces are included negatively in the
     * equation of motion.  They should be on the opposite side of the equation and included as a positive constant.
     * The final matrix body accomplishes this.  And when only active forces are requested, they should be sent out as
     * positive values.
     * However, when pulling the information out, the signs must be reversed.
     * The boolean variable triggers to determine if this should happen.  If any reactive or cross-body forces are
     * activated as well, this variable is set false.
     * @return Returns boolean variable.  Variable passed by value.
     * Returns true if only active forces are used in the equation of motion.
     * Returns false if any reactive or cross-body forces are used in the equation of motion.
     */
    bool getActiveOnly();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records the index of the body object referenced by the cross body.
     *
     * Records the index of the body object referenced by the cross body.  Each body object contains a list of
     * pointers for the cross-body objects.  Each cross-body force has a pointer associated with it.  This pointer
     * points to another body object.  This allows comparison between memory addresses of different body objects.
     * However, when the body objects are copied over, the pointers are now pointing to different, invalid memory
     * addresses.  to eliminate this problem in the motion model, the model will record the position of the body object
     * in the vector of body objects.  This forms a vector.  Each entry in the vector represents one cross-body force for
     * the current body.  The integer entry in the vector is the integer index of the body that the cross-body force is
     * linked to.
     * @return Returns a vector of integers.  Returned variable is passed by reference.   Each entry in the vector
     * represents one cross-body force for the current body.  The integer entry in the vector is the integer index of
     * the body that the cross-body force is linked to.
     */
    std::vector<int> &listCompCrossBod_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records the index of the body object referenced by the cross body.
     *
     * Records the index of the body object referenced by the cross body.  Each body object contains a list of
     * pointers for the cross-body objects.  Each cross-body force has a pointer associated with it.  This pointer
     * points to another body object.  This allows comparison between memory addresses of different body objects.
     * However, when the body objects are copied over, the pointers are now pointing to different, invalid memory
     * addresses.  to eliminate this problem in the motion model, the model will record the position of the body object
     * in the vector of body objects.  This forms a vector.  Each entry in the vector represents one cross-body force for
     * the current body.  The integer entry in the vector is the integer index of the body that the cross-body force is
     * linked to.
     * @param crossbodIn Integer parameter.  Specified the index of which value to retrieve from the list of values
     * for the CrossBod indices.
     * @return Returns an integer.  Variable passed by reference.  Returned integer is the index of the Body object
     * referenced by the cross-body force located at the index specified by CrossBod.
     * Example:  CrossBod (index) -> (vector of cross body forces) -> Index of Body object that cross-body force
     * points to.
     */
    int &listCompCrossBod_hydro(int crossbodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records the index of the body object referenced by the cross body.
     *
     * Records the index of the body object referenced by the cross body.  Each body object contains a list of
     * pointers for the cross-body objects.  Each cross-body force has a pointer associated with it.  This pointer
     * points to another body object.  This allows comparison between memory addresses of different body objects.
     * However, when the body objects are copied over, the pointers are now pointing to different, invalid memory
     * addresses.  to eliminate this problem in the motion model, the model will record the position of the body object
     * in the vector of body objects.  This forms a vector.  Each entry in the vector represents one cross-body force for
     * the current body.  The integer entry in the vector is the integer index of the body that the cross-body force is
     * linked to.
     * @return Returns a vector of integers.  Returned variable is passed by reference.   Each entry in the vector
     * represents one cross-body force for the current body.  The integer entry in the vector is the integer index of
     * the body that the cross-body force is linked to.
     */
    std::vector<int> &listCompCrossBod_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Records the index of the body object referenced by the cross body.
     *
     * Records the index of the body object referenced by the cross body.  Each body object contains a list of
     * pointers for the cross-body objects.  Each cross-body force has a pointer associated with it.  This pointer
     * points to another body object.  This allows comparison between memory addresses of different body objects.
     * However, when the body objects are copied over, the pointers are now pointing to different, invalid memory
     * addresses.  to eliminate this problem in the motion model, the model will record the position of the body object
     * in the vector of body objects.  This forms a vector.  Each entry in the vector represents one cross-body force for
     * the current body.  The integer entry in the vector is the integer index of the body that the cross-body force is
     * linked to.
     * @param crossbodIn Integer parameter.  Specified the index of which value to retrieve from the list of values
     * for the CrossBod indices.
     * @return Returns an integer.  Variable passed by reference.  Returned integer is the index of the Body object
     * referenced by the cross-body force located at the index specified by CrossBod.
     * Example:  CrossBod (index) -> (vector of cross body forces) -> Index of Body object that cross-body force
     * points to.
     */
    int &listCompCrossBod_user(int crossbodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Resets the class data to have all input coefficients.  Any evaluation after a reset will produce a
     * value of zero.  Force coefficients will be zero and force values will be zero.
     */
    void Reset();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the current operating frequency for the function.  Only necessary when calculating true forces
     * and using derivatives defined in the motion model.  Otherwise, you can safely ignore this function.
     * @param Double precision value that sets the current wave frequency value.
     */
    void setFreq(double freq);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the current operating frequency for the function.  Only necessary when calculating true forces
     * and using derivatives defined in the motion model.  Otherwise, you can safely ignore this function.
     * @return Double precision variable that is the current wave frequency value.  Variable returned by value.
     */
    double getFreq();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     */
    void useForceActive_user(unsigned int force, unsigned int eqn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  With only the force number specified, all equations are used as coefficients.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     */
    void useForceActive_user(unsigned int force);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  All forces and all coefficients are used.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     */
    void useForceActive_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     */
    void useForceActive_hydro(unsigned int force, unsigned int eqn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  With only the force number specified, all equations are used as coefficients.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     */
    void useForceActive_hydro(unsigned int force);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  All forces and all coefficients are used.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     */
    void useForceActive_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     * @param var Integer specifying which variable to use from the selected equation.  Based on data index.
     */
    void useForceReact_user(unsigned int force, unsigned int ord, unsigned int eqn, unsigned int var);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables within the specified equation, derivative, and
     * force.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     */
    void useForceReact_user(unsigned int force, unsigned int ord, unsigned int eqn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within the specified derivative
     * and force.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     */
    void useForceReact_user(unsigned int force, unsigned int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within all derivatives
     * within the specified force
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     */
    void useForceReact_user(unsigned int force);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within all derivatives
     * within all forces available.
     */
    void useForceReact_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     * @param var Integer specifying which variable to use from the selected equation.  Based on data index.
     */
    void useForceReact_hydro(unsigned int force, unsigned int ord, unsigned int eqn, unsigned int var);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables within the specified equation, derivative, and
     * force.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     */
    void useForceReact_hydro(unsigned int force, unsigned int ord, unsigned int eqn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within the specified derivative
     * and force.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     */
    void useForceReact_hydro(unsigned int force, unsigned int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within all derivatives
     * within the specified force
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     */
    void useForceReact_hydro(unsigned int force);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within all derivatives
     * within all forces available.
     */
    void useForceReact_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     * @param var Integer specifying which variable to use from the selected equation.  Based on data index.
     */
    void useForceCross_user(unsigned int force, unsigned int ord, unsigned int eqn, unsigned int var);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables within the specified equation, derivative, and
     * force.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     */
    void useForceCross_user(unsigned int force, unsigned int ord, unsigned int eqn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within the specified derivative
     * and force.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     */
    void useForceCross_user(unsigned int force, unsigned int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within all derivatives
     * within the specified force
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     */
    void useForceCross_user(unsigned int force);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within all derivatives
     * within all forces available.
     */
    void useForceCross_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     * @param var Integer specifying which variable to use from the selected equation.  Based on data index.
     */
    void useForceCross_hydro(unsigned int force, unsigned int ord, unsigned int eqn, unsigned int var);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables within the specified equation, derivative, and
     * force.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     */
    void useForceCross_hydro(unsigned int force, unsigned int ord, unsigned int eqn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within the specified derivative
     * and force.
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     * @param ord Integer specifying which order of derviative to use for the selected force.
     */
    void useForceCross_hydro(unsigned int force, unsigned int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within all derivatives
     * within the specified force
     * @param force Integer specifying which force to use in the set of forces for the given force type.
     */
    void useForceCross_hydro(unsigned int force);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This method uses all coefficients for all variables and all equations within all derivatives
     * within all forces available.
     */
    void useForceCross_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     * @param var Integer specifying which variable to use from the selected equation.  Based on data index.
     */
    void useForceMass(unsigned int eqn, unsigned int var);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This implementation will copy over all variables for the specified equation.
     * @param eqn Integer specifying which equation to use in the selected force.  Based on data index.
     */
    void useForceMass(unsigned int eqn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Passes information to the object to use input coefficients from the entry specified.
     *
     * Passes information to the object to use input coefficients from the entry specified.  Limits inputs to
     * only the force object type specified by the method.  Calls to useForce methods are cumulative.  Sucessive calls
     * to different entries in the same force sequence will add their coefficients to the sets for evaluation.  Can be
     * combined with other useForce methods.  Multiple calls to the same useForce method with the same index coordinates
     * are not cumulative.  An input coefficient can either be on or off, not multiple instances of the exact same
     * coefficient.  This implementation will copy over all variables for all equations.
     */
    void useForceMass();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Evaluates the motion model for a whole range of equations on the specified force.
     *
     * Evaluates the motion model for a whole range of equations on the specified force.  Returns a complex matrix
     * that contains the results of the entire evaluation.
     * @param force Integer specifying which force object to evaluate.  Integer specifies the vector occurrence
     * index of the force.
     * @return Returns a complex matrix that contains the results of the entire evaluation.  Returned argument passed
     * by value.
     */
    arma::cx_mat getMatForceActive_user(int force);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Evaluates the motion model for a whole range of equations on the specified force.
     *
     * Evaluates the motion model for a whole range of equations on the specified force.  Returns a complex matrix
     * that contains the results of the entire evaluation.
     * @param force Integer specifying which force object to evaluate.  Integer specifies the vector occurrence
     * index of the force.
     * @return Returns a complex matrix that contains the results of the entire evaluation.  Returned argument passed
     * by value.
     */
    arma::cx_mat getMatForceActive_hydro(int force);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.
     *
     * Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.  Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     * @param force Integer specifying the force object to use.  Integer specifies the vector occurrence
     * index of the force.
     * @param ord Integer specifying which order of derivative to use on the specified force object.
     * @return Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     */
    arma::cx_mat getMatForceReact_user(int force, int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.
     *
     * Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.  Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     * @param force Integer specifying the force object to use.  Integer specifies the vector occurrence
     * index of the force.
     * @param ord Integer specifying which order of derivative to use on the specified force object.
     * @return Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     */
    arma::cx_mat getMatForceReact_hydro(int force, int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.
     *
     * Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.  Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     * @param force Integer specifying the force object to use.  Integer specifies the vector occurrence
     * index of the force.
     * @param ord Integer specifying which order of derivative to use on the specified force object.
     * @return Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     */
    arma::cx_mat getMatForceCross_user(int force, int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.
     *
     * Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.  Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     * @param force Integer specifying the force object to use.  Integer specifies the vector occurrence
     * index of the force.
     * @param ord Integer specifying which order of derivative to use on the specified force object.
     * @return Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     */
    arma::cx_mat getMatForceCross_hydro(int force, int ord);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.
     *
     * Evaluates the motion model for a whole range of equations and variable on the specified force and order
     * of derivative.  Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     * @return Returns a complex matrix that contains the results of the entire evaluation.  Saves some time
     * on computing effort.
     */
    arma::cx_mat getMatForceMass();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Triggers evaluation of the currently activated set of input coefficients.
     *
     * Triggers evaluation of the currently activated set of input coefficients.  If Calc_Coeff is set to True, then
     * evaluation will only generate the force coefficients from the resulting evaluation.  Otherwise, the evaluation
     * will use the currently defined solution data and evaluate for force values.
     * @param eqn Integer representing which equation object to evaluate.  Integer specifies the Data index of the
     * equation.
     * @return Returns a complex number representing the force under the currently set conditions.
     */
    std::complex<double> Evaluate(int eqn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reports the number of equations used in the motion model.
     *
     * Reports the number of equations used in the motion model.  This lets the matBody object know how many
     * equations to prepare for.
     * @return Returns the number of equations used in the motion model.
     */
    int numEquations();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a vector containing all equation indices.  This may be the same as the number of equations.
     * However, if they are custom equations, they must avoid the first six indices, which
     * are reserved for standard 6dof models.  This means that the data index may not start at zero, which is why the
     * data index vector is returned.  It allows you to see for each entry in the slot, what the index is for that
     * equation.
     * @return Returns a vector containing all the equation indices currently in use.  Returned vector is passed by
     * reference.
     */
    std::vector<int> &listDataIndex();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns an enry from a vector containing all equation indices.
     *
     * The requested entry is specified by the input variable index.  The list of equation data indices may be the
     * same as the number of equations.  However, if they are custom equations, they must avoid the first six
     * indices, which are reeserved for standard 6dof models.  This means that the data index may not start at zero,
     * which is why the entries of the data index vector are exposed for retrieval and manipulation.  It allows you
     * to see for each entry in the slot, which the data index is for that equation.
     * @param index Integer variable.  Passed by value.  Specified the index of which entry in the data index you
     * want to see.  If the requested index is beyond the current limits of the vectors, the vector is automatically
     * resized, but never larger than the number of current equations.  Each entry in the index represents an equation.
     * @return Returns an integer variable.  Variable passed by value.  The returned variable is an entry from the
     * vector of all equation data indices currently in use.
     */
    int &listDataIndex(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Assembles and gets the vector of equation indices.
     *
     * The list of equation indices may be the same as the number of equations.
     * However, if they are custom equations, they must avoid the first six indices, which
     * are reserved for standard 6dof models.  This means that the data index may not start at zero, which is why the
     * data index vector is returned.  It allows you to see for each entry in the slot, what the index is for that
     * equation.  This method also searches through all the included equation objects to retrieve their data index
     * automatically.  So if you have an imcomplete list, this method will automatically complete the list before
     * returning the vector of the complete list of data indices.
     * @return Returns a vector containing all the equation indices currently in use.
     */
    std::vector<int> getDataIndex();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the maximum number of the data index.
     *
     * Returns the maximum number of the data index.  This may be the same as the number of equations.  Very
     * few equations may be used.  However, if they are custom equations, they must avoid the first six indices, which
     * are reserved for standard 6dof models.
     * @return Returns integer number representing the maximum number of the data index found from all equations.
     */
    int MaxDataIndex();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Name for the motion model
     *
     * Name for the motion model.  Used by the user to identify the motion model.
     * @param nameIn The name to set for the motion model.  std::string variable.  Variable passed by value.
     */
    void setName(std::string nameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Name for the motion model
     *
     * Name for the motion model.  Used by the user to identify the motion model.
     * @return The name to set for the motion model.  std::string variable.  Variable passed by value.
     */
    std::string getName();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Description for the motion model.
     *
     * Description for the motion model.  Used by the user to provide a more extensive description of the motion model.
     * Used purely for user information.  Not used for model identification.
     * @param DescIn std::string.  The description for the motion model.  Variable passed by value.
     */
    void setDescription(std::string DescIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Description for the motion model.
     *
     * Description for the motion model.  Used by the user to provide a more extensive description of the motion model.
     * Used purely for user information.  Not used for model identification.
     * @return std::string.  The description for the motion model.  Variable passed by value.
     */
    std::string getDescription();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of Bodies referenced by the motion model.
     * @return Reference to vector of Body objects.  Variable passed by reference.
     * @sa Body
     */
    std::vector<Body> &listBody();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Direct access to an individual Body from the list of Bodies contained in the motion model.
     * @param bodIn Integer specifying which Body object to access in the list of Bodies.
     * @return Returns reference to the Body object specified by input bodIn.
     * @sa listBody()
     */
    Body &listBody(int bodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of Bodies used as data for the motion model.
     * @return Reference to the vector of Body objects used as data.  Variable passed by reference.
     * @sa Bodyy
     */
    std::vector<Body> &listData();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Direct access to an individual Body from the list of Data contained in the motion model.
     * @param dataIn Integer specifying which Body object to access from the list of Data.
     * @return Returns reference to the Body object specified by dataIn.
     * @sa listData()
     */
    Body &listData(int dataIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of equation of motion objects used in the motion model.
     * @return Reference to the vector of EquationofMotion objects.  Variable passed by reference.  Each entry in the
     * vector is a pointer to the relevent equation of motion object.
     * @sa EquationofMotion
     */
    std::vector<EquationofMotion *> &listEquation();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Direct access to an individual EquationofMotion object from the list of Equations contained in the motion
     * model.
     * @param eqIn Integer specifying which EquationofMotion object to access from the list of Equations
     * @return Returns reference to the EquationofMotion object specified by eqIn.
     */
    EquationofMotion &listEquation(int eqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Provides direct access to the list of equation of motion objects used in the motion model.
     *
     * This is the same as the listEquation() function, but just under a different name.
     * @return Reference to the vector of EquationofMotion objects.  Variable passed by reference.  Each entry in the
     * vector is a pointer to the relevent equation of motion object.
     * @sa EquationofMotion
     * @sa MotionModel::listEquation()
     */
    std::vector<EquationofMotion *> &listDataEquation();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Direct access to an individual EquationofMotion object from the list of Equations contained in the motion
     * model.
     *
     * This function specifies the requested equation through the use of the equation's data index.  Not its normal
     * occurrence index in the vector.  The data index is the number of the data this equation will retrieve, not its
     * sequence in the vector that stores it.
     * @param eqIn Integer.  The data index of the EquationofMotion object requested.
     * @return Returns reference to the EquationofMotion object specified by eqIn.  Returned variable passed by
     * reference.
     */
    EquationofMotion &listDataEquation(int eqIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief This adds an equation of motion to the motion model.
     *
     * Adds the equation of motion on to the end of the vector of equation of motions.  Also works for any objects
     * derived from the EquationofMotion object, which is how this method should really be used.
     * @param eqIn EquationfMotion object.  The object that you want to add to the list of equations of motion.
     * Also works for any object classes derived from the EquationOfMotion.  Variable passed by value, so it will
     * make a copy of the input variable.
     * @sa DefineEquations()
     */
    void AddEquation(EquationofMotion *eqIn);

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The function used to define the equation of motion objects.
     *
     * This function gets executed when the Motion model is first created.  It contains all the statements to add
     * the appropriate equations to the motion model.  This is where each EquationofMotion object is created.  Can
     * also be any object from a class that is derived from the EquationofMotion object.  Before definining equations
     * within this motion model, the individual equation must be defined.  These will be new objects inheritted from
     * the EquationofMotion object. Once those new equation clases are defined, they can be used in the motion model.
     *
     * Using an EquationofMotion in the motion model will generally follow the following sequence.  All step are
     * executed within the DefineEquations function.
     * 1.)  Create a new object from the appropriate class which is derived from the EquationofMotion.  When creating
     *      the equation of motion, you must include the pointer to the existing motion model.  Use the keyword this
     *      when creating the new object.
     * 2.)  Set the data index for the equation.  This is probably the most important step.
     *      Regardless of what name you give the equation, the program ofreq only sees the equation as one in a list
     *      of equations, and refers to it by its index within that list.  Any input data (such as hydrodynamic
     *      or user coefficients) is similarly referenced by that index.  When you set the data index, you tell
     *      ofreq which index in the list of data is has available should correspond to this specific equation.
     *      (Set the data index using the function setDataIndex().
     * 3.)  Set the name for the new object.  This is just the short name or equation symbol. (Use the function
     *      setName() to set it.)
     * 4.)  Set the description for the new object.  This is the more extensive name for the equation.  (Use the
     *      function setDescription() to set it.)
     * 5.)  Now that you set all the appropriate information, add the equation of motion into the list of equations
     *      used by this motion model.  (Use the function AddEquation()).
     *
     * @sa AddEquation()
     * @sa EquationofMotion
     */
    virtual void DefineEquations();

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Name for the motion model
     *
     * Name for the motion model.  Used by the user to identify the motion model.
     */
    std::string pName;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Description for the motion model.
     *
     * Description for the motion model.  Used by the user to provide a more extensive description of the motion model.
     * Used purely for user information.  Not used for model identification.
     */
    std::string pDesc;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Pointer to the bodies vector.
     *
     * Pointer to the bodies vector.
     */
    std::vector<ofreq::Body> *plistBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Vector containing the currently activated body data.  A call to the Reset method copies all data over
     * to this list and fills it with Body objects that contain the same number and sizes of forces as the originals,
     * but all entries within the copied objects are zeros.
     */
    std::vector<ofreq::Body> plistData;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Current wave frequency.  Used for calculating derivatives.
     */
    double pFreq;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of equations of motion to use in the motion model.  The sequence of equations in the list is
     * important.  Equations appear in the list in the order as they appear in the matrix model of the body.
     * Vector must use a list of pointer objects to allow correct polymorphism to overload the setFormula() function
     * of derived classes and have that derived class formula used.
     * @sa setFormula()
     */
    std::vector<EquationofMotion*> plistEquations;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets whether the force coefficients should be calculated, or values of response.
     */
    bool pcalcCoeff;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Copies the matrix and outputs a matrix that is the same size, but filled with zeros.
     * @param The matrix to copy.  Matrix of double precision floating numbers.
     */
    arma::Mat<double> CopyZero(arma::Mat<double> &copyMat);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The current body to perform calculations on.
     */
    int curBody;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Checks to make sure the number of bodies matches the input data.  If not, fills out the body data.
     *
     * Checks to make sure the number of bodies matches the input data.  If not, fills out the body data.  Operates
     * on listData vector.  Adds items to the vector and assigns basic properties to those items.
     */
    void fillBodies();

    //------------------------------------------Function Separator --------------------------------------------------------
    /**
     * @brief Records the index of the body object referenced by the cross body.
     *
     * Records the index of the body object referenced by the cross body.  Each body object contains a list of
     * pointers for the cross-body objects.  Each cross-body force has a pointer associated with it.  This pointer
     * points to another body object.  This allows comparison between memory addresses of different body objects.
     * However, when the body objects are copied over, the pointers are now pointing to different, invalid memory
     * addresses.  to eliminate this problem in the motion model, the model will record the position of the body object
     * in the vector of body objects.  This forms a vector.  Each entry in the vector represents one cross-body force for
     * the current body.  The integer entry in the vector is the integer index of the body that the cross-body force is
     * linked to.
     */
    std::vector<int> pCompCrossBod_hydro;

    //------------------------------------------Function Separator --------------------------------------------------------
    /**
     * @brief Records the index of the body object referenced by the cross body.
     *
     * Records the index of the body object referenced by the cross body.  Each body object contains a list of
     * pointers for the cross-body objects.  Each cross-body force has a pointer associated with it.  This pointer
     * points to another body object.  This allows comparison between memory addresses of different body objects.
     * However, when the body objects are copied over, the pointers are now pointing to different, invalid memory
     * addresses.  to eliminate this problem in the motion model, the model will record the position of the body object
     * in the vector of body objects.  This forms a vector.  Each entry in the vector represents one cross-body force for
     * the current body.  The integer entry in the vector is the integer index of the body that the cross-body force is
     * linked to.
     */
    std::vector<int> pCompCrossBod_user;

    //------------------------------------------Function Separator --------------------------------------------------------
    /**
     * @brief Boolean to track whether only the active forces are requested.
     *
     * Boolean to track whether only the active forces are requested.  The active forces are included negatively in the
     * equation of motion.  They should be on the opposite side of the equation and included as a positive constant.
     * The final matrix body accomplishes this.  And when only active forces are requested, they should be sent out as
     * positive values.
     * However, when pulling the information out, the signs must be reversed.
     * The boolean variable triggers to determine if this should happen.  If any reactive or cross-body forces are
     * activated as well, this variable is set false.
     */
    bool pActiveOnly;

    //------------------------------------------Function Separator --------------------------------------------------------
    /**
     * @brief A vector containing all equation indices.  This may be the same as the number of equations.
     * However, if they are custom equations, they must avoid the first six indices, which
     * are reserved for standard 6dof models.  This means that the data index may not start at zero, which is why the
     * data index vector is provided.  It allows you to see for each entry in the slot, what the index is for that
     * equation.
     */
    std::vector<int> pDataIndex;

    //------------------------------------------Function Separator --------------------------------------------------------
    /**
     * @brief Executes setup actions that are common to all versions of the object constructor.
     */
    void ConstructorCommon();

    //------------------------------------------Function Separator --------------------------------------------------------
    /**
     * @brief Searches through the list of EquationofMotion objects to find the one specified by the indexIn parameter.
     *
     * Searches through the list of EquationofMotion objects to find the one specified by the indexIn parameter.
     * Returns the integer specifying the position of the object in the vector of equation objects.
     * @param indexIn The data index requested.
     * @return Returns the integer specifying the position of the object in the vector of equation objects.
     */
    int findIndex(int indexIn);
};

}   //Namespace ofreq
}   //Namespace osea

#endif // MOTIONMODEL_H
