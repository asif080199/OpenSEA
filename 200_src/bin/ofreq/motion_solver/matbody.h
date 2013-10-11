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
 *May 15, 2013  Shane Honanie       Initially created
 *Aug 01 2013   Nicholas Barczak    Added header comments
 *Aug 02 2013   Nicholas Barczak    Recreated new body definition around matrices.
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
#ifndef MATBODY_H
#define MATBODY_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <QtGlobal>
#ifdef Q_OS_WIN
    #include "armadillo.h"  //References the armadillo library in lib folder.
#elif defined Q_OS_LINUX
    #include <armadillo>    //Armadillo library included with standard system libraries.
#endif
#include "../global_objects/body.h"
#include "../global_objects/derivative.h"
#include "../global_objects/ofreqcore.h"
#include "matforcereact.h"
#include "matforceactive.h"
#include "matforcecross.h"

using namespace arma;
using namespace std;

typedef complex<double> complexDouble;

//######################################### Class Separator ###########################################################
/**
 * This class holds all data for a body and related force matrices.  The matBody class contains data defined in a pure
 * mathematical context.  It is prepared for combination and solution.  User interface items, such as relative
 * coordinates and body names are stripped out.  Definitions of equations, derivatives, and variables are replaced
 * by sets of matrices.  The body still contains force objects, but the objects are defined in terms of matrices.
 * Each force object contains a vector of matrices, denoted by the derivative property.  Each matrix within that vector
 * represents a derivative, starting with order 0 (index 0 in the vector.)  Each matrix is organized so that rows =
 * equations, and columns = variables.
 *
 * The class definition also includes a number and size property.  The number denotes the body's position within
 * a vector of bodies.  The size denotes the number of equations used.  This in turn notes how big the matrices must
 * be to accomodate any forces from the body.
 */

class matBody : public oFreqCore
{
//==========================================Section Separator =========================================================
public: 
    //------------------------------------------Function Separator ----------------------------------------------------
    matBody(); /**< The default constructor. */

    //------------------------------------------Function Separator ----------------------------------------------------
    ~matBody(); /**< The default destructor, nothing happens here. */
	
    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the force id number for the object.
     *
     * This is similar to the name parameter in other force objects.  It is an identifier.  In this case, a numerical
     * identifier.  Normally correlates to the objects index in a vector of other objects of the same class.
     * @param num The integer number to input as the objects integer id.
     */
    void setId(int num);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns the force id number for the object.
     *
     * This is similar to the name parameter in other force objects.  It is an identifier.  In this case, a numerical
     * identifier.  Normally correlates to the objects index in a vector of other objects of the same class.
     * @return Returns the force id number, integer data type.
     */
    int getId();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Sets the integer id of the motion model used by the matBody object.
     *
     * Sets the integer id of the motion model used by the matBody object.
     * @param num Integer.  The integer id of the motion model used by the matBody object.  Variable is passed by value.
     */
    void setModelId(int num);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Gets the integer id of the motion model used by the matBody object.
     *
     * Gets the integer id of the motion model used by the matBody object.
     * @return Returns the integer id of the motion model used by the matBody object.  Variable is passed by value.
     */
    int getModelId();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the mass matrix.
     *
     * Returns a reference to the mass matrix.
     * @return Returns a reference to the mass matrix.  Variable passed by reference.
     */
    cx_mat &refMass();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the Reactive Force, user objects.
     *
     * Returns a reference to the Reactive Force, user objects.  This is a vector list of the Reactive Force objects.
     * Provides direct access to the variable and all the member functions of the vector class.
     * @return This is a vector list of the Reactive Force objects. Provides direct access to the variable and all
     * the member functions of the vector class.  Variable passed by reference.
     */
    vector<matForceReact> &listForceReact_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the Reactive Force, user object specified by the index.
     *
     * This is a single item from the vector list of the Reactive Force objects.
     * Provides direct access to the variable.
     * @param index Unsigned integer.  Index to specify which variable retrieve from the vector.
     * @return Returns matForceReact object specified by index.  Returned variable passed by reference.
     * @sa matForceReact
     */
    matForceReact &listForceReact_user(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the Cross-Body Force, user objects.
     *
     * Returns a reference to the Cross-Body Force, user objects.  This is a vector list of the Cross-Body Force objects.
     * Provides direct access to the variable and all the member functions of the vector class.
     * @return This is a vector list of the Cross-Body Force objects. Provides direct access to the variable and all
     * the member functions of the vector class.  Variable passed by reference.
     */
    vector<matForceCross> &listForceCross_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
    * @brief Returns a reference to the Cross-Body Force, user object specified by the index.
    *
    * This is a single item from the vector list of the Cross-Body Force objects.
    * Provides direct access to the variable.
    * @param index Unsigned integer.  Index to specify which variable retrieve from the vector.
    * @return Returns matForceCross object specified by index.  Returned variable passed by reference.
    * @sa matForceCross
    */
    matForceCross &listForceCross_user(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the Active Force, user objects.
     *
     * Returns a reference to the Active Force, user objects.  This is a vector list of the Active Force objects.
     * Provides direct access to the variable and all the member functions of the vector class.
     * @return This is a vector list of the Active Force objects. Provides direct access to the variable and all
     * the member functions of the vector class.  Variable passed by reference.
     */
    vector<matForceActive> &listForceActive_user();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
    * @brief Returns a reference to the Active Force, user object specified by the index.
    *
    * This is a single item from the vector list of the Active Force objects.
    * Provides direct access to the variable.
    * @param index Unsigned integer.  Index to specify which variable retrieve from the vector.
    * @return Returns matForceActive object specified by index.  Returned variable passed by reference.
    * @sa matForceActive
     */
    matForceActive &listForceActive_user(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the Reactive Force, hydro objects.
     *
     * Returns a reference to the Reactive Force, hydro objects.  This is a vector list of the Reactive Force objects.
     * Provides direct access to the variable and all the member functions of the vector class.
     * @return This is a vector list of the Reactive Force objects. Provides direct access to the variable and all
     * the member functions of the vector class.  Variable passed by reference.
     */
    vector<matForceReact> &listForceReact_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the Reactive Force, hydro object specified by the index.
     *
     * This is a single item from the vector list of the Reactive Force objects.
     * Provides direct access to the variable.
     * @param index Unsigned integer.  Index to specify which variable retrieve from the vector.
     * @return Returns matForceReact object specified by index.  Returned variable passed by reference.
     * @sa matForceReact
     */
    matForceReact &listForceReact_hydro(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the Cross-Body Force, hydro objects.
     *
     * Returns a reference to the Cross-Body Force, hydro objects.  This is a vector list of the Cross-Body Force objects.
     * Provides direct access to the variable and all the member functions of the vector class.
     * @return This is a vector list of the Cross-Body Force objects. Provides direct access to the variable and all
     * the member functions of the vector class.  Variable passed by reference.
     */
    vector<matForceCross> &listForceCross_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
    * @brief Returns a reference to the Cross-Body Force, hydro object specified by the index.
    *
    * This is a single item from the vector list of the Cross-Body Force objects.
    * Provides direct access to the variable.
    * @param index Unsigned integer.  Index to specify which variable retrieve from the vector.
    * @return Returns matForceCross object specified by index.  Returned variable passed by reference.
    * @sa matForceCross
    */
    matForceCross &listForceCross_hydro(unsigned int index);

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
     * @brief Returns a reference to the Active Force, hydro objects.
     *
     * Returns a reference to the Active Force, hydro objects.  This is a vector list of the Active Force objects.
     * Provides direct access to the variable and all the member functions of the vector class.
     * @return This is a vector list of the Active Force objects. Provides direct access to the variable and all
     * the member functions of the vector class.  Variable passed by reference.
     */
    vector<matForceActive> &listForceActive_hydro();

    //------------------------------------------Function Separator ----------------------------------------------------
    /**
    * @brief Returns a reference to the Active Force, hydro object specified by the index.
    *
    * This is a single item from the vector list of the Active Force objects.
    * Provides direct access to the variable.
    * @param index Unsigned integer.  Index to specify which variable retrieve from the vector.
    * @return Returns matForceActive object specified by index.  Returned variable passed by reference.
    * @sa matForceActive
     */
    matForceActive &listForceActive_hydro(unsigned int index);


//==========================================Section Separator =========================================================
protected:

//==========================================Section Separator =========================================================
private:
    /**
     * @brief the number of the object in the outside vector that contains it.
     *
     * This is similar to the name parameter in other body objects.  It is an identifier.  In this case, a numerical
     * identifier.  Normally correlates to the objects index in a vector of other objects of the same class.
     */
    int pId;

    /**
     * @brief Holds the integer id of the motion model used by the matBody object.
     */
    int pModelId;

    //Body Mass Matrix
    cx_mat pMass; /**< The Mass Matrix. */

    //User Force Coefficient Matrices
    vector<matForceReact> plistForceReact_user; /**< List of user reactive force matrices. */
    vector<matForceCross> plistForceCross_user; /**< List of user cross body force matrices. */
    vector<matForceActive> plistForceActive_user; /**< List of user active force matrices. */

    //Hydro Force Coefficient Matrices
    vector<matForceReact> plistForceReact_hydro; /**< List of hydro reactive force matrices. */
    vector<matForceCross> plistForceCross_hydro; /**< List of hydro cross body force matrices. */
    vector<matForceActive> plistForceActive_hydro; /**< List of hydro active force matrices. */
};
#endif
