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
#ifndef MODEL6DOF_H
#define MODEL6DOF_H
#include <QObject>
#ifdef Q_OS_WIN
    //Include statements specific to windows
#elif defined Q_OS_LINUX
    //Include statements specific to linux
#endif
#include "motionmodel.h"
#include "eqntranslation.h"
#include "eqnrotation.h"

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
//Prototype class declarations


//######################################### Class Separator ###########################################################
/**
 * @brief The motion model for standard six-degree of freedom rigid-body dynamics problems.
 *
 * --------------------------------------------------------------------------------------------------------------------
 *
 * This is where each EquationofMotion object is created.  Can
 * also be any object from a class that is derived from the EquationofMotion object.  Before adding equations
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
 * Once you define all the equations, you will also want to define a name for your motion model.  This is the
 * name you will use to select the motion model within the input files.  It can be any sequence you want and
 * include spaces.  One name is already reserved as part of the standard program models.  You can not use
 * the following name:
 * "6DOF" - Reserved.*
 *
 * @sa AddEquation()
 * @sa EquationofMotion
 */
class Model6DOF : public MotionModel
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The default constructor
     */
    Model6DOF();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Constructor.  This is the preferred constructor as it supplies the body data.
     * @param listBodIn The vector of the body objects to input.
     */
    Model6DOF(std::vector<Body> &listBodIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * Default destructor.
     */
    ~Model6DOF();

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
     * Once you define all the equations, you will also want to define a name for your motion model.  This is the
     * name you will use to select the motion model within the input files.  It can be any sequence you want and
     * include spaces.  One name is already reserved as part of the standard program models.  You can not use
     * the following name:
     * "6DOF" - Reserved.
     *
     *
     * @sa AddEquation()
     * @sa EquationofMotion
     */
    void DefineEquations();

//==========================================Section Separator =========================================================
private:

};

}   //End of namespace ofreq

}   //End of namespace osea

#endif // MODEL6DOF_H
