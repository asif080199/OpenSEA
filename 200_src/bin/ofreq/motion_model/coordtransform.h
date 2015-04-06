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
 *Mar 28, 2015      Nicholas Barczak        Initially created
 *
\*-------------------------------------------------------------------------------------------------------------------*/

//License
/*-------------------------------------------------------------------------------------------------------------------*\
 *Copyright Datawave Marine Solutions, 2015.
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
#ifndef COORDTRANSFORM_H
#define COORDTRANSFORM_H
#include "../system_objects/ofreqcore.h"
#include <vector>
#include <complex>
#ifdef Q_OS_WIN

#elif defined Q_OS_LINUX

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
//Prototype class declarations
class MotionModel;


//######################################### Class Separator ###########################################################
/**
 * @brief The CoordTransform class shifts variables for a motion model for correct input into the global coordinate
 * system.
 *
 * The user is permitted to define all body variables relative to a body coordinate system.  This body coordinate
 * system must first be converted into a global coordinate system before any equations of motion can be solved.  This
 * requirement is necessary for multi-body problems.  However, the introduction of custom equations of motion
 * presents a serious problem.  Because the equations of motion can be customized, the programmer can not know
 * how the coordinate transforms apply to each variable in the motion model.  Instead, for each variable, the
 * user must use the coordinate transform class to define one important function:
 * GlobtoBod:   Transform varaible from global coordinates to body coordiantes
 *
 * The user has access to four things defined for each body:
 * dx:  Body translation in X-axis for body coordinate system.
 * dy:  Body translation in Y-axis for body coordinate system.
 * dz:  Body translation in Z-axis for body coordinate system.
 * rot: Body rotation in Z-axis for body coordinate system.
 *
 * Note that all other segments of oFreq assume that the coordinate transform happens with translation first, followed
 * by rotation of the body.
 *
 * This defines the base class that allows the user to access these items and define the necessary functions.
 *
 */
class CoordTransform : public oFreqCore
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
    CoordTransform(MotionModel *modelIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Contrustor with name and index.  Contains a reference to the motion model class which constructs it.
     *
     * Default contrustor.  Contains a reference to the motion model class which constructs it.  The constructing class
     * is necessary because several functions in the EquationOfMotion class use data in the constructing class, the
     * motion model class.
     * @param modelIn A pointer to the motion model object that created the equation of motion.
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
    CoordTransform(MotionModel *modelIn, int IndexIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the index for the equation of motion.
     *
     * Sets the index for the equation of motion.  The index is how the equation determines which numbers to access
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
     * @param DataIn The integer of the data index to use.
     */
    void setDataIndex(int DataIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the index for the equation of motion.
     *
     * Gets the index for the equation of motion.  The index is how the equation determines which numbers to access
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
     * @return Returns an integer number representing the data index used by the equation.
     */
    int getDataIndex();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the index for the equation of motion.
     *
     * Gets the index for the equation of motion.  The index is how the equation determines which numbers to access
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
     * @return Returns a reference to the protected data index variable contained in the class.
     */
    int &refDataIndex();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Transforms the input variable from global coordinate system to Body coordinate system.
     * @return Returns double, passed by value.  Equation of motion variable, converted to Body coordinate system.
     */
    virtual std::complex<double> GlobtoBod();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Clears the list of variables provided.
     */
    void clearVars();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a vector of variables to the list of input variables.
     *
     * All added variables are in global coordinate system.
     * @param varsIn Vector of complex, double variables.  Variables passed by value.  The list of variables to add
     * to the list.
     */
    void addVars(std::vector<std::complex<double> > varsIn);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Adds a single variable to the list of input variables.
     *
     * All added variabes are in global coordinate system.
     * @param varsIn Complex, double value.  Variable passed by value.  A single variable value to add to the list.
     */
    void addVars(std::complex<double> varsIn);


//==========================================Section Separator =========================================================
protected:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Translation of coordinate system in the X-axis (global X-axis)
     * @param bodIn Integer, variable passed by value.  The index of the body that you are requesting translation
     * for.  Default entry will request the body index for the current body.  Index is numbered starting from 1.
     * @return Returns the distance of translation of body coordinate system in the X-axis.
     */
    std::complex<double> T_x(int bodIn = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Translation of coordinate system in the Y-axis (global Y-axis)
     * @param bodIn Integer, variable passed by value.  The index of the body that you are requesting translation
     * for.  Default entry will request the body index for the current body.  Index is numbered starting from 1.
     * @return Returns the distance of translation of body coordinate system in the Y-axis.
     */
    std::complex<double> T_y(int bodIn = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Translation of coordinate system in the Z-axis (global Z-axis)
     * @param bodIn Integer, variable passed by value.  The index of the body that you are requesting translation
     * for.  Default entry will request the body index for the current body.  Index is numbered starting from 1.
     * @return Returns the distance of translation of body coordinate system in the Z-axis.
     */
    std::complex<double> T_z(int bodIn = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Rotation of coordinate system about the Z-axis (global z-axis)
     * @param bodIn Integer, variable passed by value.  The index of the body that you are requesting heading
     * for.  Default entry will request the body index for the current body.  Index is numbered starting from 1.
     * @return Returns the angle of rotation of body coordinate system about the Z-axis.  Rotation angle is in
     * units of radians.
     */
    std::complex<double> R_z(int bodIn = 0);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the body constant for the requested index.
     *
     * Each Body object has a set of constants associated with it.  These constants can be anything that the user
     * desires.  They are a vector of constants.  They are referenced simply in the order of the sequence that the
     * user enters them.  To use these, the user would specify a constant (by index) in the equation of motion or
     * coordinate transform.  The user then enters that constant in the same sequence in the body input definition.
     * @param index Integer, variable passed by value.  The index of the constant that you want to retrieve from the
     * body.
     * @param bodIn Integer, variable passed by value.  The index of the body that you are requesting translation
     * for.  Default entry will request the body index for the current body.  Index is numbered starting from 1.
     * @return Returns single double, passed by value.  The body constant specified by the index.
     */
    std::complex<double> BodConst(int index, int bodIn = 0);


    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Function to access the variables supplied to the conversion equation.
     * @param varIn Integer, variable passed by value.  Specifies the index of the variable to retrieve.  Index
     * specified in human numbering system, starting from 1.
     * @return Returns complex double, variable passed by value.  This is the value of the variable.
     */
    std::complex<double> var(unsigned int varIn);

//==========================================Section Separator =========================================================
private:
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Reference to the motion model object which created the object.
     *
     * Reference to the motion model object which created the object.  This is necessary because the equation of motion
     * object needs to access several data members that are on the motion model which called it.
     */
    MotionModel* pParentModel;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief The list of variables, in global coordinate system.
     *
     * The list of the actual values of the variables, in the global coordinate system.
     */
    std::vector< std::complex<double> > plistVars;

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Integer that stores the equation index.  Used to retrieving data from the calling object motion model.
     * Reserved indices are:
     * 1:  Translation in x-direction.
     * 2:  Translation in y-direction.
     * 3:  Translation in z-direction.
     * 4:  Rotation about x-direction.
     * 5:  Rotation about y-direction.
     * 6:  Rotation about z-direction.
     */
    int pDataIndex;

};

}   //end of namespace ofreq

}   //end of namespace osea

#endif // COORDTRANSFORM_H
