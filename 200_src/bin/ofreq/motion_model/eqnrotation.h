/*----------------------------------------*- C++ -*------------------------------------------------------------------*\
| O pen         | OpenSea: The Open Source Seakeeping Suite                                                           |
| S eakeeping	| Web:     www.opensea.dmsonline.us                                                                   |
| E valuation   |                                                                                                     |
| A nalysis     |                                                                                                     |
\*-------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------*\
 *Revision History
 *---------------------------------------------------------------------------------------------------------------------
 *Date          Author				Description
 *---------------------------------------------------------------------------------------------------------------------
 *Nov 2, 2013   Nicholas Barczak    Initially created
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
#ifndef EQNROTATION_H
#define EQNROTATION_H
#include <QObject>
#ifdef Q_OS_WIN
    //Include statements specific to windows
#elif defined Q_OS_LINUX
    //Include statements specific to linux
#endif
#include "equationofmotion.h"

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
 * @brief The EqnRotation class.
 *
 * This class defines motion for rotation in one of the three principle axes.  (X-axis, Y-axis, Z-axis).
 * This is specific to rigid body motion and intended for applicaiton in a six-degree of freedom (6DOF) motion model.
 *
 * --------------------------------------------------------------------------------------------------------------------
 * This class is a derived equation of motion.  It inherits all the programming
 * needed to define an equation of motion from the class EquationofMotion.
 *
 * Custom equation classes serve a specific purpose.  All the programming necessary to define an equation of motion
 * within the program ofreq is already contained within the class EquationofMotion.  All that programming gets
 * inherited from the class.  The only thing the custom class needs to define is the specific equation for this
 * purpose.  That equation is contained inside the function setFormula().  Everything else is already defined, as
 * far as programming.
 *
 * There are specific requirements for how to define the custom EquationofMotion class.  These instructions assume
 * the user has some basic experience with C++ programming.  If not, first try a few tutorials.  There are numerous
 * tutorials on the internet.  You should specifically familiarize yourself with concepts such as namespaces,
 * classes, objects, and functions.  Every effort was made to isolate the user from the complexity of the ofreq
 * program, but some basic understand of program operation is needed.
 *
 * Assuming you have a basic knowledge of C++ programming, you can create your custom equation of motion by editing
 * the .cpp file associated with this class.  Follow these basic rules:
 *
 * 1.)  Always create a backup of every file before you edit it.
 *
 * 2.)  Only edit the .cpp file.  Leave the header file alone (the .h file).  The header file creates the linking
 *      necessary to make everything work correctly.
 *
 * 3.)  Only define your equation within the function setFormula().
 *
 * 4.)  Do not define your own functions unless you are familiar with C++ class programming and know how to safely
 *      add the function into the class definition.
 *
 * 5.)  There are several functions inheritted from the EquationofMotion class.  You can use these to refer to the
 *      different forces when developing your own equation of motion.  oFreq recognizes seven (7) basic force types
 *      shown below, with the function name to reference them in the EquationofMotion. (Arguments for each function
 *      are not shown, for sake of clarity.
 *      5.1) ForceMass(...) = The forces associated with the mass of an object.  This includes direct mass for
 *           straight linear motion, and moment of inertia for rotational motion.
 *      5.2) ForceActive_hydro(...) = The forces which are independant of body motions.  The hydro subcategory
 *           refers to active forces that specifically come from hydrodynamic forces.  This includes the forces
 *           from incident waves.  Sometimes call the Froude-Krylov forces.
 *      5.3) ForceActive_user(...) = The forces which are independant of body motions.  The user subcategory
 *           refers to active forces specifically defined by the user in the ofreq run file.  These may be
 *           some external force such as an active control system.  Regardless, it is customed defined by the user.
 *      5.4) ForceReact_hydro(...) = The forces which are reactive and dependant on body motions.  This includes
 *           derivatives of body motions.  The hydro subcategory refers to reactive forces hydrodynamic in
 *           origin.  This would include body hydrostatic properties, added damping, and added mass.
 *      5.5) ForceReact_user(...) = The forces which are reactive and dependant on body motions.  This includes
 *           derivatives of body motions.  The user subcategory refers to reactive forces defined by the user.
 *           This might include external forces such as a mooring line or dynamic positioning system.  In any
 *           case, these are reactive forces defined at run time in the ofreq input files.
 *      5.6) ForceCross_hydro(...) = The forces which are reactive and dependant on the body motions of another
 *           body.  This is only applicable to multi-body systems.  Examples might be two vessels near each other.
 *           The program can accept equations that use the cross-body forces but are only applied to a single
 *           body problem.  The hydro subcategory refers to reactive forces hydrodynamic in
 *           origin.  This would include body hydrostatic properties, added damping, and added mass, except that
 *           these forces would be dependant on the motions of another body.
 *      5.7) ForceCross_user(...) = The forces which are reactive and dependant on the body motions of another
 *           body.  This is only applicable to multi-body systems.  Examples might be two vessels near each other.
 *           The program can accept equations that use the cross-body forces but are only applied to a single
 *           body problem.  The user subcategory refers to reactive forces defined by the user.
 *           This might include external forces such as a mooring line or dynamic positioning system.  In any
 *           case, these are reactive forces defined at run time in the ofreq input files.
 *
 * @sa EquationofMotion
 * @sa MotionModel
 */
class EqnRotation : public EquationofMotion
{
//==========================================Section Separator =========================================================
public:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default contrustor.  Contains a reference to the motion model class which constructs it.
     *
     * Default contrustor.  Contains a reference to the motion model class which constructs it.  The constructing class
     * is necessary because several functions in the EquationOfMotion class use data in the constructing class, the
     * motion model class.
     * @param modelIn A pointer to the motion model object that created the equation of motion.
     */
    EqnRotation(MotionModel *modelIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Contrustor with name.  Contains a reference to the motion model class which constructs it.
     *
     * Default contrustor.  Contains a reference to the motion model class which constructs it.  The constructing class
     * is necessary because several functions in the EquationOfMotion class use data in the constructing class, the
     * motion model class.
     * @param modelIn A pointer to the motion model object that created the equation of motion.
     * @param NameIn A name for what physical property the equation solves for.  Used for user output.  Not critical
     * to program execution.
     */
    EqnRotation(MotionModel *modelIn, std::string NameIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Contrustor with name and index.  Contains a reference to the motion model class which constructs it.
     *
     * Default contrustor.  Contains a reference to the motion model class which constructs it.  The constructing class
     * is necessary because several functions in the EquationOfMotion class use data in the constructing class, the
     * motion model class.
     * @param modelIn A pointer to the motion model object that created the equation of motion.
     * @param NameIn A name for what physical property the equation solves for.  Used for user output.  Not critical
     * to program execution.
     * @param IndexIn Sets the index for the Equation of Motion.  The index is how the equation determines which numbers to access
     * on the data.  The following indices are used.  Any higher indices can extend beyond this range, and the program
     * easily adapts.  But the following three are reserved.  Unused indices are not transferred to the matrices when
     * solved.  So unused indices to not negatively impact calculation performance.  However, using excessively large
     * indices (say 500 when you only have 3 equations) will result in large matrices and unecessary memory
     * requirements.  THe following index reservations apply.
     * 1:  Translation in x-direction.  Specific to rigid body motion.
     * 2:  Translation in y-direction.  Specific to rigid body motion.
     * 3:  Translation in z-direction.  Specific to rigid body motion.
     * 4:  Rotation about x-direction.  Specific to rigid body motion.
     * 5:  Rotation about y-direction.  Specific to rigid body motion.
     * 6:  Rotation about z-direction.  Specific to rigid body motion.
     */
    EqnRotation(MotionModel *modelIn, std::string NameIn, int IndexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Default destructor.
     */
    ~EqnRotation();

//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The formula used by the equation of motion.
     *
     * The formula used by the equation of motion.  The formula gets rewritten in a unique form.  Rearrange any
     * equations so that they have zero on the right hand size.
     *
     * Example:  If the formula were Ax + By = F, it must be rearranged to:  Ax + By - F = 0
     *
     * The formula can also make use of several math functions provided by the equation of motion object.
     */
    std::complex<double> setFormula();

//==========================================Section Separator =========================================================
private:

};

}   //End of Namespace ofreq

}   //End of Namespace osea

#endif // EQNROTATION_H
